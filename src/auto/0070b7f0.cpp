#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RpPTankAtomicLock, 0x005821D0, (int a1, uint32_t* a2, unsigned int a3, int a4));
CSF_FUNC(int, __cdecl, RpPTankAtomicUnlock, 0x005823E0, (int a1));
CSF_FUNC(int, __cdecl, RwMatrixScale, 0x006249A0, (int a1, float* a2, int a3));
CSF_FUNC(float*, __stdcall, WaterParticle_ComputeBasis, 0x0070B380, (float* a1, float* a2, void* a3, void* a4));

CSF_GLOBAL(int32_t, g_PTankPluginOffset, 0x008B09EC);

static char __fastcall WaterParticleEmitter_AddParticle(uint32_t* self, void* /*edx*/, int a2)
{
    uint8_t* src = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(a2));

    int tank = static_cast<int>(CSF_AT(uint32_t, self, 8));
    if (!tank)
        return 0;

    uint32_t lockA[2];
    uint32_t lockB[2];
    RpPTankAtomicLock(tank, lockA, 8, 0x40000000);
    RpPTankAtomicLock(static_cast<int>(CSF_AT(uint32_t, self, 8)), lockB, 128, 0x40000000);

    int offB = static_cast<int>(CSF_AT(uint32_t, self, 0x18)) * static_cast<int>(lockB[1]);
    uint8_t* dstB = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(lockB[0] + offB));
    CSF_AT(uint32_t, dstB, 0) = CSF_AT(uint32_t, src, 0x1C);
    CSF_AT(uint32_t, dstB, 4) = CSF_AT(uint32_t, src, 0x20);
    CSF_AT(uint32_t, dstB, 8) = CSF_AT(uint32_t, src, 0x24);
    CSF_AT(uint32_t, dstB, 0xC) = CSF_AT(uint32_t, src, 0x28);

    float pos[3] = {0.0f, 0.0f, 0.0f};
    int right[3] = {0, 0, 0};
    int up[3] = {0, 0, 0};

    WaterParticle_ComputeBasis(reinterpret_cast<float*>(src), pos, right, up);

    int offA = static_cast<int>(CSF_AT(uint32_t, self, 0x18)) * static_cast<int>(lockA[1]);
    uint8_t* m = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(offA + lockA[0]));
    CSF_AT(float, m, 0x30) = pos[0];
    CSF_AT(float, m, 0x34) = pos[1];
    CSF_AT(float, m, 0x38) = pos[2];
    CSF_AT(int, m, 0) = right[0];
    CSF_AT(int, m, 4) = right[1];
    CSF_AT(int, m, 8) = right[2];
    CSF_AT(int, m, 0x10) = up[0];
    CSF_AT(int, m, 0x14) = up[1];
    CSF_AT(int, m, 0x18) = up[2];

    float scale = CSF_AT(float, src, 0x2C);
    pos[0] = scale;
    pos[1] = scale;
    pos[2] = scale;
    RwMatrixScale(static_cast<int>(reinterpret_cast<uintptr_t>(m)), pos, 1);

    RpPTankAtomicUnlock(static_cast<int>(CSF_AT(uint32_t, self, 8)));

    uint32_t count = CSF_AT(uint32_t, self, 0x14);
    uint32_t capacity = CSF_AT(uint32_t, self, 0x10);
    if (count < capacity)
        CSF_AT(uint32_t, self, 0x14) = count + 1;

    CSF_AT(uint32_t, self, 0x18) = (CSF_AT(uint32_t, self, 0x18) + 1) % capacity;

    uint8_t* plugin = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(
        static_cast<uint32_t>(g_PTankPluginOffset) + CSF_AT(uint32_t, self, 8)));
    uint8_t* data = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(CSF_AT(uint32_t, plugin, 0)));
    CSF_AT(uint32_t, data, 0x40) |= 0x800000;

    plugin = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(
        static_cast<uint32_t>(g_PTankPluginOffset) + CSF_AT(uint32_t, self, 8)));
    data = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(CSF_AT(uint32_t, plugin, 0)));
    CSF_AT(uint32_t, data, 4) = CSF_AT(uint32_t, self, 0x14);

    return 1;
}

HOOK(0x0070B7F0, WaterParticleEmitter_AddParticle, "auto");
