#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(bool, __fastcall, sub_4285B0, 0x004285B0, (void* self, void* edx, int a2));
CSF_FUNC(bool, __fastcall, MenuControls_QueryFlag, 0x004285D0, (void* self, void* edx, int value));
CSF_FUNC(bool, __fastcall, sub_4285F0, 0x004285F0, (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, sub_42B230, 0x0042B230, (void* self, void* edx));
CSF_FUNC(int, __fastcall, sub_42B6B0, 0x0042B6B0, (void* self, void* edx));
CSF_FUNC(char, __fastcall, sub_42BA00, 0x0042BA00, (void* self, void* edx));
CSF_FUNC(char, __fastcall, sub_42C540, 0x0042C540, (void* self, void* edx));
CSF_FUNC(int, __fastcall, sub_44BB40, 0x0044BB40, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, GetGlobalValue_8B0568, 0x004FDBB0, ());
CSF_FUNC(void*, __fastcall, UmsVirtualProcessor_GetDefaultDestination, 0x004FE4A0, (void* self, void*));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

CSF_GLOBAL(int32_t, g_CheatSequenceFirst, 0x00830414);
CSF_GLOBAL(int32_t, g_CheatSequencePreviousBase, 0x00830410);

static char __fastcall C_00830578_UpdateCheatInput(C_00830578* self, void* /*edx*/)
{
    void* parent = reinterpret_cast<void*>(self->field_0018);
    if (parent != nullptr)
    {
        int activeScreen =
            ((int(__fastcall*)(void*, void*))CSF_AT(void**, parent, 0)[0x80 / 4])(
                parent, nullptr);
        if (activeScreen != 0)
        {
            parent = reinterpret_cast<void*>(self->field_0018);
            activeScreen =
                ((int(__fastcall*)(void*, void*))CSF_AT(void**, parent, 0)[0x80 / 4])(
                    parent, nullptr);
            if (reinterpret_cast<void*>(activeScreen) != self)
                return static_cast<char>(activeScreen);
        }
    }

    sub_42B6B0(self, nullptr);
    sub_42BA00(self, nullptr);
    sub_42B230(self, nullptr);
    sub_42C540(self, nullptr);

    int stateAddress = GetGlobalValue_8BC1E0();
    if (CSF_AT(uint8_t, reinterpret_cast<void*>(stateAddress), 0x50) == 0)
        return static_cast<char>(stateAddress);

    stateAddress = GetGlobalState_50D190();
    if (CSF_AT(uint8_t, reinterpret_cast<void*>(stateAddress), 0xD0) != 0)
        return static_cast<char>(stateAddress);

    // Inline the sequence-processing continuation at 0x42B1B0.
    int expectedInput =
        CSF_AT(int32_t, &g_CheatSequenceFirst, self->field_0028 * 4);
    if (sub_4285F0(reinterpret_cast<void*>(self->field_0014), nullptr, expectedInput))
    {
        uint32_t nextIndex = self->field_0028 + 1;
        self->field_0028 = nextIndex;
        if (CSF_AT(int32_t, &g_CheatSequenceFirst, nextIndex * 4) == -1)
        {
            int manager = GetGlobalValue_8B0568();
            void* destination = UmsVirtualProcessor_GetDefaultDestination(
                reinterpret_cast<void*>(manager), nullptr);
            return static_cast<char>(sub_44BB40(destination, nullptr, 0));
        }
        return 1;
    }

    expectedInput =
        CSF_AT(int32_t, &g_CheatSequenceFirst, self->field_0028 * 4);
    if (sub_4285B0(reinterpret_cast<void*>(self->field_0014), nullptr, expectedInput))
        return 1;

    int32_t sequenceIndex = static_cast<int32_t>(self->field_0028);
    if (sequenceIndex <= 0)
    {
        self->field_0028 = 0;
        return static_cast<char>(sequenceIndex);
    }

    int previousInput =
        CSF_AT(int32_t, &g_CheatSequencePreviousBase,
               static_cast<uint32_t>(sequenceIndex) * 4);
    bool previousInputActive = MenuControls_QueryFlag(
        reinterpret_cast<void*>(self->field_0014), nullptr, previousInput);
    if (!previousInputActive)
        self->field_0028 = 0;
    return static_cast<char>(previousInputActive);
}

HOOK(0x0042C780, C_00830578_UpdateCheatInput, "auto");
