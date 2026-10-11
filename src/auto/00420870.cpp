#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, HasNonzeroLinkedValueDifferentFrom, 0x0048E680, (void* self, void* , int comparisonValue));
CSF_FUNC(int, __fastcall, TCharTask_BaseUpdate, 0x004DB140, (void* self, void* edx, int a2));

// Per-frame tick for C_0082f9f8: refreshes the owner's animation state when this task is idle
// and not blocked by a linked task, notifies the linked object, then runs the base TCharTask update.
static int __fastcall C_0082f9f8_TickStateAndDispatch(C_0082f9f8* self, void* /*edx*/)
{
    if (!self->field_0230 &&
        !HasNonzeroLinkedValueDifferentFrom(reinterpret_cast<void*>(self->field_0014), nullptr,
                                            static_cast<int>(reinterpret_cast<uintptr_t>(self))))
    {
        void* owner = reinterpret_cast<void*>(self->field_0008);
        void* ownerCtrl = CSF_AT(void*, owner, 0x14);
        void* ctrlSub = CSF_AT(void*, ownerCtrl, 0x48);

        bool canRefresh = reinterpret_cast<bool(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, ctrlSub, 0)[9])(ctrlSub, nullptr, 1);
        if (canRefresh)
        {
            reinterpret_cast<void(__fastcall*)(void*, void*, int, int, int, int)>(
                CSF_AT(void**, ctrlSub, 0)[0] == nullptr ? nullptr : CSF_AT(void**, ownerCtrl, 0)[35])(
                ownerCtrl, nullptr, 0, 0, -1, 0);
        }
    }

    void* linked = reinterpret_cast<void*>(self->field_0014);
    void* linkedSub = CSF_AT(void*, linked, 0x48);
    if (linkedSub)
    {
        reinterpret_cast<void(__fastcall*)(void*, void*, C_0082f9f8*)>(
            CSF_AT(void**, linkedSub, 0)[8])(linkedSub, nullptr, self);
    }

    return TCharTask_BaseUpdate(self, nullptr,
                                static_cast<int>(reinterpret_cast<uintptr_t>(
                                    reinterpret_cast<uint8_t*>(self) + 0x20)));
}

HOOK(0x420870, C_0082f9f8_TickStateAndDispatch, "auto");
