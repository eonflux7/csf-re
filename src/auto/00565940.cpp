#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, MenuAccounts_ConstructEmbeddedBase,
         0x00712CC0, (void* self, void* edx));
CSF_FUNC(void, __fastcall, C_00861bd8_ClearField0010,
         0x006E5090, (void* self, void* edx));

using ClearFieldMethod = void (__fastcall*)(void*, void*);

static CMenuMpNetworkType* __fastcall CMenuMpNetworkType_Construct(
    CMenuMpNetworkType* self, void* /*edx*/, int argument)
{
    void* embeddedBase = reinterpret_cast<uint8_t*>(self) + 4;

    CSF_AT(void*, self, 0x00) = CSF_VTBL(0x0083042C);
    MenuAccounts_ConstructEmbeddedBase(embeddedBase, nullptr);

    self->field_003c = static_cast<uint32_t>(argument);
    CSF_AT(void*, self, 0x00) = CSF_VTBL(0x00845BF0);
    self->field_0004 = static_cast<uint32_t>(CSF_ADDR_VT(0x00845BA0));

    self->field_0038 = 0;
    self->field_0048 = 0;
    self->field_0014 = 0;
    self->field_0018 = 0;
    self->field_001c = 0;
    self->field_0020 = 0;
    self->field_002c = 0;
    self->field_0024 = 0;
    self->field_0025 = 0;
    self->field_0026 = 0;
    self->field_0027 = 0;
    self->field_0034 = 0;
    self->field_0044 = 0;
    self->field_0030 = 0xFFFFFFFFu;
    self->field_0049 = 0;

    // This slot resolves to C_00861bd8_ClearField0010 (0x006E5090).
    // Preserve the original indirect dispatch rather than bypassing the slot.
    reinterpret_cast<ClearFieldMethod>(CSF_VTBL(0x00845C30)[0])(self, nullptr);
    return self;
}

HOOK(0x00565940, CMenuMpNetworkType_Construct, "auto");
