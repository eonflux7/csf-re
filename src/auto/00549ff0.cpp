#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __fastcall, GameBddLoader_ShowSaveGameMessage, 0x0050F860, (void* self, void* , int text, int messageAction, int configuration));
CSF_FUNC(int, __fastcall, CMenuVideoOptions_HasVideoSettingsChanged, 0x005495D0, (CMenuVideoOptions* self, void*));
CSF_FUNC(int, __fastcall, CMenuVideoOptions_UpdateOptionsMenu, 0x00549630, (CMenuVideoOptions* self, void*));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(void*, __cdecl, InitStringFromLiteral, 0x00692720, (void* returnSlot, const char* text, int count));

static void* __fastcall CMenuVideoOptions_CommitVideoOptions(
    CMenuVideoOptions* self, void* /*edx*/)
{
    void* control = reinterpret_cast<void*>(self->field_0058);
    if (control)
    {
        int selection =
            reinterpret_cast<int(__fastcall*)(void*, void*)>(
                CSF_AT(void**, control, 0)[0x2AC / 4])(control, nullptr);

        void* option = reinterpret_cast<void*>(self->field_0024);
        reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, option, 0)[0x1C / 4])(
                option, nullptr, selection);
    }

    if (static_cast<uint8_t>(
            CMenuVideoOptions_HasVideoSettingsChanged(self, nullptr)) == 0)
    {
        return reinterpret_cast<void*>(static_cast<uintptr_t>(
            CMenuVideoOptions_UpdateOptionsMenu(self, nullptr)));
    }

    self->field_007c = 1;

    // The return slot and its adjacent string storage occupy the full frame.
    alignas(8) uint8_t stringBytes[0x1C];
    CSF_AT(uint32_t, stringBytes, 0x18) = 7;
    CSF_AT(uint32_t, stringBytes, 0x14) = 0;
    CSF_AT(uint16_t, stringBytes, 0x04) = 0;

    InitStringFromLiteral(stringBytes, CSF_STR(0x00842B6C, "opt6"), 1);

    void* text;
    if (CSF_AT(uint32_t, stringBytes, 0x18) < 8)
        text = stringBytes + 0x04;
    else
        text = CSF_AT(void*, stringBytes, 0x04);

    void* globalState = reinterpret_cast<void*>(
        static_cast<uintptr_t>(GetGlobalState_50D190()));
    int result = GameBddLoader_ShowSaveGameMessage(
        globalState, nullptr,
        static_cast<int>(reinterpret_cast<uintptr_t>(text)), 1, 0);

    if (CSF_AT(uint32_t, stringBytes, 0x18) >= 8)
    {
        void* block = CSF_AT(void*, stringBytes, 0x04);
        if (block)
            return reinterpret_cast<void*>(
                static_cast<uintptr_t>(Mem_FreeDebug(block)));
        return nullptr;
    }

    return reinterpret_cast<void*>(static_cast<uintptr_t>(result));
}

HOOK(0x00549FF0, CMenuVideoOptions_CommitVideoOptions, "auto");
