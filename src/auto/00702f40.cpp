#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, RpPrtStdAtomicGetEmitter, 0x0057C290, (int a1));
CSF_FUNC(int, __cdecl, RpPrtStdAtomicCreate, 0x0057C5D0, (int a1));
CSF_FUNC(int, __cdecl, RpMultiTextureSetEffect, 0x005802A0, (int a1, int a2, int a3));
CSF_FUNC(int, __cdecl, RpPrtStdEmitterSetPTank, 0x00581250, (int a1, int a2));
CSF_FUNC(int, __cdecl, RpPTankAtomicCreate, 0x00582400, (int a1, int a2, int a3));
CSF_FUNC(int, __cdecl, RpAtomicSetFrame, 0x0058F300, (int a1, int a2));
CSF_FUNC(void*, __cdecl, RwFrameCreate, 0x006235D0, ());
CSF_FUNC(int, __fastcall, GetIndexedTableValue_6AF8A0, 0x006AF8A0, (void* self, void* , int index));
CSF_FUNC(int, __cdecl, GetValue_8BB738, 0x006E04A0, ());
CSF_FUNC(int, __fastcall, ParticleEmitters_GetTableEntry, 0x006E0810, (void* self, void* , int index));
CSF_FUNC(double, __cdecl, _ceil, 0x007B1C40, (double X));

CSF_GLOBAL(float, g_UnsignedIntegerBias, 0x0082D744);
CSF_GLOBAL(float, g_ContextualDistanceScale, 0x0082DD74);

static void GfxParticleAtomic_InitializeFrame(void* frame)
{
    const uint32_t flags = CSF_AT(uint32_t, frame, 0x1C) | 0x20003;
    CSF_AT(float, frame, 0x38) = 1.0f;
    CSF_AT(float, frame, 0x24) = 1.0f;
    CSF_AT(float, frame, 0x10) = 1.0f;
    CSF_AT(float, frame, 0x20) = 0.0f;
    CSF_AT(float, frame, 0x18) = 0.0f;
    CSF_AT(float, frame, 0x14) = 0.0f;
    CSF_AT(float, frame, 0x34) = 0.0f;
    CSF_AT(float, frame, 0x30) = 0.0f;
    CSF_AT(float, frame, 0x28) = 0.0f;
    CSF_AT(float, frame, 0x48) = 0.0f;
    CSF_AT(float, frame, 0x44) = 0.0f;
    CSF_AT(float, frame, 0x40) = 0.0f;
    CSF_AT(uint32_t, frame, 0x1C) = flags;
}

static int __stdcall GfxParticleAtomic_CreateWithPTank(
    uint32_t emitterType, uint32_t particleCount)
{
    void* definitions = reinterpret_cast<void*>(GetValue_8BB738());
    const int emitterClass = GetIndexedTableValue_6AF8A0(
        definitions, nullptr, static_cast<int32_t>(emitterType));
    const int atomic = RpPrtStdAtomicCreate(emitterClass);

    void* frame = RwFrameCreate();
    GfxParticleAtomic_InitializeFrame(frame);
    RpAtomicSetFrame(atomic, reinterpret_cast<int>(frame));

    const int emitter = RpPrtStdAtomicGetEmitter(atomic);
    uint32_t emitterCapacity = particleCount;
    if (particleCount >= 0x32)
    {
        // The unsigned-to-double conversion replaces the compiler's bias adjustment.
        emitterCapacity = static_cast<uint32_t>(static_cast<int64_t>(
            _ceil(static_cast<double>(particleCount) * g_ContextualDistanceScale)));
    }

    int tankFlags = 0;
    switch (emitterType)
    {
    case 0:
    case 1:
    {
        definitions = reinterpret_cast<void*>(GetValue_8BB738());
        const int entry = ParticleEmitters_GetTableEntry(
            definitions, nullptr, static_cast<int32_t>(emitterType));
        RpMultiTextureSetEffect(
            emitter, entry,
            static_cast<int32_t>(emitterType == 0 ? emitterCapacity : particleCount));
        tankFlags = 524327;
        break;
    }
    case 2:
    {
        definitions = reinterpret_cast<void*>(GetValue_8BB738());
        const int entry = ParticleEmitters_GetTableEntry(definitions, nullptr, 2);
        RpMultiTextureSetEffect(emitter, entry, static_cast<int32_t>(emitterCapacity));
        tankFlags = 655370;
        break;
    }
    case 3:
    {
        definitions = reinterpret_cast<void*>(GetValue_8BB738());
        const int entry = ParticleEmitters_GetTableEntry(definitions, nullptr, 3);
        RpMultiTextureSetEffect(emitter, entry, static_cast<int32_t>(emitterCapacity));
        tankFlags = 524330;
        break;
    }
    default:
        break;
    }

    const int tank = RpPTankAtomicCreate(
        static_cast<int32_t>(particleCount), tankFlags | 0x10000000, 0);
    frame = RwFrameCreate();
    GfxParticleAtomic_InitializeFrame(frame);
    RpAtomicSetFrame(tank, reinterpret_cast<int>(frame));
    RpPrtStdEmitterSetPTank(emitter, tank);
    return atomic;
}

HOOK(0x00702F40, GfxParticleAtomic_CreateWithPTank, "auto");
