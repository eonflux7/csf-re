#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, CharActionFsm_SetCurrentItemCount, 0x00476170,
         (void* self, void* edx, void* a2));
CSF_FUNC(int, __fastcall, CharActionFsm_GetItemIdentifier, 0x00479310,
         (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, CharActionFsm_InitializeActionParameters, 0x00485010,
         (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, GetCurrentItemCount, 0x00681F40,
         (void* self, void* edx));

static void __fastcall CCharActionFsm_ConsumeCurrentItem(
    CCharActionFsm* self, void* /*edx*/)
{
    void* action = reinterpret_cast<void*>(self->field_009c);
    if (!action)
        return;

    const int actionType =
        reinterpret_cast<int(__fastcall*)(void*, void*)>(
            CSF_AT(void**, action, 0)[0x4C / 4])(action, nullptr);

    if (actionType == 11)
    {
        void* currentItem = reinterpret_cast<void*>(self->vGetIndexedEntryOrZero());
        const int identifier =
            CharActionFsm_GetItemIdentifier(currentItem, nullptr);
        void* item = reinterpret_cast<void*>(self->vLookupEntryByIndexOrZero(identifier));

        if (item && GetCurrentItemCount(item, nullptr) > 0)
        {
            const int remaining = GetCurrentItemCount(item, nullptr) - 1;
            CharActionFsm_SetCurrentItemCount(
                item, nullptr, reinterpret_cast<void*>(remaining));
        }
    }
    else if (actionType == 10)
    {
        uint32_t parameters[0x28 / sizeof(uint32_t)] = {};
        CharActionFsm_InitializeActionParameters(
            parameters, nullptr,
            static_cast<int>(reinterpret_cast<uintptr_t>(&self->field_003c)));

        action = reinterpret_cast<void*>(self->field_009c);
        CSF_AT(uint8_t, parameters, 0x21) = 1;
        CSF_AT(uint8_t, parameters, 0x23) = 1;

        reinterpret_cast<void(__fastcall*)(void*, void*, uint32_t*)>(
            CSF_AT(void**, action, 0)[0x40 / 4])(
                action, nullptr, parameters);

        void* currentItem = reinterpret_cast<void*>(self->vGetIndexedEntryOrZero());
        if (GetCurrentItemCount(currentItem, nullptr) > 1)
        {
            currentItem = reinterpret_cast<void*>(self->vGetIndexedEntryOrZero());
            const int remaining =
                GetCurrentItemCount(currentItem, nullptr) - 1;
            currentItem = reinterpret_cast<void*>(self->vGetIndexedEntryOrZero());
            CharActionFsm_SetCurrentItemCount(
                currentItem, nullptr, reinterpret_cast<void*>(remaining));
        }
    }
}

HOOK(0x00485870, CCharActionFsm_ConsumeCurrentItem, "auto");
