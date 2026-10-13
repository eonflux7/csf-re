#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_FUNC(uint32_t*, __cdecl, VectorConstructAtEnd, 0x00732530, (uint32_t* a1, int a2, uint32_t* a3, int a4, int a5));
CSF_FUNC(uint32_t*, __fastcall, VectorReallocAndEmplace, 0x007325C0, (void* self, void* edx, uint32_t* a2, uint32_t* a3, int a4));

// Container with 16-byte elements: +4 = begin, +8 = end (next free), +12 = capacity end.
// Appends one element (value passed by the stack argument) and returns the callee's result.
static uint32_t* __fastcall VectorEmplaceBack16(void* self, void* /*edx*/, uint32_t* argValue)
{
    uint32_t begin = CSF_AT(uint32_t, self, 4);

    uint32_t usedCount = 0;
    if (begin != 0)
        usedCount = static_cast<uint32_t>(static_cast<int32_t>(CSF_AT(uint32_t, self, 8) - begin) >> 4);

    if (begin == 0 ||
        usedCount >= static_cast<uint32_t>(static_cast<int32_t>(CSF_AT(uint32_t, self, 12) - begin) >> 4))
    {
        // No free slot: grow and insert through the reallocating path.
        uint32_t* endPtr = CSF_AT(uint32_t*, self, 8);
        return VectorReallocAndEmplace(self, nullptr,
                                       reinterpret_cast<uint32_t*>(&argValue),
                                       endPtr,
                                       static_cast<int>(reinterpret_cast<uintptr_t>(argValue)));
    }

    // Free slot available: construct at end, then advance end by one element (16 bytes).
    uint32_t* slot = CSF_AT(uint32_t*, self, 8);
    uint32_t* result = VectorConstructAtEnd(slot, 1, argValue,
                                            static_cast<int>(reinterpret_cast<uintptr_t>(self)),
                                            static_cast<int>(reinterpret_cast<uintptr_t>(argValue)));
    CSF_AT(uint32_t*, self, 8) = slot + 4;
    return result;
}

HOOK(0x007324C0, VectorEmplaceBack16, "auto");
