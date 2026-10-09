#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"


CSF_FUNC(void*, __fastcall, MenuScenarioSelect_InitializeEmbeddedSubobject,
         0x00712CC0, (void* self, void* edx));

static CMenuMpLoadout* __fastcall CMenuMpLoadout_ConstructAndResetState(
    CMenuMpLoadout* self, void* /*edx*/, int argument)
{
    CSF_AT(void*, self, 0x00) = CSF_VTBL(0x0083042C);
    MenuScenarioSelect_InitializeEmbeddedSubobject(
        reinterpret_cast<uint8_t*>(self) + 4, nullptr);

    self->field_0038 = static_cast<uint32_t>(argument);
    self->field_0040 = 0xFFFFFFFFu;
    self->field_0048 = 0xFFFFFFFFu;
    self->field_0050 = 0xFFFFFFFFu;

    CSF_AT(void*, self, 0x00) = CSF_VTBL(0x00841810);
    self->field_0004 = static_cast<uint32_t>(CSF_ADDR_VT(0x008417C0));

    self->field_0018 = 0;
    self->field_004c = 0;
    self->field_0020 = 0;
    self->field_0028 = 0;
    self->field_0030 = 0;
    self->field_0034 = 0;
    self->field_0054 = 0;
    self->field_0058 = 0;
    self->field_0014 = 0;
    self->field_003c = 0;
    self->field_0044 = 0;

    // Slot 16 of the class's own vtable, called directly (0x6E5090 C_00861bd8_ClearField0010).
    reinterpret_cast<void(__fastcall*)(void*, void*)>(CSF_VTBL(0x00841810)[0x40 / 4])(self, nullptr);

    return self;
}

HOOK(0x0052F640, CMenuMpLoadout_ConstructAndResetState, "auto");
