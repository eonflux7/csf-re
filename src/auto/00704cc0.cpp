#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

using ParticleBatchCallback = int (__cdecl*)(int, uint32_t*, int);

CSF_FUNC(int, __cdecl, RpPrtStdEmitterForAllParticleBatch, 0x00580160,
         (int a1, ParticleBatchCallback a2, int a3));
CSF_FUNC(int, __cdecl, RpPTankAtomicLock, 0x005821D0, (int a1, uint32_t* a2, unsigned int a3, int a4));
CSF_FUNC(int, __cdecl, RpPTankAtomicUnlock, 0x005823E0, (int a1));

static int __cdecl GfxParticleRender_UpdateParticleTankBatches(
    int /*atomic*/, int emitter, uint32_t* arguments)
{
    alignas(8) uint8_t contextBytes[0x14] = {};
    alignas(8) uint8_t colorLockBytes[0x8] = {};
    alignas(8) uint8_t positionLockBytes[0x8] = {};

    void* state = reinterpret_cast<void*>(arguments[1]);
    void* tank = CSF_AT(void*, state, 0x10);
    const bool usesMatrix = (CSF_AT(uint32_t, tank, 0) & 8) != 0;

    RpPTankAtomicLock(
        CSF_AT(int, tank, 0x18),
        reinterpret_cast<uint32_t*>(positionLockBytes),
        usesMatrix ? 8 : 1,
        0x40000000);

    RpPTankAtomicLock(
        CSF_AT(int, CSF_AT(void*, state, 0x10), 0x18),
        reinterpret_cast<uint32_t*>(colorLockBytes),
        2,
        0x40000000);

    CSF_AT(uint32_t, contextBytes, 0) = arguments[0];
    CSF_AT(void*, contextBytes, 0x4) = state;
    CSF_AT(void*, contextBytes, 0x8) = positionLockBytes;
    CSF_AT(void*, contextBytes, 0xC) = colorLockBytes;
    CSF_AT(uint8_t, contextBytes, 0x10) = usesMatrix;

    RpPrtStdEmitterForAllParticleBatch(
        emitter,
        reinterpret_cast<ParticleBatchCallback>(CSF_ADDR_FN(0x007047C0)),
        reinterpret_cast<int>(contextBytes));
    RpPrtStdEmitterForAllParticleBatch(
        emitter,
        reinterpret_cast<ParticleBatchCallback>(CSF_ADDR_FN(0x006C2AB0)),
        reinterpret_cast<int>(contextBytes));

    RpPTankAtomicUnlock(
        CSF_AT(int, CSF_AT(void*, state, 0x10), 0x18));
    return emitter;
}

HOOK(0x00704CC0, GfxParticleRender_UpdateParticleTankBatches, "auto");
