#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CCharWeaponFsm_FinishReset, 0x0047F930,
         (void* self, void* edx));
CSF_FUNC(int, __fastcall, CCharWeaponFsm_SetActionTime, 0x00483540,
         (void* self, void* edx, float a2));
CSF_FUNC(void, __fastcall, CCharActionFsm_ResetWeaponState, 0x004846D0,
         (void* self, void* edx));
CSF_FUNC(char, __fastcall, CharActionFsm_ResetController, 0x0048D770,
         (void* self, void* edx));

static int __fastcall CCharActionFsm_Reset(CCharActionFsm* self, void* /*edx*/)
{
    self->field_0094 = -1;
    self->field_0098 = -1;
    self->field_00a8 = 0;
    self->field_00ac = 0;
    self->field_00b4 = -1;
    self->field_00b8 = -1;
    self->field_00a0 = 0;
    self->field_00a4 = 0;
    self->field_00bc = 0;
    self->field_00c0 = 0;
    self->field_00c4 = 0;
    self->field_00d0 = -1.0f;
    self->field_00cc = -1.0f;

    CCharActionFsm_ResetWeaponState(self, nullptr);

    const uint32_t stateIndex = self->field_0090;
    if (stateIndex != static_cast<uint32_t>(-1))
    {
        void* state = reinterpret_cast<void**>(self->field_0078)[stateIndex];
        reinterpret_cast<void (__fastcall*)(void*, void*)>(
            CSF_AT(void**, state, 0)[0x24 / 4])(state, nullptr);
    }

    CCharWeaponFsm_SetActionTime(self, nullptr, 0.0f);
    CharActionFsm_ResetController(
        reinterpret_cast<void*>(self->field_00d4), nullptr);
    return CCharWeaponFsm_FinishReset(self, nullptr);
}

HOOK(0x004866E0, CCharActionFsm_Reset, "auto");
