#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, SaveGame_DestroyLocalizedString, 0x00428E90, (void* self, void*));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __cdecl, MenuMpCharacterSelect_ClearFlag14A2, 0x00542780, ());
CSF_FUNC(int, __cdecl, MenuMpCharacterSelect_IsActive, 0x005427C0, ());
CSF_FUNC(int, __cdecl, MenuMpCharacterSelect_IsSecondaryStateActive, 0x005427F0, ());
CSF_FUNC(void, __fastcall, C_00720DF0_ForwardCallToChildVtblSlot16, 0x00720DF0,
         (void* self, void* edx, void* text));

static char __fastcall C_00830578_UpdateCharacterSelectionState(
    C_00830578* self, void* /*edx*/)
{
    const int globalState = GetGlobalState_50D190();
    if (CSF_AT(uint8_t, reinterpret_cast<void*>(globalState), 0xD0) == 0)
        return static_cast<char>(globalState);

    if (CSF_AT(uint32_t, self, 0x14C) != 0)
        MenuMpCharacterSelect_ClearFlag14A2();

    if (static_cast<uint8_t>(MenuMpCharacterSelect_IsActive()) != 0)
    {
        ((void(__fastcall*)(C_00830578*, void*))self->vtbl[0xA4 / 4])(
            self, nullptr);
        CSF_AT(uint32_t, self, 0x148) = 2;
        CSF_AT(uint8_t, self, 0x141) = 1;
        CSF_AT(uint8_t, self, 0x140) = 1;
        CSF_AT(uint8_t, self, 0x272) = 1;
        return 1;
    }

    CSF_AT(uint8_t, self, 0x272) = 0;
    if (static_cast<uint8_t>(MenuMpCharacterSelect_IsSecondaryStateActive()) != 0)
    {
        ((void(__fastcall*)(C_00830578*, void*))self->vtbl[0xA4 / 4])(
            self, nullptr);
        CSF_AT(uint32_t, self, 0x148) = 2;
        CSF_AT(uint8_t, self, 0x141) = 1;
        CSF_AT(uint8_t, self, 0x140) = 1;
        return 1;
    }

    // Preserve the contiguous storage spanning both stack-object addresses.
    alignas(8) uint8_t localizedStringBytes[0x1C];
    void* child = CSF_AT(void*, self, 0x138);
    CSF_AT(uint32_t, localizedStringBytes, 0x18) = 7;
    CSF_AT(uint32_t, localizedStringBytes, 0x14) = 0;
    CSF_AT(uint16_t, localizedStringBytes, 0x04) = 0;

    if (child != nullptr)
        C_00720DF0_ForwardCallToChildVtblSlot16(
            child, nullptr, localizedStringBytes + 0x04);

    CSF_AT(uint8_t, self, 0x141) = 0;
    CSF_AT(uint32_t, self, 0x148) = 0;
    return static_cast<char>(
        SaveGame_DestroyLocalizedString(localizedStringBytes, nullptr));
}

HOOK(0x00537650, C_00830578_UpdateCharacterSelectionState, "auto");
