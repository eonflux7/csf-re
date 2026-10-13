#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, InitDefaultsRecord, 0x00506B00, (void* self, void*));
CSF_FUNC(C_00860178*, __fastcall, C_00860178_Construct, 0x006B2820, (C_00860178* self, void*));
CSF_FUNC(int, __fastcall, C_00863044_Construct, 0x006FE3F0, (C_00863044* self, void*));

CSF_GLOBAL(uint32_t, g_DefaultComponentX, 0x008A7F54);
CSF_GLOBAL(uint32_t, g_DefaultComponentY, 0x008A7F58);
CSF_GLOBAL(uint32_t, g_DefaultComponentZ, 0x008A7F5C);
CSF_GLOBAL(uint32_t, g_DefaultParameterE8, 0x00861254);
CSF_GLOBAL(uint32_t, g_DefaultParameterEC, 0x00861258);

static constexpr uint32_t k_DefaultParameterE8 = 0x3E4CCCCD;
static constexpr uint32_t k_DefaultParameterEC = 0x3ECCCCCD;
static_assert(k_DefaultParameterE8 == 1045220557);
static_assert(k_DefaultParameterEC == 1053609165);

static int __fastcall C_00861120_Construct(C_00861120* self, void* /*edx*/)
{
    C_00860178_Construct(reinterpret_cast<C_00860178*>(self), nullptr);
    self->vtbl = CSF_VTBL(0x00861120);
    InitDefaultsRecord(&self->field_0004, nullptr);

    self->field_0040 = 0;
    self->field_0044 = 0;
    self->field_0048 = 0;
    self->field_004c = 10;
    self->field_0054 = 0;
    self->field_0058 = 0;
    self->field_005c = 0;
    self->field_0060 = 10;
    self->field_0064 = 0;
    self->field_0068 = 0;
    self->field_006c = 0;
    self->field_0070 = 10;

    C_00863044_Construct(
        reinterpret_cast<C_00863044*>(
            reinterpret_cast<uint8_t*>(self) + 0x74),
        nullptr);

    self->field_00dc = 0;
    self->field_00e0 = 0;
    self->field_00e4 = 0;
    self->field_00fc = 0;
    self->field_0100 = 0;
    self->field_0104 = 0;
    self->field_003c = 0;
    self->field_00d9 = 0;
    self->field_00d8 = 1;
    self->field_00da = 1;

    self->field_00dc = g_DefaultComponentX;
    self->field_00e0 = g_DefaultComponentY;
    self->field_00e4 = g_DefaultComponentZ;

    // Original constant data: k_DefaultParameterE8 and k_DefaultParameterEC.
    self->field_00e8 = g_DefaultParameterE8;
    const uint32_t defaultParameterEC = g_DefaultParameterEC;
    self->field_00f8 = 0;
    self->field_0050 = 0;
    self->field_00f0 = 0;
    self->field_00f4 = 0;
    self->field_00ec = defaultParameterEC;

    return reinterpret_cast<int>(self);
}

HOOK(0x006D6530, C_00861120_Construct, "auto");
