#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, C_00832f18_Destroy, 0x0047E2E0, (void* self, void* edx));
CSF_FUNC(void, __fastcall, CharTaskBase_Init_4DB3C0, 0x004DB3C0, (void* self, void* edx));

static void __fastcall C_00831ac0_Destroy(C_00831ac0* self, void* /*edx*/)
{
    auto* bytes = reinterpret_cast<uint8_t*>(self);

    self->vtbl = CSF_VTBL(0x00833030);
    self->field_0004 = reinterpret_cast<uint32_t>(CSF_VTBL(0x00833010));
    CSF_AT(void*, self, 0x2B8) = CSF_VTBL(0x0082D410);

    C_00832f18_Destroy(bytes + 0x268, nullptr);

    CharTaskBase_Init_4DB3C0(bytes + 0x230, nullptr);
    CharTaskBase_Init_4DB3C0(bytes + 0x21C, nullptr);
    CharTaskBase_Init_4DB3C0(bytes + 0x208, nullptr);
    CharTaskBase_Init_4DB3C0(bytes + 0x1F4, nullptr);
    CharTaskBase_Init_4DB3C0(bytes + 0x1E0, nullptr);

    CharTaskBase_Init_4DB3C0(bytes + 0x1CC, nullptr);
    CharTaskBase_Init_4DB3C0(bytes + 0x1B8, nullptr);
    CharTaskBase_Init_4DB3C0(bytes + 0x1A0, nullptr);
    CharTaskBase_Init_4DB3C0(bytes + 0x18C, nullptr);
    CharTaskBase_Init_4DB3C0(bytes + 0x178, nullptr);
    CharTaskBase_Init_4DB3C0(bytes + 0x164, nullptr);

    CharTaskBase_Init_4DB3C0(bytes + 0x150, nullptr);
    CharTaskBase_Init_4DB3C0(bytes + 0x13C, nullptr);
    CharTaskBase_Init_4DB3C0(bytes + 0x128, nullptr);
    CharTaskBase_Init_4DB3C0(bytes + 0x114, nullptr);
    CharTaskBase_Init_4DB3C0(bytes + 0x100, nullptr);

    CharTaskBase_Init_4DB3C0(bytes + 0xEC, nullptr);
    CharTaskBase_Init_4DB3C0(bytes + 0xD8, nullptr);
    CharTaskBase_Init_4DB3C0(bytes + 0xC4, nullptr);
    CharTaskBase_Init_4DB3C0(bytes + 0xB0, nullptr);
    CharTaskBase_Init_4DB3C0(bytes + 0x9C, nullptr);
    CharTaskBase_Init_4DB3C0(bytes + 0x88, nullptr);
    CharTaskBase_Init_4DB3C0(bytes + 0x74, nullptr);
    CharTaskBase_Init_4DB3C0(bytes + 0x60, nullptr);
    CharTaskBase_Init_4DB3C0(bytes + 0x4C, nullptr);
    CharTaskBase_Init_4DB3C0(bytes + 0x38, nullptr);
    CharTaskBase_Init_4DB3C0(bytes + 0x24, nullptr);

    self->vtbl = CSF_VTBL(0x00831AC0);
    self->field_0004 = reinterpret_cast<uint32_t>(CSF_VTBL(0x00831AA0));
    CSF_AT(void*, self, 0x18) = CSF_VTBL(0x0082D410);

    return CharTaskBase_Init_4DB3C0(self, nullptr);
}

HOOK(0x0047F7B0, C_00831ac0_Destroy, "auto");
