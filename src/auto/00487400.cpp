#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, C_008319c0_GetSubObjectField74, 0x004760A0, (C_008319c0* self, void*));
CSF_FUNC(int, __fastcall, sub_479310, 0x00479310, (void* self, void* edx));
CSF_FUNC(int, __fastcall, sub_487260, 0x00487260, (void* self, void* edx));
CSF_FUNC(char, __fastcall, Hud_IsSuspended, 0x00495520, (void* self, void* edx));
CSF_FUNC(int, __fastcall, sub_496DC0, 0x00496DC0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GetCurrentItemCount, 0x00681F40, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));
CSF_FUNC(int, __cdecl, sub_7610E0, 0x007610E0, (int a1));
CSF_FUNC(int, __cdecl, sub_761120, 0x00761120, (int a1));

static char __fastcall CCharActionFsm_CanUseItem(
    CCharActionFsm* self, void* /*edx*/, uint32_t* item)
{
    if (!item)
        return 0;

    if (!static_cast<uint8_t>(self->vIsField000cNull()) &&
        self->field_000c != 4)
        return 0;

    if (!reinterpret_cast<bool(__fastcall*)(void*, void*)>(
            CSF_AT(void**, item, 0)[0x2C / 4])(item, nullptr) &&
        C_008319c0_GetSubObjectField74(
            reinterpret_cast<C_008319c0*>(item), nullptr) != 28)
        return 0;

    auto* owner = reinterpret_cast<C_00833908*>(self->field_0008);
    if ((CSF_AT(uint32_t, owner, 0x2E0) & 0x40) != 0)
    {
        if (!Hud_IsSuspended(owner, nullptr) ||
            item != reinterpret_cast<uint32_t*>(self->field_0084))
            return 0;
    }

    owner = reinterpret_cast<C_00833908*>(self->field_0008);
    if ((CSF_AT(uint32_t, owner, 0x2E0) & 0x10000) != 0)
    {
        int allowed;
        if (CSF_AT(uint8_t, owner, 0x3D0))
        {
            const int itemType = C_008319c0_GetSubObjectField74(
                reinterpret_cast<C_008319c0*>(item), nullptr);
            allowed = sub_761120(itemType);
        }
        else
        {
            const int itemType = C_008319c0_GetSubObjectField74(
                reinterpret_cast<C_008319c0*>(item), nullptr);
            allowed = sub_7610E0(itemType);
        }
        if (!static_cast<uint8_t>(allowed))
            return 0;
    }

    void* currentItem = reinterpret_cast<void*>(self->vSlotDC());
    const int category = reinterpret_cast<int(__fastcall*)(void*, void*)>(
        CSF_AT(void**, item, 0)[0x4C / 4])(item, nullptr);

    switch (category)
    {
    case 3:
        return GetCurrentItemCount(item, nullptr) > 0;

    case 9:
        if (!currentItem)
            return 0;
        if (static_cast<uint8_t>(sub_496DC0(
                reinterpret_cast<void*>(self->field_0008), nullptr)))
            return 1;
        return (CSF_AT(uint32_t,
                    reinterpret_cast<void*>(self->field_0008), 0x2E0) &
                0x800) == 0;

    case 10:
    {
        if (!currentItem)
            return 0;
        void* provider = GetCachedObjectProvider(currentItem, nullptr);
        return GetCachedObjectProvider(item, nullptr) == provider;
    }

    case 11:
    {
        const int itemId = sub_479310(item, nullptr);
        void* matchingItem = reinterpret_cast<void*>(self->vSlotEC(itemId));
        if (!matchingItem)
            return 0;
        return GetCurrentItemCount(matchingItem, nullptr) > 0;
    }

    case 12:
    case 13:
        return 1;

    case 14:
    {
        if (GetCurrentItemCount(item, nullptr) <= 0)
            return 0;
        auto* character = reinterpret_cast<C_00833908*>(self->field_0008);
        void* capacityObject = reinterpret_cast<void*>(character->field_0038);
        const int count = reinterpret_cast<int(__fastcall*)(void*, void*)>(
            CSF_AT(void**, capacityObject, 0)[0xA0 / 4])(
                capacityObject, nullptr);
        const int capacity = reinterpret_cast<int(__fastcall*)(void*, void*)>(
            CSF_AT(void**, capacityObject, 0)[0x84 / 4])(
                capacityObject, nullptr);
        return count < capacity;
    }

    case 15:
        return sub_487260(self, nullptr) != 0;

    default:
        return 0;
    }
}

HOOK(0x00487400, CCharActionFsm_CanUseItem, "auto");
