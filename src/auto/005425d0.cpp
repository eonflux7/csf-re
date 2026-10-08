#include <cstdint>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// ---------------------------------------------------------------------------
// Original engine entry points used by this menu tick.
// ---------------------------------------------------------------------------
CSF_FUNC(int, __cdecl, GetGlobalState_50D190,        0x50D190, ());
CSF_FUNC(int, __cdecl, GetGlobalDword_73C020, 0x73C020, ());
CSF_FUNC(int, __cdecl, ReturnZero,            0x4B2A30, ());
CSF_FUNC(int, __cdecl, Sub_743FD0,            0x743FD0, ());
CSF_FUNC(int,  __fastcall, Sub_647AA0,        0x647AA0, (void* self, void* edx, int a, int b));
CSF_FUNC(char, __fastcall, Sub_655520,        0x655520, (void* self, void* edx, int a, int b));

// ---------------------------------------------------------------------------
// Constant data referenced by the original code.
// ---------------------------------------------------------------------------
CSF_GLOBAL(uint8_t, g_MenuMpCharacterSelectActive, 0x8BC121);  // byte_8BC121
CSF_GLOBAL(uint8_t, g_MenuMpCharacterSelectLookup, 0x8B9A90);  // ecx object for Sub_647AA0

// 0x5425D0 ends with a tail jmp into 0x73D6B0 (ecx = GetGlobalDword_73C020()); the
// packet's callee/data lists include everything that 0x73D6B0 touches, so the merged
// body is reproduced here inline.
static char __cdecl MenuMpCharacterSelect_TickCameraPreview()
{
    int state = GetGlobalState_50D190();
    char result = (char)state;

    if (CSF_AT(uint8_t, (void*)(uintptr_t)state, 0xd0) != 0)
    {
        int obj = GetGlobalDword_73C020();

        result = (char)g_MenuMpCharacterSelectActive;
        if (g_MenuMpCharacterSelectActive == 0 &&
            CSF_AT(uint32_t, (void*)(uintptr_t)obj, 0x98) == 6)
        {
            Sub_743FD0();

            int spawnId = ReturnZero();
            int entry = Sub_647AA0(&g_MenuMpCharacterSelectLookup, nullptr, 4, spawnId);

            int handle = 0;
            if (entry != 0)
            {
                uint32_t inner = CSF_AT(uint32_t, (void*)(uintptr_t)entry, 0xc);
                if (inner != 0)
                    handle = (int)CSF_AT(uint32_t, (void*)(uintptr_t)inner, 8);
            }

            result = Sub_655520((void*)(uintptr_t)handle, nullptr, -1, 1);
        }
    }

    return result;
}

HOOK(0x5425D0, MenuMpCharacterSelect_TickCameraPreview, "auto");
