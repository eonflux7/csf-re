#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static int __fastcall CharActionFsm_NotifyLinkedObjectPositionAlt(
    C_008342e8** self, void* /*edx*/)
{
    C_008342e8* owner = CSF_AT(C_008342e8*, self, 0x8);
    auto* link = reinterpret_cast<std::uint8_t*>(owner) + 0x2B0;

    if (CSF_AT(int, owner, 0x2BC) < 0)
        return static_cast<int>(reinterpret_cast<std::uintptr_t>(link));

    int resolved = CHandleTable_Resolve(
        g_pCurrentScene, nullptr,
        static_cast<int>(reinterpret_cast<std::uintptr_t>(link + 4)));
    if (!resolved)
        return resolved;

    owner = CSF_AT(C_008342e8*, self, 0x8);

    alignas(4) std::uint8_t notification[0x24]{};
    CSF_AT(int, notification, 0x04) = -1;
    CSF_AT(int, notification, 0x08) = -1;
    CSF_AT(std::uint8_t, notification, 0x0C) = 0;
    CSF_AT(void**, notification, 0x00) = CSF_VTBL(0x0082D448);
    CSF_AT(int, notification, 0x10) = 0;
    CSF_AT(int, notification, 0x14) = 0;
    CSF_AT(int, notification, 0x18) = 0;
    CSF_AT(int, notification, 0x20) = 0;
    CSF_AT(float, notification, 0x1C) = -1.0f;

    CSF_AT(int, notification, 0x04) = owner->field_0020;
    CSF_AT(int, notification, 0x08) = owner->field_0024;
    CSF_AT(std::uint8_t, notification, 0x0C) = 1;

    const int* position = reinterpret_cast<const int*>(
        GetObjectPositionVector(owner, nullptr));
    CSF_AT(int, notification, 0x10) = position[0];
    CSF_AT(int, notification, 0x14) = position[1];
    CSF_AT(int, notification, 0x18) = position[2];

    auto* receiver = reinterpret_cast<std::uint8_t*>(resolved) + 4;
    void** receiverVtable = CSF_AT(void**, receiver, 0);
    CSF_AT(int, notification, 0x20) = 2;

    return reinterpret_cast<int (__fastcall*)(void*, void*, void*)>(
        receiverVtable[0x44 / 4])(receiver, nullptr, notification);
}

HOOK(0x004863B0, CharActionFsm_NotifyLinkedObjectPositionAlt, "auto");
