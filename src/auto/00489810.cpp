#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __cdecl, CharActionFsm_CreateItem, 0x00475A40, (int a1, int a2, int a3));
CSF_FUNC(void, __fastcall, CharActionFsm_SetItemCount, 0x00476170, (void* self, void* edx, void* a2));
CSF_FUNC(int, __fastcall, C_Unknown_StoreField20, 0x00476180, (void* self, void* , int value));
CSF_FUNC(uint64_t, __fastcall, CharActionFsm_GetPrimaryCapacity, 0x004769F0, (void* self, void* edx));
CSF_FUNC(uint64_t, __fastcall, GetItemCapacity, 0x00476A50, (void* self, void* edx));
CSF_FUNC(int, __fastcall, FindRecordIndexById, 0x00485980, (void* self, void* edx, int a2));
CSF_FUNC(void, __fastcall, CharActionFsm_AddItem, 0x004892B0, (void* self, void* edx, void* a2));
CSF_FUNC(void, __fastcall, CharActionFsm_AddItemRecord, 0x00489470, (void* self, void* edx, int a2, void* a3));
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(int, __fastcall, CharActionFsm_GetPrimaryCount, 0x0052D5C0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GetCurrentItemCount, 0x00681F40, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, MenuManager_SetIntroPageData, 0x006F2850, (void* self, void* edx, void* data));
CSF_FUNC(int, __fastcall, CharActionFsm_GetSecondaryCount, 0x0070E390, (void* self, void* edx));
CSF_FUNC(int, __cdecl, IsSubStateEnabled, 0x00760F90, (int a1));

static void __fastcall CCharActionFsm_AddOrUpdateItem(
    CCharActionFsm* self, void* /*edx*/, int itemId, int recordAddress)
{
    bool mergeCounts = false;
    const int itemIndex = self->vSlotC0(itemId);
    void* item;

    if (itemIndex == -1)
    {
        void* manager = reinterpret_cast<void*>(GetValue8B0554());
        void* definitions = CSF_AT(void*, manager, 0x1FC0);
        void* definition =
            reinterpret_cast<void* (__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, definitions, 0)[0x8 / 4])(
                    definitions, nullptr, itemId);

        if (static_cast<uint8_t>(
                IsSubStateEnabled(CSF_AT(int, definition, 0x74))))
        {
            CharActionFsm_AddItemRecord(
                self, nullptr, itemId, CSF_AT(void*, definition, 0x104));
            return;
        }

        item = CharActionFsm_CreateItem(
            1, itemId, static_cast<int>(self->field_0008));
        CharActionFsm_AddItem(self, nullptr, item);

        if (self->field_0024 == static_cast<uint32_t>(-1))
        {
            void* lastItem = CSF_AT(
                void*, reinterpret_cast<void*>(self->field_001c - 4), 0);
            if (reinterpret_cast<bool (__fastcall*)(void*, void*)>(
                    CSF_AT(void**, lastItem, 0)[0x98 / 4])(
                        lastItem, nullptr))
            {
                const uint32_t begin = self->field_0018;
                const int count = begin
                    ? (static_cast<int>(self->field_001c - begin) >> 2)
                    : 0;
                self->vSlotF8(count - 1);
            }
        }
    }
    else
    {
        item = CSF_AT(
            void*, reinterpret_cast<void*>(self->field_0018), 4 * itemIndex);

        if (reinterpret_cast<CCharWeaponFsm*>(self->field_0008)->field_0054 == 1
            && reinterpret_cast<int (__fastcall*)(void*, void*)>(
                   CSF_AT(void**, item, 0)[0x4C / 4])(item, nullptr) == 10
            && FindRecordIndexById(self, nullptr, itemId) == -1)
        {
            mergeCounts = true;
            CharActionFsm_AddItemRecord(
                self, nullptr, itemId, reinterpret_cast<void*>(-1));

            uint32_t begin = self->field_0078;
            int count = begin
                ? (static_cast<int>(self->field_007c - begin) >> 2)
                : 0;
            CharActionFsm_SetItemCount(
                CSF_AT(void*, reinterpret_cast<void*>(self->field_0078),
                       4 * count - 4),
                nullptr, nullptr);

            begin = self->field_0078;
            count = begin
                ? (static_cast<int>(self->field_007c - begin) >> 2)
                : 0;
            C_Unknown_StoreField20(
                CSF_AT(void*, reinterpret_cast<void*>(self->field_0078),
                       4 * count - 4),
                nullptr, 0);

            begin = self->field_0078;
            count = begin
                ? (static_cast<int>(self->field_007c - begin) >> 2)
                : 0;
            MenuManager_SetIntroPageData(
                CSF_AT(void*, reinterpret_cast<void*>(self->field_0078),
                       4 * count - 4),
                nullptr, nullptr);
        }

        if (!recordAddress)
        {
            reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, item, 0)[0x60 / 4])(item, nullptr, 0);
        }
    }

    if (!item || !recordAddress)
        return;

    void* record = reinterpret_cast<void*>(recordAddress);
    if (mergeCounts)
    {
        if (!GetCurrentItemCount(item, nullptr))
        {
            CharActionFsm_SetItemCount(
                item, nullptr, CSF_AT(void*, record, 0x188));
        }

        const uint32_t primaryIncrement = CSF_AT(uint32_t, record, 0x18C);
        const int primaryCount = CharActionFsm_GetPrimaryCount(item, nullptr);
        C_Unknown_StoreField20(
            item, nullptr,
            static_cast<int>(primaryIncrement + static_cast<uint32_t>(primaryCount)));

        const uint32_t secondaryIncrement = CSF_AT(uint32_t, record, 0x190);
        const int secondaryCount = CharActionFsm_GetSecondaryCount(item, nullptr);
        MenuManager_SetIntroPageData(
            item, nullptr,
            reinterpret_cast<void*>(
                secondaryIncrement + static_cast<uint32_t>(secondaryCount)));

        const int updatedPrimaryCount =
            CharActionFsm_GetPrimaryCount(item, nullptr);
        if (updatedPrimaryCount >
            static_cast<int>(CharActionFsm_GetPrimaryCapacity(item, nullptr)))
        {
            const int capacity =
                static_cast<int>(CharActionFsm_GetPrimaryCapacity(item, nullptr));
            C_Unknown_StoreField20(item, nullptr, capacity);
        }

        const int updatedSecondaryCount =
            CharActionFsm_GetSecondaryCount(item, nullptr);
        if (updatedSecondaryCount >
            static_cast<int>(GetItemCapacity(item, nullptr)))
        {
            const uint32_t capacity =
                static_cast<uint32_t>(GetItemCapacity(item, nullptr));
            MenuManager_SetIntroPageData(
                item, nullptr, reinterpret_cast<void*>(capacity));
        }
    }
    else if (itemIndex == -1)
    {
        C_Unknown_StoreField20(
            item, nullptr, CSF_AT(int, record, 0x18C));
        CharActionFsm_SetItemCount(
            item, nullptr, CSF_AT(void*, record, 0x188));
        MenuManager_SetIntroPageData(
            item, nullptr, CSF_AT(void*, record, 0x190));
    }
    else
    {
        reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, item, 0)[0x60 / 4])(
                item, nullptr, recordAddress);
    }
}

HOOK(0x00489810, CCharActionFsm_AddOrUpdateItem, "auto");
