#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, C_00830438_Construct, 0x00712CC0,
         (void* self, void* edx));
CSF_FUNC(void, __fastcall, C_00861bd8_ClearField0010, 0x006E5090,
         (void* self, void* edx));

static CMenuAccounts* __fastcall CMenuAccounts_Construct(
    CMenuAccounts* self, void* /*edx*/, int /*unused*/)
{
    CSF_AT(uint32_t, self, 0x00) = CSF_ADDR_VT(0x0083042C);
    C_00830438_Construct(
        reinterpret_cast<uint8_t*>(self) + 4, nullptr);

    CSF_AT(uint32_t, self, 0x00) = CSF_ADDR_VT(0x00845AF8);
    self->field_0004 = 0x00845AA8;
    self->field_003c = 0;
    self->field_0018 = 0;
    self->field_001c = 0;
    self->field_0020 = 0;
    self->field_0024 = 0;
    self->field_002c = 0;
    self->field_002d = 0;
    self->field_002e = 0;
    self->field_002f = 0;
    self->field_0034 = 0xFFFFFFFFu;
    self->field_0038 = 0;
    self->field_0040 = 0;
    self->field_0028 = 0;
    self->field_0048 = 0;
    self->field_0030 = 1;

    // The slot normally contains C_00861bd8_ClearField0010.
    // Read it at call time, as the original does.
    auto postInitialize = C_00861bd8_ClearField0010;
    postInitialize =
        reinterpret_cast<decltype(postInitialize)>(CSF_VTBL(0x00845B38)[0]);
    postInitialize(self, nullptr);

    return self;
}

HOOK(0x00565470, CMenuAccounts_Construct, "auto");
