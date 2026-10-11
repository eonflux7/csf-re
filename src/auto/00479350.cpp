#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static CSF_FUNC(void*, __fastcall, InitializeFirearmBase, 0x00477CF0,
               (void* self, void* edx, int a2, int a3));

static uint32_t* __fastcall C_008317c8_Initialize(
    C_008317c8* self, void* /*edx*/, int a2, int a3)
{
    InitializeFirearmBase(self, nullptr, a2, a3);
    CSF_AT(void**, self, 0x00) = CSF_VTBL(0x008317C8);
    self->field_0068 = -1;
    self->field_006c = -1;
    CSF_AT(void*, self, 0x64) = CSF_VTBL(0x0082D448);
    self->field_0070 = 0;
    self->field_0074 = 0;
    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x00479350, C_008317c8_Initialize, "auto");
