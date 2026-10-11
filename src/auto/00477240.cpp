#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static CSF_FUNC(uint32_t*, __fastcall, sub_4759B0, 0x004759B0, (void* self, void* edx, int a2));

static uint32_t* __fastcall C_0082d448_Initialize(
    C_0082d448* self, void* /*edx*/, int source, int configuration)
{
    self->field_0004 = -1;
    self->field_0008 = -1;
    self->vtbl = CSF_VTBL(0x0082D448);
    self->field_0014 = -1;
    self->field_0018 = -1;

    void* embedded = reinterpret_cast<uint8_t*>(self) + 0x0C;
    self->field_0010 =
        reinterpret_cast<uint32_t>(CSF_VTBL(0x0082D448));
    self->field_001c = 0;
    self->field_0020 = 0;
    self->field_0024 = 0;
    CSF_AT(uint32_t, self, 0x28) = -1;
    self->field_000c = 0;
    CSF_AT(uint8_t, self, 0x2D) = 0;
    CSF_AT(uint8_t, self, 0x2E) = 0;
    CSF_AT(uint8_t, self, 0x2F) = 0;
    CSF_AT(uint8_t, self, 0x31) = 0;
    CSF_AT(uint8_t, self, 0x2C) = 1;
    CSF_AT(uint8_t, self, 0x30) = 1;

    self->field_0004 = CSF_AT(uint32_t, source, 0x04);
    self->field_0008 = CSF_AT(uint32_t, source, 0x08);
    sub_4759B0(embedded, nullptr, configuration);
    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x00477240, C_0082d448_Initialize, "auto");
