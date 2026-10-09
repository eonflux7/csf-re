#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, CharActionFsm_CanSelectItem, 0x0047A1E0, (void* self, void* edx, void* a2));

static void __fastcall CCharActionFsm_SelectItem(
    CCharActionFsm* self, void* /*edx*/, uint32_t* request)
{
    const uint32_t currentIndex = self->field_0090;
    if (currentIndex == UINT32_MAX)
        return;

    const uint32_t initialItems = self->field_0078;
    if (!reinterpret_cast<void**>(initialItems)[currentIndex])
        return;

    const uint32_t mode = request[0];
    uint32_t selectedIndex;

    if (mode == 0)
    {
        selectedIndex = static_cast<uint32_t>(
            self->vSlotBC(static_cast<int32_t>(request[1])));
        if (selectedIndex == UINT32_MAX)
            return;

        void* item = reinterpret_cast<void**>(self->field_0078)[selectedIndex];
        if (!((bool(__fastcall*)(void*, void*))
                  CSF_AT(void**, item, 0)[0x98 / 4])(item, nullptr))
            return;

        void** itemSlot =
            reinterpret_cast<void**>(self->field_0078) + selectedIndex;
        void* context = reinterpret_cast<void*>(self->vSlotD8());
        if (!CharActionFsm_CanSelectItem(*itemSlot, nullptr, context))
            return;
    }
    else if (mode == 1 || mode == 2)
    {
        const uint32_t itemsBegin =
            mode == 1 ? self->field_0078 : initialItems;
        const uint32_t count = itemsBegin
            ? static_cast<uint32_t>(
                  static_cast<int32_t>(self->field_007c - itemsBegin) >> 2)
            : 0;

        const uint32_t pendingIndex = self->field_0094;
        const uint32_t origin =
            pendingIndex == UINT32_MAX ? currentIndex : pendingIndex;
        const uint32_t nextIndex =
            mode == 1 ? origin + 1 : origin + count - 1;

        selectedIndex = static_cast<uint32_t>(
            static_cast<int32_t>(nextIndex) % static_cast<int32_t>(count));

        bool accepted = false;
        void* item = reinterpret_cast<void**>(initialItems)[selectedIndex];
        if (((bool(__fastcall*)(void*, void*))
                 CSF_AT(void**, item, 0)[0x98 / 4])(item, nullptr))
        {
            void** itemSlot =
                reinterpret_cast<void**>(self->field_0078) + selectedIndex;
            void* context = reinterpret_cast<void*>(self->vSlotD8());
            accepted =
                CharActionFsm_CanSelectItem(*itemSlot, nullptr, context) != 0;
        }

        if (!accepted)
        {
            uint32_t candidate = mode == 1
                ? (selectedIndex + 1) % count
                : (count + selectedIndex - 1) % count;

            for (;;)
            {
                if (candidate == selectedIndex)
                    return;

                item = reinterpret_cast<void**>(self->field_0078)[candidate];
                if (((bool(__fastcall*)(void*, void*))
                         CSF_AT(void**, item, 0)[0x98 / 4])(item, nullptr))
                {
                    void** itemSlot =
                        reinterpret_cast<void**>(self->field_0078) + candidate;
                    void* context = reinterpret_cast<void*>(self->vSlotD8());
                    if (CharActionFsm_CanSelectItem(*itemSlot, nullptr, context))
                        break;
                }

                candidate = mode == 1
                    ? (candidate + 1) % count
                    : (candidate - 1) % count;
            }

            selectedIndex = candidate;
        }
    }
    else
    {
        return;
    }

    if (selectedIndex != UINT32_MAX && selectedIndex != self->field_0094)
        self->field_0094 = selectedIndex;
}

HOOK(0x00485F10, CCharActionFsm_SelectItem, "auto");
