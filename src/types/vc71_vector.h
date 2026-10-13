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
    if constexpr ((Bytes & (Bytes - 1)) == 0) {
        constexpr unsigned shift = []() constexpr {
            unsigned n = 0;
            for (size_t b = Bytes; b > 1; b >>= 1) ++n;
            return n;
        }();
        return static_cast<uint32_t>(static_cast<int32_t>(end - begin) >> shift);
    } else {
        return static_cast<uint32_t>(static_cast<int32_t>(end - begin) / static_cast<int32_t>(Bytes));
    }
}

// The binary retains this unsigned max_size comparison even though its limit is
// UINT32_MAX. A volatile local keeps the original (unreachable) call boundary.
__forceinline void vector_check_length(void* self, uint32_t count) {
    volatile uint32_t limit = 0xFFFFFFFFu;
    if (count > limit) vector_length_error(self, nullptr);
}

template <class T> __forceinline uint8_t vector_buy(vector<T>* self, uint32_t count) {
    self->first = nullptr;
    self->last = nullptr;
    self->end = nullptr;
    if (!count) return 0;
    vector_check_length(self, count);
    T* block = static_cast<T*>(allocate_buffer(sizeof(T) * count, 0, 0));
    self->end = pointer<T>(address(block) + sizeof(T) * count);
    self->first = block;
    self->last = block;
    return 1;
}

// Reserve never grows geometrically. Power-of-two shapes leave new end in EAX;
// reciprocal-division shapes leave the allocation address instead.
template <class Ops> __forceinline uint32_t vector_reserve(
    vector<typename Ops::value_type>* self, uint32_t count) {
    using T = typename Ops::value_type;
    constexpr uint32_t bytes = sizeof(T);
    vector_check_length(self, count);
    uint32_t first = address(self->first);
    uint32_t result = first ? distance<bytes>(address(self->end), first) : 0;
    if (result < count) {
        T* block = static_cast<T*>(allocate_buffer(bytes * count, 0, 0));
        try {
            Ops::copy(self, address(self->first), address(self->last), address(block));
        } catch (...) {
            release_failed_buffer(block, pointer<void>(count));
            rethrow_exception(nullptr, nullptr);
            __assume(0);
        }
        first = address(self->first);
        uint32_t size = first ? distance<bytes>(address(self->last), first) : 0;
        if (first) {
            Ops::destroy(first, address(self->last));
            if (self->first) release_buffer(self->first);
        }
        self->end = pointer<T>(address(block) + bytes * count);
        self->last = pointer<T>(address(block) + bytes * size);
        self->first = block;
        if constexpr (Ops::returns_last) result = address(block) + bytes * size;
        else result = Ops::returns_end ? address(block) + bytes * count : address(block);
    }
    return result;
}

template <class Ops> __forceinline vector<typename Ops::value_type>* vector_copy_construct(
    vector<typename Ops::value_type>* self, const vector<typename Ops::value_type>* source) {
    using T = typename Ops::value_type;
    uint32_t first = address(source->first);
    uint32_t count = first ? distance<sizeof(T)>(address(source->last), first) : 0;
    if (vector_buy(self, count)) {
        try {
            self->last = pointer<T>(Ops::copy(self, address(source->first),
                                             address(source->last), address(self->first)));
        } catch (...) {
            Ops::cleanup(self);
            rethrow_exception(nullptr, nullptr);
            __assume(0);
        }
    }
    return self;
}

// Single-value assignment, not iterator-to-iterator assignment: the source is
// deliberately unchanged throughout the loop. Empty ranges leave EAX unspecified.
template <class Ops> __forceinline void vector_assign_value(
    typename Ops::value_type* begin, typename Ops::value_type* end,
    const typename Ops::value_type* value) {
    for (auto* p = begin; p != end; ++p) Ops::assign(p, value);
}

template <class Ops> __forceinline void vector_destroy_range(
    typename Ops::value_type* begin, typename Ops::value_type* end) {
    for (auto* p = begin; p != end; ++p) Ops::destroy(p);
}

template <class Ops> __forceinline uint32_t vector_fill_assign(
    uint32_t begin, uint32_t end, uint32_t value) {
    for (uint32_t p = begin; p != end; p += sizeof(typename Ops::value_type))
        Ops::assign(pointer<typename Ops::value_type>(p),
                    pointer<typename Ops::value_type>(value));
    return end;
}

