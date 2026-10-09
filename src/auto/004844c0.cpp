#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, Handle_GetValuePointer, 0x00476060, (void* self, void* edx));
CSF_FUNC(int, __fastcall, sub_479C60, 0x00479C60, (void* self, void* edx));
CSF_FUNC(int, __fastcall, sub_482DC0, 0x00482DC0, (void* self, void* edx, int a2, float a3, char a4));

static C_00833908* __fastcall CCharActionFsm_ProcessCurrentItem(
    CCharActionFsm* self, void* /*edx*/)
{
    std::uintptr_t result = self->vSlotD8();
    void* item = reinterpret_cast<void*>(self->field_009c);

    if (result)
    {
        void* previousItem = reinterpret_cast<void*>(result);
        result = reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, previousItem, 0)[0x38 / 4])(
                previousItem, nullptr, 0);
    }

    if (item)
    {
        int itemState = 0;
        if (self->field_009c)
        {
            void* currentItem = reinterpret_cast<void*>(self->field_009c);
            itemState = reinterpret_cast<int(__fastcall*)(void*, void*)>(
                CSF_AT(void**, currentItem, 0)[0x4C / 4])(
                    currentItem, nullptr);
        }

        result = static_cast<std::uint32_t>(itemState) - 10;
        switch (result)
        {
        case 0:
            result = self->field_0008;
            if (!CSF_AT(std::uint8_t, reinterpret_cast<void*>(result), 0x228))
            {
                result = reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
                    CSF_AT(void**, item, 0)[0x38 / 4])(
                        item, nullptr, 1);
            }
            break;

        case 1:
            break;

        case 2:
            result = self->field_0008;
            if (CSF_AT(std::uint8_t, reinterpret_cast<void*>(result), 0x228))
                result = sub_479C60(item, nullptr);
            break;

        case 3:
        {
            void* value = reinterpret_cast<void*>(
                Handle_GetValuePointer(item, nullptr));
            int action = CSF_AT(int, value, 0x1BC);
            result = sub_482DC0(self, nullptr, action, 0.0f, 1);
            break;
        }

        default:
            result = reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, item, 0)[0x38 / 4])(
                    item, nullptr, 0);
            break;
        }
    }

    return reinterpret_cast<C_00833908*>(result);
}

HOOK(0x004844C0, CCharActionFsm_ProcessCurrentItem, "auto");
