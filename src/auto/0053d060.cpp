#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __fastcall, GameBddLoader_ShowSaveGameMessage, 0x0050F860, (void* self, void* , int text, int messageAction, int configuration));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(void*, __cdecl, InitStringFromLiteral, 0x00692720, (void* returnSlot, const char* text, int count));

static void* __fastcall CMenuSoundOptions_ApplyChangesAndReturn(
    CMenuSoundOptions* self, void* /*edx*/)
{
    self->vApplyAcousticSelection();
    self->vApplyToggleSelection();
    ((void(__fastcall*)(void*, void*))self->vtbl[0x98 / 4])(self, nullptr);

    void* control = reinterpret_cast<void*>(self->field_0024);
    if (!control ||
        !((bool(__fastcall*)(void*, void*))
              CSF_AT(void**, control, 0)[0x6C / 4])(control, nullptr))
    {
        return reinterpret_cast<void*>(self->vReturnToOptions());
    }

    control = reinterpret_cast<void*>(self->field_0024);
    const int selection =
        ((int(__fastcall*)(void*, void*))
             CSF_AT(void**, control, 0)[0x2AC / 4])(control, nullptr);

    if (self->field_003c == (selection == 0))
        return reinterpret_cast<void*>(self->vReturnToOptions());

    self->field_0058 = 1;

    alignas(8) uint8_t stringBytes[0x1C];
    CSF_AT(uint32_t, stringBytes, 0x18) = 7;
    CSF_AT(uint32_t, stringBytes, 0x14) = 0;
    CSF_AT(uint16_t, stringBytes, 0x04) = 0;

    InitStringFromLiteral(stringBytes, CSF_STR(0x00842B6C, "opt6"), 1);

    void* text = CSF_AT(void*, stringBytes, 0x04);
    if (CSF_AT(uint32_t, stringBytes, 0x18) < 8)
        text = stringBytes + 0x04;

    void* state = reinterpret_cast<void*>(GetGlobalState_50D190());
    void* result = reinterpret_cast<void*>(
        GameBddLoader_ShowSaveGameMessage(
            state, nullptr, reinterpret_cast<int>(text), 1, 0));

    if (CSF_AT(uint32_t, stringBytes, 0x18) >= 8)
    {
        result = CSF_AT(void*, stringBytes, 0x04);
        if (result)
            return reinterpret_cast<void*>(Mem_FreeDebug(result));
    }

    return result;
}

HOOK(0x0053D060, CMenuSoundOptions_ApplyChangesAndReturn, "auto");
