#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static int __fastcall CCharWeaponFsm_ResetWeaponAction(
    CCharWeaponFsm* self, void* /*edx*/)
{
    self->vSetCurrentWeaponAction(static_cast<int>(self->field_0028));
    self->field_0028 = 0xFFFFFFFF;
    self->vUpdateWeaponSelection(13);

    auto* owner = reinterpret_cast<std::uint8_t*>(self->field_0008);
    if (CSF_AT(int, owner, 0x2BC) < 0)
        return self->vBeginWeaponStateChange(0, 0);

    const int resolved = CHandleTable_Resolve(
        g_pCurrentScene, nullptr,
        reinterpret_cast<int>(owner + 0x2B4));
    if (!resolved)
        return resolved;

    owner = reinterpret_cast<std::uint8_t*>(self->field_0008);

    alignas(4) std::uint8_t notification[0x10] = {};
    CSF_AT(int, notification, 0x04) = -1;
    CSF_AT(int, notification, 0x08) = -1;
    CSF_AT(void**, notification, 0x00) = CSF_VTBL(0x0082D448);
    CSF_AT(std::uint32_t, notification, 0x04) =
        CSF_AT(std::uint32_t, owner, 0x20);

    auto* receiver = reinterpret_cast<std::uint8_t*>(resolved) + 4;
    void** receiverVtable = CSF_AT(void**, receiver, 0);
    CSF_AT(std::uint32_t, notification, 0x08) =
        CSF_AT(std::uint32_t, owner, 0x24);
    CSF_AT(char, notification, 0x0C) = 1;

    reinterpret_cast<void (__fastcall*)(void*, void*, void*)>(
        receiverVtable[0x3C / 4])(receiver, nullptr, notification);

    return self->vBeginWeaponStateChange(0, 0);
}

HOOK(0x00485360, CCharWeaponFsm_ResetWeaponAction, "auto");
