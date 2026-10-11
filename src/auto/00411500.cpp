#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, C_0082e520_Initialize, 0x0041E0F0,
         (void* self, void* edx));
CSF_FUNC(int, __fastcall, Member08_InitializeFromSource, 0x00410F50,
         (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, Member08_Release, 0x00410DA0,
         (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalValue_753A30, 0x00753A30, ());

CSF_GLOBAL(float, g_ZeroVectorX, 0x008BB420);
CSF_GLOBAL(float, g_ZeroVectorY, 0x008BB424);
CSF_GLOBAL(float, g_ZeroVectorZ, 0x008BB428);

static uint32_t* __fastcall C_0082deb8_Initialize(
    C_0082deb8* self, void* /*edx*/)
{
    alignas(4) uint8_t temporaryMember[0x22C];

    C_0082e520_Initialize(self, nullptr);
    *reinterpret_cast<void***>(self) = CSF_VTBL(0x0082DEB8);

    self->field_0018 = 0.0f;
    self->field_001c = 0.0f;
    self->field_0020 = 0.0f;
    self->field_0024 = 0;
    self->field_0028 = 0;
    self->field_002c = 0;
    self->field_0030 = 0;
    self->field_0078 = 0;
    self->field_0044 = 0;
    self->field_0048 = -1;
    self->field_004c = static_cast<uint8_t>(-1);
    self->field_007c = 0;
    self->field_0080 = 10;
    self->field_0010 = 0;
    self->field_0014 = 0;

    std::memcpy(&self->field_0024, &g_ZeroVectorX, sizeof(float));
    std::memcpy(&self->field_0028, &g_ZeroVectorY, sizeof(float));
    std::memcpy(&self->field_002c, &g_ZeroVectorZ, sizeof(float));

    self->field_003d = 1;
    self->field_003e = 0;
    self->field_0034 = -1082130432;
    self->field_0038 = -1082130432;
    self->field_003c = 0;

    int globalValue = GetGlobalValue_753A30();
    Member08_InitializeFromSource(temporaryMember, nullptr, globalValue + 8);
    self->field_0040 = -1;
    Member08_Release(temporaryMember, nullptr);

    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x00411500, C_0082deb8_Initialize, "auto");
