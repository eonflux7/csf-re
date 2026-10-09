#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, C_008319c0_GetSubObjectField74, 0x004760A0, (C_008319c0* self, void*));
CSF_FUNC(char, __fastcall, CCharWeaponFsm_CheckFirstActionCondition, 0x004832A0, (void* self, void* edx));
CSF_FUNC(char, __fastcall, CCharWeaponFsm_CheckSecondActionCondition, 0x00483300, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CCharActionFsm_CanRequestPrimaryItemChange, 0x00485A70, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CCharActionFsm_CanRequestSecondaryItemChange, 0x00485AC0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GetCurrentItemCount, 0x00681F40, (void* self, void* edx));
CSF_FUNC(int, __cdecl, CharActionFsm_CheckItemChangeEligibility, 0x00760FF0, (int a1));

static char __fastcall CCharActionFsm_HandleEmptyItems(
    CCharActionFsm* self, void* /*edx*/, int /*a2*/)
{
    char result = CCharWeaponFsm_CheckSecondActionCondition(self, nullptr);
    if (result)
        return result;

    result = CCharWeaponFsm_CheckFirstActionCondition(self, nullptr);
    if (result)
        return result;

    if (self->field_0024 != static_cast<uint32_t>(-1))
    {
        void* item = reinterpret_cast<void*>(self->vSlotD8());
        if (((char (__fastcall*)(void*, void*))
                 CSF_AT(void**, item, 0)[0x9C / 4])(item, nullptr))
        {
            item = reinterpret_cast<void*>(self->vSlotD8());
            if (GetCurrentItemCount(item, nullptr) == 0)
            {
                if (self->vSlot108())
                {
                    void* controller =
                        reinterpret_cast<uint8_t*>(self->field_0008) + 4;
                    ((void (__fastcall*)(void*, void*))
                         CSF_AT(void**, controller, 0)[0x70 / 4])(
                             controller, nullptr);
                }
                else if (self->field_0028 == static_cast<uint32_t>(-1))
                {
                    auto* currentItem =
                        reinterpret_cast<C_008319c0*>(self->vSlotD8());
                    int subObjectField =
                        C_008319c0_GetSubObjectField74(currentItem, nullptr);
                    if (static_cast<uint8_t>(
                            CharActionFsm_CheckItemChangeEligibility(subObjectField)))
                    {
                        if (CCharActionFsm_CanRequestPrimaryItemChange(self, nullptr))
                        {
                            void* controller =
                                reinterpret_cast<uint8_t*>(self->field_0008) + 4;
                            int request[2] = {1, -1};
                            ((void (__fastcall*)(void*, void*, int*))
                                 CSF_AT(void**, controller, 0)[0x68 / 4])(
                                     controller, nullptr, request);
                        }
                    }
                }
            }
        }
    }

    uint32_t itemIndex = self->field_0090;
    if (itemIndex == static_cast<uint32_t>(-1))
        return static_cast<char>(itemIndex);

    void* item = reinterpret_cast<void**>(self->field_0078)[itemIndex];
    result = ((char (__fastcall*)(void*, void*))
                  CSF_AT(void**, item, 0)[0x9C / 4])(item, nullptr);
    if (!result)
        return result;

    itemIndex = self->field_0090;
    item = nullptr;
    if (itemIndex != static_cast<uint32_t>(-1))
        item = reinterpret_cast<void**>(self->field_0078)[itemIndex];

    int itemCount = GetCurrentItemCount(item, nullptr);
    if (itemCount != 0 || self->field_0094 != static_cast<uint32_t>(-1))
        return static_cast<char>(itemCount);

    int canChange = CCharActionFsm_CanRequestSecondaryItemChange(self, nullptr);
    if (canChange)
    {
        void* controller =
            reinterpret_cast<uint8_t*>(self->field_0008) + 4;
        int request[2] = {1, -1};
        return static_cast<char>(
            ((int (__fastcall*)(void*, void*, int*))
                 CSF_AT(void**, controller, 0)[0x6C / 4])(
                     controller, nullptr, request));
    }

    self->field_0090 = static_cast<uint32_t>(-1);
    return static_cast<char>(canChange);
}

HOOK(0x00486780, CCharActionFsm_HandleEmptyItems, "auto");
