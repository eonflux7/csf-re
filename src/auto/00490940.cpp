#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, GrowByteBuffer, 0x0048EA50, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(void*, __fastcall, C_00867768_Construct, 0x0073F700, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GameConfig_IsStateCodeAllowed, 0x00755810, (std::uint32_t stateCode));

static std::uint32_t* __fastcall C_00833908_Construct(
    C_00833908* self, void* /*edx*/, int objectId, int stateCode)
{
    self->vtbl = CSF_VTBL(0x0082D428);
    self->field_0004 = reinterpret_cast<std::uint32_t>(CSF_VTBL(0x008330E8));
    CSF_AT(void*, self, 0x008) = CSF_VTBL(0x008304D8);
    self->field_000c = reinterpret_cast<std::uint32_t>(CSF_VTBL(0x00833860));
    CSF_AT(void*, self, 0x010) = CSF_VTBL(0x0082D418);

    C_00867768_Construct(&self->field_0014, nullptr);
    self->field_0014 = reinterpret_cast<std::uint32_t>(CSF_VTBL(0x0083386C));
    self->vtbl = CSF_VTBL(0x00833908);
    self->field_0004 = reinterpret_cast<std::uint32_t>(CSF_VTBL(0x00833888));
    CSF_AT(void*, self, 0x008) = CSF_VTBL(0x00833884);
    self->field_000c = reinterpret_cast<std::uint32_t>(CSF_VTBL(0x00833878));
    CSF_AT(void*, self, 0x010) = CSF_VTBL(0x00833870);

    self->field_0020 = -1;
    self->field_0024 = -1;
    self->field_001c = reinterpret_cast<std::uint32_t>(CSF_VTBL(0x0082D448));
    self->field_0084 = 0;
    self->field_0088 = 0;
    self->field_008c = 0;
    self->field_0094 = 0;
    self->field_0098 = 0;
    self->field_009c = 0;
    self->field_00a0 = 0;
    self->field_00a4 = 0;
    self->field_00a8 = 10;
    self->field_00b8 = 10;
    self->field_00ac = 0;
    self->field_00b0 = 0;
    self->field_00b4 = 0;
    self->field_00cc = 10;
    self->field_00c0 = 0;
    self->field_00c4 = 0;
    self->field_00c8 = 0;
    self->field_00dc = 10;
    self->field_00d0 = 0;
    self->field_00d4 = 0;
    self->field_00d8 = 0;
    self->field_00e0 = -1;
    self->field_00e4 = -1082130432;
    self->field_00e8 = 0;
    self->field_00ec = 0;
    self->field_00f0 = 0;
    self->field_00f4 = 0;
    self->field_00f8 = 0;
    self->field_00fc = reinterpret_cast<std::uint32_t>(CSF_VTBL(0x0082D448));
    self->field_0100 = -1;
    self->field_0104 = -1;
    self->field_0108 = 0;
    self->field_010c = 0;
    self->field_0110 = 0;
    self->field_0114 = 1;
    self->field_0115 = 0;
    self->field_0116 = 1;
    self->field_0118 = 0;
    self->field_0120 = -1;
    self->field_011c = 1;
    self->field_0124 = 1;
    self->field_0128 = -1;
    self->field_012c = reinterpret_cast<std::uint32_t>(CSF_VTBL(0x0082D448));
    self->field_0130 = -1;
    self->field_0134 = -1;
    self->field_013c = reinterpret_cast<std::uint32_t>(CSF_VTBL(0x0082D448));
    self->field_0140 = -1;
    self->field_0144 = -1;
    self->field_0148 = reinterpret_cast<std::uint32_t>(CSF_VTBL(0x0082D448));
    self->field_014c = -1;
    self->field_0150 = -1;
    self->field_0158 = 0;
    self->field_015c = 0;
    self->field_0160 = 0;
    self->field_0168 = 0;
    self->field_0154 = 0;
    self->field_0164 = -1082130432;
    self->field_0170 = 0;

    void* game = reinterpret_cast<void*>(GetValue8B0554());
    void* registry = CSF_AT(void*, game, 0x1FC8);
    const std::uint32_t definition =
        reinterpret_cast<std::uint32_t(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, registry, 0)[2])(registry, nullptr, objectId);

    self->field_0028 = objectId;
    self->field_0050 = stateCode;
    self->field_002c = definition;
    self->field_0054 = CSF_AT(std::uint32_t, reinterpret_cast<void*>(definition), 0xB8);

    self->field_0170 = 0;
    self->field_0170 = 1;
    self->field_0170 = 17;
    self->field_0170 = 21;
    self->field_0170 = 277;
    self->field_0170 = 789;
    self->field_0170 = 917;
    self->field_0170 = 2965;

    const bool stateAllowed =
        static_cast<std::uint8_t>(GameConfig_IsStateCodeAllowed(self->field_0050)) != 0;
    if (stateAllowed)
        self->field_0170 = self->field_0170 | 0x2000;
    else
        self->field_0170 = self->field_0170 & 0xFFFFDFFF;

    self->field_0078 = -1;
    self->field_0080 = -1;
    self->field_0090 = -1;
    self->field_0174 = 1;
    self->field_007c = 0;
    self->field_0058 = 0;
    self->field_0030 = 0;
    self->field_0034 = 0;
    self->field_016c = 0;
    self->field_0038 = 0;
    self->field_003c = 0;
    self->field_0040 = 0;
    self->field_0044 = 0;
    self->field_0048 = 0;
    self->field_004c = 0;
    self->field_0175 = 0;

    const std::uint32_t capacity = self->field_00b0;
    if (capacity < 0x1E)
        GrowByteBuffer(&self->field_00ac, nullptr, static_cast<int>(30 - capacity));

    for (std::uint32_t index = self->field_00b4; index < 0x1E; ++index)
        reinterpret_cast<std::uint8_t*>(self->field_00ac)[index] = 0;

    self->field_00b4 = 30;
    self->field_0084 = 0;
    self->field_0088 = 0;
    self->field_00bc = 0;
    self->field_0094 = 0;
    self->field_0178 = 0;
    self->field_017c = 0;
    self->field_0180 = 0;
    self->field_008c = 0;
    return reinterpret_cast<std::uint32_t*>(self);
}

HOOK(0x00490940, C_00833908_Construct, "auto");
