#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, InitEmbeddedSubobject, 0x00712CC0,
         (void* self, void* edx));

CSF_FUNC(void, __fastcall, C_00861bd8_ClearField0010, 0x006E5090,
         (void* self, void* edx));

static void(__fastcall* const kClearField0010Target)(void*, void*) =
    C_00861bd8_ClearField0010;

static CMenuOptions* __fastcall CMenuOptions_Construct(
    CMenuOptions* self, void* /*edx*/, int argument)
{
    void* embeddedSubobject = reinterpret_cast<uint8_t*>(self) + 0x04;

    CSF_AT(void*, self, 0x00) = CSF_VTBL(0x0083042C);
    InitEmbeddedSubobject(embeddedSubobject, nullptr);

    self->field_0030 = static_cast<uint32_t>(argument);

    CSF_AT(void*, self, 0x00) = CSF_VTBL(0x00842638);
    self->field_0004 = static_cast<uint32_t>(CSF_ADDR_VT(0x008425E8));

    self->field_001c = 0;
    self->field_0014 = 0;
    self->field_0034 = 0;
    self->field_0020 = 0;
    self->field_0018 = 0;
    self->field_0024 = 0;
    self->field_0028 = 0;
    self->field_002c = 0;

    reinterpret_cast<void(__fastcall*)(void*, void*)>(
        CSF_VTBL(0x00842678)[0])(self, nullptr);

    return self;
}

HOOK(0x0053AF80, CMenuOptions_Construct, "auto");
