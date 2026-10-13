#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int, __fastcall, S_004f21d0_SetPattern, 0x00697100, (S_004f21d0* self, void* , uint8_t* a2));
CSF_FUNC(int, __fastcall, HudStringShow_ShowWithStyle, 0x004F39E0, (void* self, void* edx, void* arg0, float a5, float a6, float a2));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

CSF_GLOBAL(uint8_t, g_byte_82E65C, 0x0082E65C);

// Stack args: a4 (float bits), a5 (float), a6 (float bits); callee pops 12 bytes.
// The return value is the leftover eax the original hands back: whatever the
// last call returned, or the block pointer when the free path is skipped.
static int __fastcall HudLayoutTextAndValue(void* self, void* /*edx*/, uint32_t a4Bits, float a5, uint32_t a6Bits)
{
    // Stack-resident S_004f21d0 object: 0x1C bytes.
    //   +0x04 = Block pointer (ebp-0x18), +0x18 = capacity/size dword (ebp-4).
    alignas(8) uint8_t patternBytes[0x1C];
    S_004f21d0* pattern = reinterpret_cast<S_004f21d0*>(patternBytes);

    S_004f21d0_SetPattern(pattern, nullptr, &g_byte_82E65C);

    float a4AsFloat;
    float a6AsFloat;
    std::memcpy(&a4AsFloat, &a4Bits, sizeof(float));
    std::memcpy(&a6AsFloat, &a6Bits, sizeof(float));

    int eaxValue = HudStringShow_ShowWithStyle(self, nullptr, pattern, a4AsFloat, a5, a6AsFloat);

    if (CSF_AT(uint32_t, patternBytes, 0x18) >= 8) {
        void* block = CSF_AT(void*, patternBytes, 0x04);
        eaxValue = static_cast<int>(reinterpret_cast<intptr_t>(block));
        if (block) {
            eaxValue = Mem_FreeDebug(block);
        }
    }

    return eaxValue;
}

HOOK(0x004F3EA0, HudLayoutTextAndValue, "auto");
