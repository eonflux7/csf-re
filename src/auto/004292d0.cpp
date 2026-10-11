#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(void, __fastcall, sub_428CE0, 0x00428CE0, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, sub_428680, 0x00428680, (void* self, void* edx, int a2, int a3));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());

// Applies the cheat flags stored in the global state's cheat table to this menu.
static uint32_t* __fastcall C_008300c0_ApplyCheatStates(C_008300c0* self, void* /*edx*/)
{
    sub_428CE0(self, nullptr);

    void* globalStateObj = reinterpret_cast<void*>(GetGlobalState_50D190());
    void* cheatSource = CSF_AT(void*, globalStateObj, 0x10);

    // vtbl[0x30 / 4] : returns the active-cheat record for this source (NULL if none).
    uint8_t* activeCheat = reinterpret_cast<uint8_t*>(
        ((void* (__fastcall*)(void*, void*))CSF_AT(void**, cheatSource, 0)[0x30 / 4])(cheatSource, nullptr));

    if (activeCheat)
    {
        if (CSF_AT(uint8_t, activeCheat, 0x1C))
            sub_428680(self, nullptr, 1, 0);
        if (CSF_AT(uint8_t, activeCheat, 0x1E))
            sub_428680(self, nullptr, 1, 1);
        if (CSF_AT(uint8_t, activeCheat, 0x1D))
            sub_428680(self, nullptr, 1, 2);
    }

    // vtbl[0x34 / 4] : returns a 256-entry per-key byte table.
    uint8_t* keyTable = reinterpret_cast<uint8_t*>(
        ((void* (__fastcall*)(void*, void*))CSF_AT(void**, cheatSource, 0)[0x34 / 4])(cheatSource, nullptr));

    uint32_t* result = reinterpret_cast<uint32_t*>(keyTable);
    if (keyTable)
    {
        for (unsigned int i = 0; i < 0x100; ++i)
        {
            if (keyTable[i])
                result = sub_428680(self, nullptr, 0, static_cast<int>(i));
        }
    }
    return result;
}

HOOK(0x004292D0, C_008300c0_ApplyCheatStates, "auto");