template <size_t Bytes, size_t Skip = 0> struct trivial_assignment {
    using value_type = element<Bytes>;
    static __forceinline void assign(value_type* dest, const value_type* source) {
        for (size_t i = Skip / 4; i < Bytes / 4; ++i)
            reinterpret_cast<uint32_t*>(dest)[i] = reinterpret_cast<const uint32_t*>(source)[i];
    }
};

template <size_t Bytes> __forceinline uint32_t vector_copy_backward(
    uint32_t begin, uint32_t end, uint32_t dest) {
    while (end != begin) {
        end -= Bytes;
        dest -= Bytes;
        trivial_assignment<Bytes>::assign(pointer<element<Bytes>>(dest), pointer<element<Bytes>>(end));
    }
    return dest;
}

template <class Ops> __forceinline uint32_t vector_uninitialized_forward(
    uint32_t begin, uint32_t end, uint32_t dest, uint32_t context) {
    // Only the low byte of the original dummy dispatch tag is initialized.
    // It is an empty type, not an integer argument used by the callee.
    return Ops::copy(begin, end, dest, context, 0);
}

// _Ufill's forwarding overload keeps the container context and dispatch tag at
// the cdecl boundary; the return is computed independently of the helper's EAX.
template <class Ops> __forceinline uint32_t vector_ufill(
    void* self, uint32_t dest, uint32_t count, const typename Ops::value_type* value) {
    Ops::fill(dest, count, value, self, Ops::zero_tag ? 0 : address(value));
    return dest + sizeof(typename Ops::value_type) * count;
}

struct vector_bool {
    uint32_t bits;
    vector<uint32_t> words;
};
static_assert(offsetof(vector_bool, words) == 4 && sizeof(vector_bool) == 20);

template <class Ops> __forceinline void vector_bool_trim(vector_bool* self, uint32_t count) {
    volatile uint32_t limit = 0xFFFFFFFFu;
    if (count > limit) Ops::length_error(self);
    uint32_t words = (count + 31) >> 5;
    uint32_t first = address(self->words.first);
    if (first && words < distance<4>(address(self->words.last), first)) {
        uint32_t last = first + 4 * words;
        if (last != address(self->words.last)) {
            // erase(end_of_kept_words, end): the source range is empty, but the
            // original still crosses memcpy before updating the word-vector end.
            self->words.last = pointer<uint32_t>(Ops::copy(last, address(self->words.last), 0));
        }
    }
    self->bits = count;
    uint32_t remainder = count & 31;
    if (remainder) *pointer<uint32_t>(address(self->words.first) + 4 * words - 4)
                    &= (uint32_t(1) << remainder) - 1;
}

template <class Ops> __forceinline uint32_t vector_bool_insert(
    vector_bool* self, uint32_t offset, uint32_t* word, uint32_t count) {
    uint32_t position = offset + 32 * distance<4>(address(word), address(self->words.first));
    if (count) {
        uint32_t old = self->bits;
        if (0xFFFFFFFFu - old < count) Ops::length_error(self);
        Ops::resize(&self->words, (old + count + 31) >> 5, 0);
        old = self->bits;
        if (!old) {
            self->bits = count;
            return position;
        }
        struct iterator { uint32_t offset; uint32_t* word; } old_end;
        old_end.offset = old & 31;
        old_end.word = pointer<uint32_t>(address(self->words.first) + 4 * (old >> 5));
        uint32_t added = old + count;
        self->bits = added;
        uint32_t* new_end = self->words.first;
        uint32_t new_offset = 0;
        if (added) {
            new_end = pointer<uint32_t>(address(new_end) + 4 * (added >> 5));
            new_offset = added & 31;
        }
        Ops::move(&old_end, position & 31,
                  pointer<uint32_t>(address(self->words.first) + 4 * (position >> 5)),
                  old_end.offset, old_end.word, new_offset, new_end);
    }
    return position;
}

