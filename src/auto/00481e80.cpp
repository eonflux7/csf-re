#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static std::uint32_t* __fastcall CCharWeaponFsm_Construct(
    CCharWeaponFsm* self, void* /*edx*/, std::uint32_t* owner)
{
    self->field_0004 = reinterpret_cast<std::uint32_t>(CSF_VTBL(0x0082D428));
    self->vtbl = CSF_VTBL(0x00833228);
    self->field_0004 = reinterpret_cast<std::uint32_t>(CSF_VTBL(0x0083320C));

    self->field_0018 = 0;
    self->field_001c = 0;
    self->field_0020 = 0;
    self->field_0030 = reinterpret_cast<std::uint32_t>(CSF_VTBL(0x0082D448));
    self->field_0040 = reinterpret_cast<std::uint32_t>(CSF_VTBL(0x0082D448));
    self->field_0044 = -1;
    self->field_0048 = -1;
    self->field_004c = 0;
    self->field_0050 = 0;
    self->field_0054 = 0;
    self->field_003c = 0;
    self->field_0058 = -1;
    self->field_005d = 0;
    self->field_005e = 0;
    self->field_005f = 0;
    self->field_0061 = 0;
    self->field_005c = 1;
    self->field_0060 = 1;
    self->field_0034 = -1;
    self->field_0038 = -1;
    self->field_0008 = reinterpret_cast<std::uint32_t>(owner);
    self->field_000c = 0;
    self->field_0010 = 0;
    self->field_0064 = 0;
    self->field_002c = 0;

    const std::uint32_t statesBegin = self->field_0018;
    const std::int32_t stateCount = statesBegin
        ? (static_cast<std::int32_t>(self->field_001c - statesBegin) >> 2)
        : 0;

    self->field_006c = 0.0f;
    self->field_0068 = 0;
    self->field_0070 = 0;
    self->field_0071 = 0;
    self->field_0024 = (stateCount != 0) - 1;
    self->field_0028 = -1;
    self->field_0069 = 1;

    self->field_0034 = CSF_AT(std::uint32_t, owner, 0x20);
    self->field_0038 = CSF_AT(std::uint32_t, owner, 0x24);
    const std::int32_t ownerMode = CSF_AT(std::int32_t, owner, 0x78);
    self->field_005c = 1;
    self->field_0058 = ownerMode != 1;

    return reinterpret_cast<std::uint32_t*>(self);
}

HOOK(0x00481E80, CCharWeaponFsm_Construct, "auto");
