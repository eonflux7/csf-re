// Binary-derived VC7.1 vector layout and _Insert_n. No STL implementation source used.
#pragma once
#include <cstddef>
#include <cstdint>
#include "globals.h"

namespace legacy {
namespace vc71 {

template <size_t Bytes> struct alignas(4) element {
    uint8_t bytes[Bytes];
};

template <class T> struct vector {
    uint32_t allocator;
    T* first;
    T* last;
    T* end;
};
using vector_layout = vector<element<4>>;
static_assert(sizeof(void*) == 4);
static_assert(sizeof(vector_layout) == 16);
static_assert(alignof(vector_layout) == 4);
static_assert(offsetof(vector_layout, allocator) == 0);
static_assert(offsetof(vector_layout, first) == 4);
static_assert(offsetof(vector_layout, last) == 8);
static_assert(offsetof(vector_layout, end) == 12);
static_assert(sizeof(element<4>) == 4 && sizeof(element<8>) == 8 && sizeof(element<16>) == 16);

CSF_FUNC(void*, __cdecl, allocate_buffer, 0x00690DC0, (size_t bytes, int file, int line));
// These members leave the full EAX from the boundary intact (the public API describes AL).
CSF_FUNC(uint32_t, __cdecl, release_buffer, 0x00690E60, (void* block));
CSF_FUNC(void*, __stdcall, release_failed_buffer, 0x006C8D30, (void* block, void* context));
CSF_FUNC(void, __stdcall, rethrow_exception, 0x007AF666, (void* object, void* info));
CSF_FUNC(void, __fastcall, vector_length_error, 0x006CE410, (void* self, void* edx));

__forceinline uint32_t address(const void* p) { return reinterpret_cast<uintptr_t>(p); }
template <class T> __forceinline T* pointer(uint32_t p) { return reinterpret_cast<T*>(p); }

// VC7.1 emits SAR for the power-of-two strides in these three clusters. Integer address
// arithmetic also avoids host C++ pointer subtraction between an empty vector's nulls.
template <size_t Bytes> __forceinline uint32_t distance(uint32_t end, uint32_t begin) {
    static_assert(Bytes == 4 || Bytes == 8 || Bytes == 16);
    constexpr unsigned shift = Bytes == 4 ? 2 : Bytes == 8 ? 3 : 4;
    return static_cast<uint32_t>(static_cast<int32_t>(end - begin) >> shift);
}

template <class Ops> struct insert_value {
    using T = typename Ops::value_type;
    T value; // Some constructors deliberately leave the allocator word untouched.
    // All first-slice FuncInfo unwind maps have zero actions: this temporary is
    // released on normal exit only. Adding a C++ owning destructor changes throws.
    __forceinline explicit insert_value(const T* source) {
        Ops::construct(&value, source);
    }
    __forceinline uint32_t finish(uint32_t result) {
        if constexpr (Ops::owns_value) return Ops::release_value(&value);
        return result;
    }
};

// Traits separate element semantics from the container algorithm. Range calls remain at
// their original boundaries until the range-helper families are replaced in a later slice.
template <class Ops> __forceinline uint32_t vector_insert_n(
    vector<typename Ops::value_type>* self, typename Ops::value_type* position,
    uint32_t count, const typename Ops::value_type* source) {
    using T = typename Ops::value_type;
    constexpr uint32_t bytes = sizeof(T);
    insert_value<Ops> saved(source); // required even when count == 0; protects aliased values
    const uint32_t pos = address(position);
    uint32_t first = address(self->first);
    uint32_t capacity = first ? distance<bytes>(address(self->end), first) : 0;
    uint32_t result = capacity;
    if (count) {
        uint32_t size = first ? distance<bytes>(address(self->last), first) : 0;
        if (0xFFFFFFFFu - size < count) vector_length_error(self, nullptr);
        // Reload after the potentially throwing boundary, matching the original.
        size = first ? distance<bytes>(address(self->last), first) : 0;
        const uint32_t added_bytes = bytes * count;
        if (capacity >= count + size) {
            const uint32_t old_last = address(self->last);
            if (distance<bytes>(old_last, pos) >= count) {
                const uint32_t split = old_last - added_bytes;
                self->last = pointer<T>(Ops::copy_spare(self, split, old_last, old_last));
                Ops::copy_backward(pos, split, old_last);
                result = Ops::assign(pos, pos + added_bytes, &saved.value);
            } else {
                Ops::copy_spare(self, pos, old_last, pos + added_bytes);
                try {
                    Ops::fill(self, address(self->last),
                              count - distance<bytes>(address(self->last), pos), &saved.value);
                } catch (...) {
                    Ops::destroy_failed(self, pos + added_bytes, address(self->last) + added_bytes);
                    rethrow_exception(nullptr, nullptr);
                    __assume(0);
                }
                self->last = pointer<T>(address(self->last) + added_bytes);
                result = Ops::assign(pos, address(self->last) - added_bytes, &saved.value);
            }
        } else {
            uint32_t grown = 0xFFFFFFFFu - (capacity >> 1) >= capacity
                           ? capacity + (capacity >> 1) : 0;
            if (grown < count + size) grown = count + size;
            T* block = static_cast<T*>(allocate_buffer(bytes * grown, 0, 0));
            uint32_t constructed_end = address(block);
            try {
                constructed_end = Ops::copy_new(self, address(self->first), pos, address(block));
                constructed_end = Ops::fill(self, constructed_end, count, &saved.value);
                Ops::copy_new(self, pos, address(self->last), constructed_end);
            } catch (...) {
                Ops::destroy_failed(self, address(block), constructed_end);
                release_failed_buffer(block, self);
                rethrow_exception(nullptr, nullptr);
                __assume(0);
            }
            first = address(self->first);
            result = first ? distance<bytes>(address(self->last), first) : 0;
            const uint32_t new_size = result + count;
            if (first) {
                Ops::destroy_old(self, first, address(self->last));
                if (self->first) result = release_buffer(self->first);
            }
            self->end = pointer<T>(address(block) + bytes * grown);
            self->last = pointer<T>(address(block) + bytes * new_size);
            self->first = block;
        }
    }
    return saved.finish(result);
}

} // namespace vc71
} // namespace legacy
