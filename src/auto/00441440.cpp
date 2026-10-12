#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, sub_43E250, 0x0043E250, ());
CSF_FUNC(char, __fastcall, sub_43F310, 0x0043F310,
         (void* self, void* edx, void** Block));
CSF_FUNC(int, __fastcall, Sub_43F890, 0x0043F890,
         (void* self, void* edx));

static uint32_t* __fastcall C_00831008_Construct(
    C_00831008* self, void* /*edx*/)
{
    self->vtbl = CSF_VTBL(0x00831008);

    self->field_0008 = static_cast<uint32_t>(sub_43E250());
    CSF_AT(uint8_t, reinterpret_cast<void*>(self->field_0008), 0x2D) = 1;

    auto* node = reinterpret_cast<void*>(self->field_0008);
    CSF_AT(uint32_t, node, 0x04) = reinterpret_cast<uint32_t>(node);
    node = reinterpret_cast<void*>(self->field_0008);
    CSF_AT(uint32_t, node, 0x00) = reinterpret_cast<uint32_t>(node);
    node = reinterpret_cast<void*>(self->field_0008);
    CSF_AT(uint32_t, node, 0x08) = reinterpret_cast<uint32_t>(node);
    CSF_AT(uint32_t, self, 0x0C) = 0;

    self->field_0014 = static_cast<uint32_t>(sub_43E250());
    CSF_AT(uint8_t, reinterpret_cast<void*>(self->field_0014), 0x2D) = 1;

    node = reinterpret_cast<void*>(self->field_0014);
    CSF_AT(uint32_t, node, 0x04) = reinterpret_cast<uint32_t>(node);
    node = reinterpret_cast<void*>(self->field_0014);
    CSF_AT(uint32_t, node, 0x00) = reinterpret_cast<uint32_t>(node);
    node = reinterpret_cast<void*>(self->field_0014);
    CSF_AT(uint32_t, node, 0x08) = reinterpret_cast<uint32_t>(node);

    CSF_AT(uint32_t, self, 0x18) = 0;
    self->field_0024 = 0;
    self->field_0028 = 0;
    self->field_002c = 0;
    self->field_0034 = 0;
    self->field_0038 = 0;
    CSF_AT(uint32_t, self, 0x3C) = 0;

    sub_43F310(
        reinterpret_cast<uint8_t*>(self) + 0x04, nullptr,
        CSF_AT(void**, reinterpret_cast<void*>(self->field_0008), 0x04));

    node = reinterpret_cast<void*>(self->field_0008);
    CSF_AT(uint32_t, node, 0x04) = reinterpret_cast<uint32_t>(node);
    node = reinterpret_cast<void*>(self->field_0008);
    CSF_AT(uint32_t, self, 0x0C) = 0;
    CSF_AT(uint32_t, node, 0x00) = reinterpret_cast<uint32_t>(node);
    node = reinterpret_cast<void*>(self->field_0008);
    CSF_AT(uint32_t, node, 0x08) = reinterpret_cast<uint32_t>(node);

    self->field_001c = 0xFFFFFFFF;
    Sub_43F890(reinterpret_cast<uint8_t*>(self) + 0x30, nullptr);
    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x00441440, C_00831008_Construct, "auto");