template <class Ops> __forceinline uint8_t vector_insert_range(
    vector<typename Ops::value_type>* self, typename Ops::value_type* position,
    typename Ops::value_type* begin, typename Ops::value_type* end, uint32_t tag) {
    using T = typename Ops::value_type;
    constexpr uint32_t bytes = sizeof(T);
    uint32_t count = distance<bytes>(address(end), address(begin));
    uint32_t result = count;
    uint32_t first = address(self->first);
    uint32_t capacity = first ? distance<bytes>(address(self->end), first) : 0;
    uint32_t pos = address(position);
    if (count) {
        uint32_t size = first ? distance<bytes>(address(self->last), first) : 0;
        if (0xFFFFFFFFu - size < count) vector_length_error(self, nullptr);
        size = first ? distance<bytes>(address(self->last), first) : 0;
        if (capacity >= count + size) {
            uint32_t last = address(self->last);
            if (distance<bytes>(last, pos) >= count) {
                uint32_t split = last - bytes * count;
                self->last = pointer<T>(Ops::copy(self, split, last, last));
                Ops::copy_backward(pos, split, last);
                result = Ops::assign(address(begin), address(end), pos);
            } else {
                Ops::copy(self, pos, last, pos + bytes * count);
                uint32_t split = address(begin) + bytes * distance<bytes>(address(self->last), pos);
                // The original's second try block has no cleanup action for these
                // trivial elements; only reallocation releases a failed buffer.
                Ops::copy_range(self, split, address(end), address(self->last));
                self->last = pointer<T>(address(self->last) + bytes * count);
                result = Ops::assign(address(begin), split, pos);
            }
        } else {
            uint32_t grown = 0xFFFFFFFFu - (capacity >> 1) >= capacity
                             ? capacity + (capacity >> 1) : 0;
            if (grown < count + size) grown = count + size;
            T* block = static_cast<T*>(allocate_buffer(bytes * grown, 0, 0));
            try {
                uint32_t next = Ops::copy(self, address(self->first), pos, address(block));
                next = Ops::copy_range(self, address(begin), address(end), next);
                Ops::copy(self, pos, address(self->last), next);
            } catch (...) {
                release_failed_buffer(block, pointer<void>(tag));
                rethrow_exception(nullptr, nullptr);
                __assume(0);
            }
            first = address(self->first);
            result = first ? distance<bytes>(address(self->last), first) : 0;
            uint32_t new_size = result + count;
            if (first) result = release_buffer(self->first);
            self->end = pointer<T>(address(block) + bytes * grown);
            self->last = pointer<T>(address(block) + bytes * new_size);
            self->first = block;
        }
    }
    return static_cast<uint8_t>(result);
}

template <class Ops> __forceinline void vector_xlen(void* edx) {
    element<28> message;
    element<40> exception;
    auto* m = reinterpret_cast<uint32_t*>(&message);
    m[6] = 15;
    m[5] = 0;
    message.bytes[4] = 0;
    Ops::text(&message, edx);
    Ops::exception_construct(&exception);
    auto* e = reinterpret_cast<uint32_t*>(&exception);
    e[0] = Ops::runtime_error_vtable();
    e[9] = 15;
    e[8] = 0;
    exception.bytes[16] = 0;
    Ops::string_copy(exception.bytes + 12, &message);
    e[0] = Ops::length_error_vtable();
    Ops::throw_error(&exception);
    __assume(0);
}

template <class Ops, class T> __forceinline auto insert_finish_value(T* value, uint32_t result, int)
    -> decltype(Ops::finish_value(value, result)) { return Ops::finish_value(value, result); }
template <class Ops, class T> __forceinline uint32_t insert_finish_value(T* value, uint32_t result, long) {
    if constexpr (Ops::owns_value) return Ops::release_value(value);
    return result;
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
        return insert_finish_value<Ops>(&value, result, 0);
    }
};

template <class Ops, class V> __forceinline auto insert_growth_size(V* self, int)
    -> decltype(Ops::growth_size(self)) { return Ops::growth_size(self); }
template <class Ops, class V> __forceinline uint32_t insert_growth_size(V* self, long) {
    return self->first ? distance<sizeof(typename Ops::value_type)>(address(self->last), address(self->first)) : 0;
}
template <class Ops, class V> __forceinline auto insert_initial_result(V* self, uint32_t capacity, int)
    -> decltype(Ops::initial_result(self, capacity)) { return Ops::initial_result(self, capacity); }
template <class Ops, class V> __forceinline uint32_t insert_initial_result(V*, uint32_t capacity, long) { return capacity; }
template <class Ops> __forceinline auto insert_allocated_result(uint32_t block, uint32_t result, int)
    -> decltype(Ops::allocated_result(block, result)) { return Ops::allocated_result(block, result); }
