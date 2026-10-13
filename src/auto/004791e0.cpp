#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(C_00833b88*, __fastcall, SpatialQuery_BuildContext, 0x0040FDA0, (C_00833b88* self, void* , uint32_t* a2, uint32_t* a3));
CSF_FUNC(int, __fastcall, C_008319c0_GetSubObjectField74, 0x004760A0, (C_008319c0* self, void*));
CSF_FUNC(char, __fastcall, C_008316d8_Fire, 0x004772F0, (C_008316d8* self, void* , int action));
CSF_FUNC(float*, __fastcall, GfxMatrix_EulerAnglesToDirection, 0x006AE640, (float* self, void* , float* out));
CSF_FUNC(unsigned int, __fastcall, WeaponFirearms_ActivateContext, 0x006C8550, (void* self, void*));
CSF_FUNC(int, __fastcall, WeaponFirearms_IsContextActiveImpl, 0x006C8620, (float* self, void*));
CSF_FUNC(int, __cdecl, AppConfig_IsSubObjectFieldFlagSet, 0x00760FF0, (int fieldId));

static char __fastcall C_008317c8_ProcessFireAction(
    C_008317c8* self, void* /*edx*/, int action)
{
    void* owner = reinterpret_cast<void*>(self->field_0004);
    void* actionData = reinterpret_cast<void*>(action);

    if (CSF_AT(int, actionData, 0) == 3 &&
        static_cast<uint8_t>(AppConfig_IsSubObjectFieldFlagSet(
            C_008319c0_GetSubObjectField74(
                reinterpret_cast<C_008319c0*>(self), nullptr))) != 0)
    {
        self->vSlotF0(reinterpret_cast<float*>(
            reinterpret_cast<uint8_t*>(actionData) + 0x10));
        self->vSlotB0();

        if (self->vGetPageCountForContexts() != 0)
        {
            uint32_t context = self->vGetPageCountForContexts();
            if (static_cast<uint8_t>(WeaponFirearms_IsContextActiveImpl(
                    reinterpret_cast<float*>(context), nullptr)) == 0)
            {
                context = self->vGetPageCountForContexts();
                WeaponFirearms_ActivateContext(
                    reinterpret_cast<void*>(context), nullptr);
            }
        }

        char actionFlag = CSF_AT(uint8_t, actionData, 0x23) == 0;
        return static_cast<char>(self->vSlot7C(
            CSF_AT(int, reinterpret_cast<void*>(self->field_0040), 0x130),
            1000002u, actionFlag, 1));
    }

    if (CSF_AT(int, reinterpret_cast<void*>(self->field_0040), 0x74) == 1 &&
        ((int (__fastcall*)(void*, void*))
            self->vtbl[0x48 / 4])(self, nullptr) == 3)
    {
        alignas(8) uint8_t contextBytes[0x18];
        alignas(8) uint8_t positionBytes[0xC];
        alignas(8) uint8_t directionBytes[0xC];
        alignas(8) uint8_t anglesBytes[0x8];

        self->vSlotB0();

        ((void (__fastcall*)(void*, void*, uint32_t*))
            CSF_AT(void**, owner, 0)[0xA4 / 4])(
                owner, nullptr, reinterpret_cast<uint32_t*>(positionBytes));

        // The direction argument remains on the stack for the converter;
        // the owner's getter consumes only the angles output argument.
        float* angles = ((float* (__fastcall*)(void*, void*, float*))
            CSF_AT(void**, owner, 0)[0xAC / 4])(
                owner, nullptr, reinterpret_cast<float*>(anglesBytes));
        GfxMatrix_EulerAnglesToDirection(
            angles, nullptr, reinterpret_cast<float*>(directionBytes));

        SpatialQuery_BuildContext(
            reinterpret_cast<C_00833b88*>(contextBytes), nullptr,
            reinterpret_cast<uint32_t*>(positionBytes),
            reinterpret_cast<uint32_t*>(directionBytes));

        self->vSlotEC(reinterpret_cast<float*>(contextBytes), 0, 0);
        return static_cast<char>(self->vSlot80());
    }

    return C_008316d8_Fire(self, nullptr, action);
}

HOOK(0x004791E0, C_008317c8_ProcessFireAction, "auto");
