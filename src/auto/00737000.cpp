#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Sets the active item of C_00867260 to a2. Notifies the owner about the new item
// (vtable slot 33), resets the item's scale and offset, and runs the item's
// initialization (this vtable slot 14). If a previous item existed, it is notified
// (vtable slot 34). Returns a2 when a2 is 0, otherwise the result of the last notification.
static int __fastcall C_00867260_SetActiveItem(C_00867260* self, void* /*edx*/, int a2)
{
    int result = a2;
    uint32_t previousItem = CSF_AT(uint32_t, self, 0x18);

    CSF_AT(uint8_t, self, 0x20) = 1;
    CSF_AT(uint32_t, self, 0x18) = static_cast<uint32_t>(a2);

    if (a2 != 0)
    {
        using OwnerNotifyFn = void(__fastcall*)(void*, void*, int);
        void* owner = CSF_AT(void*, self, 0x0c);
        ((OwnerNotifyFn)CSF_AT(void**, owner, 0)[33])(owner, nullptr, a2);

        void* item = CSF_AT(void*, self, 0x18);
        void* itemVtable = CSF_AT(void*, item, 0);
        CSF_AT(uint32_t, self, 0x7c) = CSF_AT(uint32_t, itemVtable, 12);
        uint32_t itemField2 = CSF_AT(uint32_t, itemVtable, 16);

        CSF_AT(float, self, 0x60) = 1.0f;
        CSF_AT(float, self, 0x64) = 1.0f;
        CSF_AT(uint32_t, self, 0x80) = itemField2;
        CSF_AT(float, self, 0x58) = 0.0f;
        CSF_AT(float, self, 0x5c) = 0.0f;

        using InitItemFn = int(__fastcall*)(void*, void*, void*);
        result = ((InitItemFn)CSF_AT(void**, self, 0)[14])(
            self, nullptr, reinterpret_cast<uint8_t*>(self) + 0x7c);
    }

    if (previousItem != 0)
    {
        using OwnerReleaseFn = int(__fastcall*)(void*, void*, uint32_t);
        void* owner = CSF_AT(void*, self, 0x0c);
        return ((OwnerReleaseFn)CSF_AT(void**, owner, 0)[34])(owner, nullptr, previousItem);
    }

    return result;
}

HOOK(0x00737000, C_00867260_SetActiveItem, "auto");
