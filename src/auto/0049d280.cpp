#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(std::uint32_t*, __fastcall, C_00833908_Construct, 0x00490940, (C_00833908* self, void* , int objectId, int stateCode));

static int __fastcall C_00834050_Construct(
    C_00834050* self, void* /*edx*/, int objectId, int stateCode)
{
    C_00833908_Construct(self, nullptr, objectId, stateCode);

    *reinterpret_cast<void***>(self) = CSF_VTBL(0x00834050);
    self->field_0004 = reinterpret_cast<std::uint32_t>(CSF_ADDR_DATA(0x00833FD0));
    CSF_AT(std::uint32_t, self, 0x008) =
        reinterpret_cast<std::uint32_t>(CSF_ADDR_DATA(0x00833FCC));
    self->field_000c = reinterpret_cast<std::uint32_t>(CSF_ADDR_DATA(0x00833FC0));
    CSF_AT(std::uint32_t, self, 0x010) =
        reinterpret_cast<std::uint32_t>(CSF_ADDR_DATA(0x00833FB8));
    self->field_0014 = reinterpret_cast<std::uint32_t>(CSF_ADDR_DATA(0x00833FB4));

    self->field_018c = -1;
    self->field_0190 = -1;
    self->field_0188 = reinterpret_cast<std::uint32_t>(CSF_VTBL(0x0082D448));
    self->field_019c = 0;
    self->field_01a0 = 0;
    self->field_01a4 = 0;
    self->field_01a8 = reinterpret_cast<std::uint32_t>(CSF_VTBL(0x0082D448));
    self->field_01ac = -1;
    self->field_01b0 = -1;
    self->field_0198 = 0;
    self->field_0199 = 0;
    self->field_01b4 = 0;
    self->field_0184 = 0;
    self->field_0194 = -1;

    return static_cast<int>(reinterpret_cast<std::uintptr_t>(self));
}

HOOK(0x0049D280, C_00834050_Construct, "auto");
