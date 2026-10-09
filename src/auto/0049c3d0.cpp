#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(unsigned int, __fastcall, MissionScript_GrowEventRecords, 0x00413570, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, C_00833b88_Construct, 0x00492F60, (void* self, void* edx, int a2, int a3));
CSF_FUNC(double, __cdecl, Gameplay_GetInitialTimerValue, 0x00493CF0, ());
CSF_FUNC(uint32_t*, __fastcall, C_00834400_Construct, 0x004A1620, (void* self, void* edx));

CSF_GLOBAL(float, g_ZeroVectorX, 0x008BB420);
CSF_GLOBAL(float, g_ZeroVectorY, 0x008BB424);
CSF_GLOBAL(float, g_ZeroVectorZ, 0x008BB428);

static uint32_t* __fastcall CPlayerCharacter_Construct(
    CPlayerCharacter* self, void* /*edx*/, int a2, int a3)
{
    C_00833b88_Construct(self, nullptr, a2, a3);

    *reinterpret_cast<void***>(self) = CSF_VTBL(0x00833EE8);
    self->field_0004 = reinterpret_cast<uint32_t>(CSF_VTBL(0x00833E68));
    CSF_AT(uint32_t, self, 0x008) = reinterpret_cast<uint32_t>(CSF_VTBL(0x00833E60));
    self->field_000c = reinterpret_cast<uint32_t>(CSF_VTBL(0x00833E54));
    CSF_AT(uint32_t, self, 0x010) = reinterpret_cast<uint32_t>(CSF_VTBL(0x00833E4C));
    self->field_0014 = reinterpret_cast<uint32_t>(CSF_VTBL(0x00833E48));

    self->field_0230 = 0;
    self->field_0234 = 0.0f;
    self->field_0238 = 0;
    self->field_0240 = reinterpret_cast<uint32_t>(CSF_VTBL(0x0082D448));
    self->field_024c = reinterpret_cast<uint32_t>(CSF_VTBL(0x0082D448));
    self->field_0250 = -1;
    self->field_0254 = -1;
    self->field_025c = 0;
    self->field_0260 = 0.0f;
    self->field_0264 = 0;
    self->field_0244 = -1;
    self->field_0248 = -1;
    self->field_0250 = -1;
    self->field_0254 = -1;
    self->field_0258 = 0;
    std::memcpy(&self->field_025c, &g_ZeroVectorX, sizeof(float));
    self->field_0260 = g_ZeroVectorY;
    std::memcpy(&self->field_0264, &g_ZeroVectorZ, sizeof(float));

    self->field_0270 = reinterpret_cast<uint32_t>(CSF_VTBL(0x0082D448));
    self->field_0274 = -1;
    self->field_0278 = -1;
    self->field_0280 = 0;
    self->field_0284 = 0;
    self->field_0288 = 0;
    self->field_0268 = 0;
    self->field_026c = 0;
    self->field_0274 = -1;
    self->field_0278 = -1;
    self->field_027c = -1;
    std::memcpy(&self->field_0280, &g_ZeroVectorX, sizeof(float));
    std::memcpy(&self->field_0284, &g_ZeroVectorY, sizeof(float));
    std::memcpy(&self->field_0288, &g_ZeroVectorZ, sizeof(float));

    self->field_028c = reinterpret_cast<uint32_t>(CSF_VTBL(0x0082D448));
    self->field_0290 = -1;
    self->field_0294 = -1;
    self->field_029c = reinterpret_cast<uint32_t>(CSF_VTBL(0x0082D448));
    self->field_02a0 = -1;
    self->field_02a4 = -1;
    self->field_0298 = 0;
    self->field_02a0 = -1;
    self->field_02a4 = -1;
    self->field_02a8 = -1;
    self->field_02ac = -1;
    self->field_02b4 = reinterpret_cast<uint32_t>(CSF_VTBL(0x0082D448));
    self->field_02b8 = -1;
    self->field_02bc = -1;
    self->field_02b0 = 0;
    self->field_02b8 = -1;
    self->field_02bc = -1;
    self->field_02c0 = -1;
    self->field_02c4 = -1;
    self->field_02c8 = reinterpret_cast<uint32_t>(CSF_VTBL(0x0082D448));
    self->field_02cc = -1;
    self->field_02d0 = -1;
    self->field_02d4 = reinterpret_cast<uint32_t>(CSF_VTBL(0x0082D448));
    self->field_02d8 = -1;
    self->field_02dc = -1;

    self->field_02ec = 0.0f;
    self->field_02f0 = 0.0f;
    self->field_02f4 = 0.0f;
    self->field_02f8 = 0.0f;
    self->field_0300 = 0;
    self->field_0304 = 0;
    self->field_0308 = 0;
    self->field_0310 = 0;
    self->field_0314 = 0;
    self->field_0318 = 0;
    self->field_031c = 0;
    self->field_0320 = 0;
    self->field_0324 = reinterpret_cast<uint32_t>(CSF_VTBL(0x0082D448));
    self->field_0328 = -1;
    self->field_032c = -1;
    self->field_0330 = 0;
    self->field_0334 = 0;
    self->field_0338 = 0;
    self->field_033c = 1;
    self->field_033d = 0;
    self->field_033e = 1;
    self->field_0340 = 0;

    C_00834400_Construct(reinterpret_cast<uint8_t*>(self) + 0x344, nullptr);

    self->field_03a8 = 0;
    self->field_03ac = 0;
    self->field_03b0 = 0;
    self->field_03b8 = -1;
    self->field_03bc = -1;
    self->field_03b4 = reinterpret_cast<uint32_t>(CSF_VTBL(0x0082D448));
    self->field_03c0 = reinterpret_cast<uint32_t>(CSF_VTBL(0x0082D448));
    self->field_03c4 = -1;
    self->field_03c8 = -1;
    self->field_03f0 = 0;
    self->field_03f4 = 0;
    self->field_03f8 = 0;
    self->field_03fc = 10;
    self->field_0410 = 0;
    self->field_0414 = 0;
    self->field_0418 = 0;
    self->field_03a8 = 0;
    self->field_03ac = 0;
    self->field_03b0 = 0;
    self->field_0228 = 0;
    self->field_0229 = 0;
    self->field_022c = 0;
    self->field_023c = 1;
    self->field_02e0 = 0;
    self->field_02e4 = 0;
    self->field_02e8 = 0;
    self->field_0390 = 1065353216;
    self->field_0394 = 1;
    self->field_0398 = 0;
    self->field_039c = 0;
    self->field_03a0 = -1;
    self->field_03a4 = 0;
    self->field_03a5 = 0;
    self->field_03cc = -1;
    self->field_03d0 = 0;
    self->field_03d4 = 0.0f;
    self->field_03d8 = static_cast<float>(Gameplay_GetInitialTimerValue());
    self->field_03dc = 0;
    self->field_03dd = 0;
    self->field_03e8 = 0;
    self->field_03ec = 0;
    self->field_03de = 0;
    self->field_03e0 = 0.0f;
    self->field_03e4 = 0;
    self->field_0400 = 0;
    self->field_020c = 0.0f;
    self->field_0210 = 0.0f;
    self->field_0214 = 0.0f;
    self->field_0218 = 0.0f;
    self->field_021c = 0.0f;
    self->field_0220 = 0.0f;
    self->field_0224 = 0.0f;

    const uint32_t recordCount = self->field_03f4;
    if (recordCount < 0x14)
        MissionScript_GrowEventRecords(&self->field_03f0, nullptr, 20 - recordCount);

    self->field_0404 = 0;
    self->field_0408 = 0;
    self->field_040c = 0;
    self->field_03fc = 5;
    std::memcpy(&self->field_0410, &g_ZeroVectorX, sizeof(float));
    std::memcpy(&self->field_0414, &g_ZeroVectorY, sizeof(float));
    std::memcpy(&self->field_0418, &g_ZeroVectorZ, sizeof(float));

    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x0049C3D0, CPlayerCharacter_Construct, "auto");
