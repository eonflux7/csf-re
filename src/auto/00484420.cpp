#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, sub_479D30, 0x00479D30, (void* self, void* edx));
CSF_FUNC(int, __fastcall, sub_483540, 0x00483540, (void* self, void* edx, float a2));

static C_00833908* __fastcall CCharActionFsm_SetActionMode(CCharActionFsm* self, void* /*edx*/, char a2)
{
    if (a2)
        self->vSlot1F4(0);

    C_00833908* result = reinterpret_cast<C_00833908*>(self->vSlotD8());
    uint32_t savedActionObj = self->field_009c;
    if (result)
    {
        result = reinterpret_cast<C_00833908*>(
            ((int (__fastcall*)(void*, void*))CSF_AT(void**, result, 0)[0x3C / 4])(result, nullptr));
    }

    if (savedActionObj)
    {
        int stateCode = self->field_009c
            ? ((int (__fastcall*)(void*, void*))CSF_AT(void**, reinterpret_cast<void*>(self->field_009c), 0)[0x4C / 4])(
                  reinterpret_cast<void*>(self->field_009c), nullptr)
            : 0;
        int stateIndex = stateCode - 11;

        if (stateIndex == 0)
            return nullptr;

        if (stateIndex == 1)
        {
            if (CSF_AT(uint8_t, reinterpret_cast<uint8_t*>(self->field_0008), 0x228))
                return reinterpret_cast<C_00833908*>(sub_479D30(reinterpret_cast<void*>(savedActionObj), nullptr));
            return reinterpret_cast<C_00833908*>(self->field_0008);
        }

        if (stateIndex == 2)
            return reinterpret_cast<C_00833908*>(sub_483540(self, nullptr, 0.0f));

        return reinterpret_cast<C_00833908*>(
            ((int (__fastcall*)(void*, void*))CSF_AT(void**, reinterpret_cast<void*>(savedActionObj), 0)[0x3C / 4])(
                reinterpret_cast<void*>(savedActionObj), nullptr));
    }

    return result;
}

HOOK(0x00484420, CCharActionFsm_SetActionMode, "auto");
