#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, NetCogibleDdl_InitializeVector, 0x0068E2B0, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, TextInputSubobject_Init, 0x00697130, (void* self, void* edx, uint8_t* arg));
CSF_FUNC(uint32_t*, __cdecl, NetCogibleDdl_GetSecondaryDescriptor, 0x0073FBA0, ());
CSF_FUNC(uint8_t*, __fastcall, NetCogibleDdl_InitializeEmbeddedState, 0x00740810, (void* self, void* edx));
CSF_FUNC(uint32_t*, __cdecl, NetCogibleDdl_GetPrimaryDescriptor, 0x00741240, ());
CSF_FUNC(int, __fastcall, NetCogibleDdl_InitializeBase, 0x00745800, (void* self, void* edx));

CSF_GLOBAL(uint8_t, g_byte_82E65C, 0x0082E65C);

static uint8_t* __fastcall C_00867900_Construct(C_00867900* self, void* /*edx*/)
{
    auto* bytes = reinterpret_cast<uint8_t*>(self);

    NetCogibleDdl_InitializeBase(self, nullptr);
    NetCogibleDdl_InitializeEmbeddedState(bytes + 0x6B0, nullptr);
    self->field_06b0 = reinterpret_cast<uint32_t>(CSF_VTBL(0x008678EC));
    self->vtbl = reinterpret_cast<decltype(self->vtbl)>(CSF_VTBL(0x00867900));

    CSF_AT(uint32_t, self, 0x6D4) =
        reinterpret_cast<uint32_t>(NetCogibleDdl_GetPrimaryDescriptor());
    CSF_AT(uint32_t, self, 0x6D8) = 0;
    CSF_AT(uint32_t, self, 0x6E0) =
        reinterpret_cast<uint32_t>(NetCogibleDdl_GetSecondaryDescriptor());
    CSF_AT(uint32_t, self, 0x6E4) = 0;

    NetCogibleDdl_InitializeVector(bytes + 0x6F4, nullptr);
    NetCogibleDdl_InitializeVector(&self->field_0700, nullptr);
    NetCogibleDdl_InitializeVector(&self->field_070c, nullptr);

    self->field_0740 = 7;
    self->field_073c = 0;
    *reinterpret_cast<uint16_t*>(&self->field_072c) = 0;
    self->field_0800 = 0;
    self->field_0804 = 0;
    self->field_0808 = 0;
    self->field_080c = 0;
    self->field_0810 = 0;
    self->field_0814 = 0;
    self->field_0818 = 0;
    self->field_081c = 0;
    self->field_0820 = 0;
    self->field_06cc = 0;
    self->field_06e8 = 0;
    self->field_06ea = static_cast<uint16_t>(-1);
    self->field_06ec = 0;
    self->field_06f0 = 0;
    self->field_06f1 = 0;
    self->field_06f2 = 0;
    self->field_06bc = static_cast<uint32_t>(-1);
    self->field_06cd = 1;
    self->field_06f3 = 1;
    self->field_06fc = 0;
    self->field_06f8 = 0;
    CSF_AT(uint32_t, self, 0x6F4) = 0;
    self->field_0708 = 0;
    self->field_0704 = 0;
    self->field_0700 = 0;
    self->field_0714 = 0;
    self->field_0710 = 0;
    self->field_070c = 0;
    self->field_0718 = 0;
    self->field_071c = static_cast<uint32_t>(-1);
    self->field_0720 = static_cast<uint32_t>(-1);
    self->field_0724 = 0;

    if (self->field_044c != 0xFF)
        self->field_044c = static_cast<uint8_t>(-1);

    self->field_0725 = 0;
    TextInputSubobject_Init(bytes + 0x728, nullptr, &g_byte_82E65C);
    self->field_0744 = static_cast<uint32_t>(-1);
    self->field_0748 = 0;
    self->field_0749 = 0;
    self->field_074c = 0;
    self->field_0750 = 0;

    if (self->field_0668 != static_cast<uint32_t>(-1))
        self->field_0668 = static_cast<uint32_t>(-1);
    if (self->field_0674)
        self->field_0674 = 0;
    if (self->field_0440)
        self->field_0440 = 0;
    if (self->field_044f)
        self->field_044f = 0;
    if (self->field_0446 != 0xFFFF)
        self->field_0446 = static_cast<uint16_t>(-1);

    self->field_0800 = 0;
    self->field_0804 = 0;
    self->field_080c = 0;
    self->field_0808 = 0;
    self->field_0810 = 0;
    self->field_0818 = 0;
    self->field_0814 = 0;
    self->field_081c = 0;
    self->field_07f4 = 0;
    self->field_07f8 = 0;
    self->field_07fc = 0;
    self->field_0824 = 0;
    self->field_0828 = 0;
    self->field_0830 = 0;
    self->field_0834 = 0;
    self->field_0838 = 0;
    self->field_083c = 0;
    self->field_0820 = 0;
    return bytes;
}

HOOK(0x00741B20, C_00867900_Construct, "auto");
