#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Callees of 0x00461750, bound by address.
CSF_FUNC(char*, __fastcall, InitScratchBlock, 0x0045BD10, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __fastcall, ApplyScratchToGlobalState, 0x0050E290, (void* self, void* edx, int a2));

// objectRef is a pointer to an object whose +4 holds a string header. The header's capacity at +0x1c decides
// where the character data lives: the inline buffer (header+8) while capacity < 0x10, otherwise the heap
// pointer stored at header+8.
static uint32_t GetStringDataFromObjectRef(uint32_t objectRef)
{
    uint8_t* object = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(objectRef));
    uint8_t* header = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(CSF_AT(uint32_t, object, 4)));
    uint32_t capacity = CSF_AT(uint32_t, header, 0x1c);
    if (capacity < 0x10)
        return static_cast<uint32_t>(reinterpret_cast<uintptr_t>(header + 8));
    return CSF_AT(uint32_t, header, 8);
}

static int __fastcall BuildLookupFromThreeStrings(void* self, void* /*edx*/, int /*a2*/, int /*a3*/, uint32_t* nameRefs)
{
    // Three 0x4C-byte entries (0xE4 bytes, the original's whole frame): InitScratchBlock (0x45BD10) initializes all
    // three. [0], [19] and [38] are each entry's first dword; Hex-Rays split the block into v19[38] + v20, and a
    // 39-dword buffer here let the init run 0x48 bytes past it (9.sav crashed ~40 s after load, 2026-10-09).
    uint32_t scratchBlock[57];
    InitScratchBlock(scratchBlock, nullptr);

    scratchBlock[0] = GetStringDataFromObjectRef(nameRefs[0]);
    scratchBlock[19] = GetStringDataFromObjectRef(nameRefs[1]);
    scratchBlock[38] = GetStringDataFromObjectRef(nameRefs[2]);

    // The original pushes &scratchBlock before calling GetGlobalState_50D190 and does not pop it there;
    // sub_50E290 (ret 4) consumes that dword as its a2, so it is passed explicitly here.
    int globalState = GetGlobalState_50D190();
    int result = ApplyScratchToGlobalState(
        reinterpret_cast<void*>(static_cast<uintptr_t>(globalState)),
        nullptr,
        static_cast<int>(reinterpret_cast<uintptr_t>(scratchBlock)));

    CSF_AT(int, self, 0x18) = 11;
    CSF_AT(uint8_t, self, 0x82) = 0;
    return result;
}

HOOK(0x00461750, BuildLookupFromThreeStrings, "auto");
