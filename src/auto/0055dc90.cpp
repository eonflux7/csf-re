#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, CMenuScreen_ConstructEmbeddedBase, 0x00712CC0,
         (void* self, void* edx));
CSF_FUNC(void, __fastcall, C_00861bd8_ClearField0010, 0x006E5090,
         (void* self, void* edx));

// This vtable slot contains C_00861bd8_ClearField0010.
// Read the slot at call time, as the original does.
CSF_GLOBAL(uint32_t, g_PlayerInfoClearField0010Slot, 0x008455B0);

static CMenuPlayerInfo* __fastcall CMenuPlayerInfo_Construct(
    CMenuPlayerInfo* self, void* /*edx*/, int argument)
{
    CSF_AT(void*, self, 0x00) = CSF_VTBL(0x0083042C);
    CMenuScreen_ConstructEmbeddedBase(
        reinterpret_cast<uint8_t*>(self) + 4, nullptr);

    self->field_003c = static_cast<uint32_t>(argument);
    CSF_AT(void*, self, 0x00) = CSF_VTBL(0x00845570);
    self->field_0004 = static_cast<uint32_t>(CSF_ADDR_VT(0x00845520));

    self->field_001c = 0;
    self->field_0048 = 0;
    self->field_0014 = 0;
    self->field_0018 = 0;
    self->field_0020 = 0;
    self->field_0024 = 0;
    self->field_0028 = 0;
    self->field_002c = 0;
    self->field_0038 = 0;
    self->field_0030 = 0;
    self->field_0034 = 0;
    self->field_0040 = 0;
    self->field_0044 = 0x40400000u;

    reinterpret_cast<void(__fastcall*)(void*, void*)>(
        static_cast<uintptr_t>(g_PlayerInfoClearField0010Slot))(
            self, nullptr);

    return self;
}

HOOK(0x0055DC90, CMenuPlayerInfo_Construct, "auto");
