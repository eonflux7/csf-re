#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/vc71_vector.h"
using Vector = legacy::vc71::vector<uint32_t>;
using legacy::vc71::address;
using legacy::vc71::pointer;
using legacy::vc71::distance;
CSF_FUNC(uint32_t*, __cdecl, CopySpare, 0x0047EBA0, (uint32_t* begin, uint32_t* end, uint32_t* dest, int context, int tag));
CSF_FUNC(uint32_t*, __cdecl, FillSpare, 0x0047EEC0, (uint32_t* dest, int count, const uint32_t* source, int context, int tag));
CSF_FUNC(uint32_t, __cdecl, CopyNew, 0x0047F240, (uint32_t begin, uint32_t end, uint32_t dest, uint32_t context));
CSF_FUNC(uint32_t, __cdecl, FillNew, 0x0047F270, (uint32_t dest, uint32_t count, uint32_t source, uint32_t context));
CSF_FUNC(void*, __cdecl, CopyBytes, 0x007AEE60, (void* dest, const void* source, size_t bytes));

static uint32_t __fastcall Insert(Vector* self, void*, uint32_t* position, uint32_t count, const uint32_t* source) {
    uint32_t value = *source;
    uint32_t first = address(self->first);
    uint32_t capacity = first ? distance<4>(address(self->end), first) : 0;
    uint32_t result = capacity;
    if (!count) return result;
    uint32_t size = first ? distance<4>(address(self->last), first) : 0;
    if (0xFFFFFFFFu - size < count) legacy::vc71::vector_length_error(self, nullptr);
    size = first ? distance<4>(address(self->last), first) : 0;
    const uint32_t pos = address(position), bytes = count * 4;
    if (capacity >= count + size) {
        const uint32_t last = address(self->last);
        if (distance<4>(last, pos) >= count) {
            const uint32_t split = last - bytes;
            self->last = CopySpare(pointer<uint32_t>(split), pointer<uint32_t>(last), pointer<uint32_t>(last), address(self), 0);
            const uint32_t moved = distance<4>(split, pos) * 4;
            CopyBytes(pointer<void>(last - moved), position, moved);
            result = pos + bytes;
            for (uint32_t p = pos; p != result; p += 4) *pointer<uint32_t>(p) = value;
        } else {
            CopySpare(position, pointer<uint32_t>(last), pointer<uint32_t>(pos + bytes), address(self), 0);
            try {
                FillSpare(self->last, count - distance<4>(address(self->last), pos), &value, address(self), 0);
            } catch (...) {
                legacy::vc71::rethrow_exception(nullptr, nullptr);
                __assume(0);
            }
            result = bytes;
            self->last = pointer<uint32_t>(address(self->last) + bytes);
            const uint32_t old_last = address(self->last) - bytes;
            for (uint32_t p = pos; p != old_last; p += 4) {
                result = value;
                *pointer<uint32_t>(p) = value;
            }
        }
    } else {
        uint32_t grown = 0xFFFFFFFFu - (capacity >> 1) >= capacity ? capacity + (capacity >> 1) : 0;
        if (grown < count + size) grown = count + (first ? distance<4>(address(self->last), first) : 0);
        void* block = legacy::vc71::allocate_buffer(4 * grown, 0, 0);
        try {
            uint32_t middle = CopyNew(address(self->first), pos, address(block), address(self));
            FillNew(middle, count, address(&value), address(self));
            middle += bytes; // Original ignores FillNew's return.
            CopyNew(pos, address(self->last), middle, address(self));
        } catch (...) {
            legacy::vc71::release_failed_buffer(block, pointer<void>(grown));
            legacy::vc71::rethrow_exception(nullptr, nullptr);
            __assume(0);
        }
        first = address(self->first);
        const uint32_t new_size = count + (first ? distance<4>(address(self->last), first) : 0);
        if (first) legacy::vc71::release_buffer(self->first);
        result = address(block);
        self->end = pointer<uint32_t>(result + 4 * grown);
        self->last = pointer<uint32_t>(result + 4 * new_size);
        self->first = pointer<uint32_t>(result);
    }
    return result;
}
HOOK(0x0047EF80, Insert, "stl");
