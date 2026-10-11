#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(bool, __fastcall, sub_4285B0, 0x004285B0, (void* self, void* edx, int a2));
CSF_FUNC(bool, __fastcall, MenuControls_QueryFlag, 0x004285D0, (void* self, void* edx, int value));
CSF_FUNC(bool, __fastcall, sub_4285F0, 0x004285F0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, sub_44BB40, 0x0044BB40, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, GetGlobalValue_8B0568, 0x004FDBB0, ());
CSF_FUNC(void*, __fastcall, UmsVirtualProcessor_GetDefaultDestination, 0x004FE4A0, (void* self, void*));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

static char __fastcall C_00830578_ProcessCheatSequenceTick(
    C_00830578* self, void* /*edx*/)
{
    if (sub_4285B0(reinterpret_cast<void*>(self->field_0014), nullptr, 92))
    {
        reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, self, 0)[0x64 / 4])(self, nullptr, 1);
    }

    if (sub_4285B0(reinterpret_cast<void*>(self->field_0014), nullptr, 36))
        self->vRunActionChecked();

    if (sub_4285B0(reinterpret_cast<void*>(self->field_0014), nullptr, 37))
        self->vCanEnterMenuState();

    int globalValue = GetGlobalValue_8BC1E0();
    if (CSF_AT(uint8_t, reinterpret_cast<void*>(globalValue), 0x50) == 0)
        return static_cast<char>(globalValue);

    int globalState = GetGlobalState_50D190();
    if (CSF_AT(uint8_t, reinterpret_cast<void*>(globalState), 0xD0) != 0)
        return static_cast<char>(globalState);

    // The original tail-jumps into this sequence-processing block.
    const int32_t* sequence =
        reinterpret_cast<const int32_t*>(CSF_ADDR_DATA(0x00830414));
    const int32_t* precedingActions =
        reinterpret_cast<const int32_t*>(CSF_ADDR_DATA(0x00830410));

    bool advanced = sub_4285F0(
        reinterpret_cast<void*>(self->field_0014), nullptr,
        sequence[self->field_0028]);

    if (advanced)
    {
        uint32_t nextIndex = self->field_0028 + 1;
        self->field_0028 = nextIndex;

        if (sequence[nextIndex] == -1)
        {
            int manager = GetGlobalValue_8B0568();
            void* destination = UmsVirtualProcessor_GetDefaultDestination(
                reinterpret_cast<void*>(manager), nullptr);
            return static_cast<char>(sub_44BB40(destination, nullptr, 0));
        }

        return static_cast<char>(advanced);
    }

    bool active = sub_4285B0(
        reinterpret_cast<void*>(self->field_0014), nullptr,
        sequence[self->field_0028]);

    if (active)
        return static_cast<char>(active);

    int32_t index = static_cast<int32_t>(self->field_0028);
    if (index <= 0)
    {
        self->field_0028 = 0;
        return static_cast<char>(index);
    }

    bool previousActionHeld = MenuControls_QueryFlag(
        reinterpret_cast<void*>(self->field_0014), nullptr,
        precedingActions[static_cast<uint32_t>(index)]);

    if (!previousActionHeld)
        self->field_0028 = 0;

    return static_cast<char>(previousActionHeld);
}

HOOK(0x0042B640, C_00830578_ProcessCheatSequenceTick, "auto");
