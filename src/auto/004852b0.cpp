#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_521BB0, 0x00521BB0, ());
CSF_FUNC(int, __fastcall, ApplyGlobalValueRequest, 0x00523940, (void* self, void* edx, void* a2, float a3));

static int __fastcall CCharWeaponFsm_RequestWeaponAction(
    CCharWeaponFsm* self, void* /*edx*/)
{
    self->vUpdateWeaponSelection(46);

    auto* owner = reinterpret_cast<C_00833908*>(self->field_0008);
    auto* weapon = reinterpret_cast<void*>(owner->field_002c);

    alignas(4) uint8_t request[0x38]{};
    CSF_AT(int, request, 0x28) = -1;
    CSF_AT(int, request, 0x2C) = -1;
    CSF_AT(int, request, 0x08) = -1;
    CSF_AT(int, request, 0x04) = 1;
    CSF_AT(float, request, 0x10) = 0;
    CSF_AT(int, request, 0x0C) = -1;
    CSF_AT(int, request, 0x18) = 0;
    CSF_AT(int, request, 0x1C) = 0;
    CSF_AT(int, request, 0x20) = 0;
    CSF_AT(void**, request, 0x24) = CSF_VTBL(0x0082D448);
    CSF_AT(int, request, 0x14) = 0;
    CSF_AT(uint8_t, request, 0x34) = 0;
    CSF_AT(uint8_t, request, 0x35) = 1;
    CSF_AT(uint8_t, request, 0x36) = 1;
    CSF_AT(int, request, 0x30) = 0;
    CSF_AT(int, request, 0x00) = 4;

    CSF_AT(int, request, 0x08) = CSF_AT(int, weapon, 0x148);
    CSF_AT(float, request, 0x10) = -1.0f;
    CSF_AT(uint32_t, request, 0x28) = owner->field_0020;
    CSF_AT(uint32_t, request, 0x2C) = owner->field_0024;
    CSF_AT(int, request, 0x04) = 3;

    void* manager = reinterpret_cast<void*>(GetGlobalValue_521BB0());
    return ApplyGlobalValueRequest(manager, nullptr, request, -1.0f);
}

HOOK(0x004852B0, CCharWeaponFsm_RequestWeaponAction, "auto");