template <class Ops> __forceinline uint32_t insert_allocated_result(uint32_t, uint32_t result, long) { return result; }
template <class Ops> __forceinline auto insert_split(uint32_t last, uint32_t count, int)
    -> decltype(Ops::split(last, count)) { return Ops::split(last, count); }
template <class Ops> __forceinline uint32_t insert_split(uint32_t last, uint32_t count, long) {
    return last - sizeof(typename Ops::value_type) * count;
}
template <class Ops, class V, class T> __forceinline auto insert_fill_spare(
    V* self, uint32_t dest, uint32_t count, const T* value, int)
    -> decltype(Ops::fill_spare(self, dest, count, value)) { return Ops::fill_spare(self, dest, count, value); }
template <class Ops, class V, class T> __forceinline uint32_t insert_fill_spare(
    V* self, uint32_t dest, uint32_t count, const T* value, long) { return Ops::fill(self, dest, count, value); }
template <class Ops, class T> __forceinline auto insert_assign_spare(
    uint32_t begin, uint32_t end, const T* value, uint32_t bytes, int)
    -> decltype(Ops::assign_spare(begin, end, value, bytes)) { return Ops::assign_spare(begin, end, value, bytes); }
template <class Ops, class T> __forceinline uint32_t insert_assign_spare(
    uint32_t begin, uint32_t end, const T* value, uint32_t, long) { return Ops::assign(begin, end, value); }
template <class Ops, class V> __forceinline auto insert_release_failed(void* block, V* self, uint32_t count, int)
    -> decltype(Ops::release_failed(block, self, count), void()) { Ops::release_failed(block, self, count); }
template <class Ops, class V> __forceinline void insert_release_failed(void* block, V* self, uint32_t, long) {
    release_failed_buffer(block, self);
}

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
    uint32_t result = insert_initial_result<Ops>(self, capacity, 0);
    if (count) {
        uint32_t size = first ? distance<bytes>(address(self->last), first) : 0;
        if (0xFFFFFFFFu - size < count) vector_length_error(self, nullptr);
        // Reload after the potentially throwing boundary, matching the original.
        size = first ? distance<bytes>(address(self->last), first) : 0;
        const uint32_t added_bytes = bytes * count;
        if (capacity >= count + size) {
            const uint32_t old_last = address(self->last);
            if (distance<bytes>(old_last, pos) >= count) {
                const uint32_t split = insert_split<Ops>(old_last, count, 0);
                self->last = pointer<T>(Ops::copy_spare(self, split, old_last, old_last));
                Ops::copy_backward(pos, split, old_last);
                result = Ops::assign(pos, pos + added_bytes, &saved.value);
            } else {
                Ops::copy_spare(self, pos, old_last, pos + added_bytes);
                try {
                    insert_fill_spare<Ops>(self, address(self->last),
                              count - distance<bytes>(address(self->last), pos), &saved.value, 0);
                } catch (...) {
                    Ops::destroy_failed(self, pos + added_bytes, address(self->last) + added_bytes);
                    rethrow_exception(nullptr, nullptr);
                    __assume(0);
                }
                self->last = pointer<T>(address(self->last) + added_bytes);
                result = insert_assign_spare<Ops>(pos, address(self->last) - added_bytes, &saved.value, added_bytes, 0);
            }
        } else {
            uint32_t grown = 0xFFFFFFFFu - (capacity >> 1) >= capacity
                           ? capacity + (capacity >> 1) : 0;
            if (grown < count + size) grown = count + insert_growth_size<Ops>(self, 0);
            T* block = static_cast<T*>(allocate_buffer(bytes * grown, 0, 0));
            uint32_t constructed_end = address(block);
            try {
                constructed_end = Ops::copy_new(self, address(self->first), pos, address(block));
                constructed_end = Ops::fill(self, constructed_end, count, &saved.value);
                Ops::copy_new(self, pos, address(self->last), constructed_end);
            } catch (...) {
                Ops::destroy_failed(self, address(block), constructed_end);
                insert_release_failed<Ops>(block, self, count, 0);
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
            result = insert_allocated_result<Ops>(address(block), result, 0);
        }
    }
    return saved.finish(result);
}

} // namespace vc71
} // namespace legacy
