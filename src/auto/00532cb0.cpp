#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Callee 0x004856E0: __thiscall, two stack args, pops 8.
// Reserves/resizes the internal character buffer of the string-like object.
CSF_FUNC(void*, __fastcall, ReserveSsoOrHeapCapacity, 0x004856E0,
         (void* self, void* edx, uint32_t count, uint32_t flag));

// Returns a pointer to the element at index `count` past the start of the
// string buffer, after reserving room for `count` elements. The destination
// slot (first stack argument) receives that pointer, and the slot's address
// is returned in eax.
//
// Layout (inferred from disassembly):
//   +0x04 : inline buffer, or pointer to heap buffer when capacity >= 8
//   +0x18 : capacity (SSO threshold 8)
static uint32_t* __fastcall MenuEula_ComputeWriteCursorForEndPointer(
        void* thisPtr, void* /*edx*/, uint32_t* out, uint32_t endAddr)
{
    uint8_t* self = reinterpret_cast<uint8_t*>(thisPtr);
    uint8_t* bufferAddr = self + 4;

    // Choose the active data pointer: inline buffer or heap pointer.
    uint32_t capacity = CSF_AT(uint32_t, self, 0x18);
    uint8_t* dataStart = (capacity < 8)
        ? bufferAddr
        : reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, self, 0x04));

    // Element count between dataStart and endAddr (signed diff, halved).
    uint32_t count = 0;
    if (endAddr != 0) {
        int32_t diff = static_cast<int32_t>(endAddr)
                     - static_cast<int32_t>(reinterpret_cast<uintptr_t>(dataStart));
        count = static_cast<uint32_t>(diff >> 1);
    }

    ReserveSsoOrHeapCapacity(thisPtr, nullptr, count, 1);

    // Reload the data pointer; the call may have changed the buffer.
    capacity = CSF_AT(uint32_t, self, 0x18);
    if (capacity >= 8) {
        bufferAddr = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, self, 0x04));
    }

    *out = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(bufferAddr) + count * 2u);
    return out;
}

HOOK(0x00532CB0, MenuEula_ComputeWriteCursorForEndPointer, "auto");
