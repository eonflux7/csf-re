#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, Handle_GetValuePointer, 0x00476060, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CCharWeaponFsm_RequestStateChange, 0x00480970, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, C_004950e0_GetField398, 0x004950E0, (void* self, void*));
CSF_FUNC(void, __fastcall, CharActionFsm_ApplyActionParameters, 0x004A11A0, (void* self, void* edx, uint32_t* a2, int a3, float a4));

static int __fastcall CCharActionFsm_RequestStateChange(
    CCharActionFsm* self, void* /*edx*/, int requestedState)
{
    int actionType = 0;
    if (self->field_009c)
    {
        void* action = reinterpret_cast<void*>(self->field_009c);
        actionType =
            reinterpret_cast<int (__fastcall*)(void*, void*)>(
                CSF_AT(void**, action, 0)[0x4C / 4])(action, nullptr);
    }

    int result;
    if (self->field_0024 != static_cast<uint32_t>(requestedState) &&
        (actionType == 10 || actionType == 11))
    {
        self->field_0028 = requestedState;
        result = self->vSlot1E8();
    }
    else
    {
        result = CCharWeaponFsm_RequestStateChange(self, nullptr, requestedState);
    }

    if (self->field_0024 != static_cast<uint32_t>(-1))
    {
        void* handle = reinterpret_cast<void*>(self->vSlotD8());
        void* value = reinterpret_cast<void*>(
            Handle_GetValuePointer(handle, nullptr));

        const float firstParameter = CSF_AT(float, value, 0x1C4);
        const float lastParameter = CSF_AT(float, value, 0x1CC);
        const int middleParameter = CSF_AT(int, value, 0x1C8);

        uint32_t parameters[2];
        std::memcpy(&parameters[0], &firstParameter, sizeof(firstParameter));
        std::memcpy(&parameters[1], &firstParameter, sizeof(firstParameter));

        CharActionFsm_ApplyActionParameters(
            reinterpret_cast<uint8_t*>(self->field_0008) + 0x344,
            nullptr, parameters, middleParameter, lastParameter);

        result = C_004950e0_GetField398(
            reinterpret_cast<void*>(self->field_0008), nullptr);
        if (result)
            return self->vSlot94(0);
    }

    return result;
}

HOOK(0x00484620, CCharActionFsm_RequestStateChange, "auto");
