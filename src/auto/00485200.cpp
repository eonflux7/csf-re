#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(uint32_t*, __fastcall, C_008342e8_SetHandle, 0x00494DC0, (void* self, void* edx, int a2));

static int __fastcall CCharWeaponFsm_ClearHandleAndNotifyOwner(
    CCharWeaponFsm* self, void* /*edx*/)
{
    struct Handle {
        void** vtbl;
        int index;
        int generation;
    };
    struct Notification {
        Handle source;
        int action;
        int priority;
        int parameters[6];
    };
    static_assert(sizeof(Handle) == 0xC);
    static_assert(sizeof(Notification) == 0x2C);

    void* resolved = reinterpret_cast<void*>(CHandleTable_Resolve(
        g_pCurrentScene, nullptr,
        reinterpret_cast<int>(
            reinterpret_cast<uint8_t*>(self->field_0008) + 0x2D4)));

    Handle emptyHandle{CSF_VTBL(0x0082D448), -1, -1};
    C_008342e8_SetHandle(
        reinterpret_cast<void*>(self->field_0008), nullptr,
        reinterpret_cast<int>(&emptyHandle));

    self->vUpdateWeaponSelection(35);

    if (!resolved)
        return 0;

    auto* owner = reinterpret_cast<C_00833908*>(self->field_0008);
    Notification notification{
        {CSF_VTBL(0x0082D448), -1, -1},
        -1, 0, {0, 0, 0, 0, 0, 0}
    };
    notification.source.index = static_cast<int>(owner->field_0020);
    notification.source.generation = static_cast<int>(owner->field_0024);

    void* recipient = CSF_AT(void*, resolved, 0x38);
    notification.action = 0;
    notification.priority = 0x7FFFFFFF;
    notification.parameters[0] = 0;

    return reinterpret_cast<int (__fastcall*)(void*, void*, Notification*)>(
        CSF_AT(void**, recipient, 0)[0x8C / 4])(
            recipient, nullptr, &notification);
}

HOOK(0x00485200, CCharWeaponFsm_ClearHandleAndNotifyOwner, "auto");
