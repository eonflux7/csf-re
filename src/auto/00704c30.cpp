#include <cstdint>
#include "hook.h"
#include "globals.h"

using ParticleBatchCallbackFn = int(__cdecl*)(int, uint32_t*, int);

CSF_FUNC(int, __cdecl, RpPTankAtomicLock, 0x005821D0, (int a1, uint32_t* a2, unsigned int a3, int a4));
CSF_FUNC(int, __cdecl, RpPTankAtomicUnlock, 0x005823E0, (int a1));
CSF_FUNC(int, __cdecl, RpPrtStdEmitterForAllParticleBatch, 0x00580160,
         (int a1, ParticleBatchCallbackFn a2, int a3));

// Per-batch callback at 0x7047C0; its body lives elsewhere, referenced here only by address.
static ParticleBatchCallbackFn ParticleBatchCallback() {
    return reinterpret_cast<ParticleBatchCallbackFn>(CSF_ADDR_FN(0x007047C0));
}

static int __cdecl GfxParticle_RenderEmitterBatches(int a1, int a2, uint32_t* a3) {
    (void)a1;

    uint8_t* ownerObj = reinterpret_cast<uint8_t*>(a3[1]);
    uint32_t* emitterHeader = reinterpret_cast<uint32_t*>(CSF_AT(uint32_t, ownerObj, 0x10));
    bool emitterFlagBit3 = (emitterHeader[0] & 8) != 0;

    // Two 8-byte lock slots in the original frame (ebp-0x8 and ebp-0x10).
    alignas(8) uint8_t outerLockBytes[8];
    alignas(8) uint8_t innerLockBytes[8];

    RpPTankAtomicLock(static_cast<int>(emitterHeader[6]),
                      reinterpret_cast<uint32_t*>(outerLockBytes),
                      emitterFlagBit3 ? 8u : 1u,
                      0x40000000);

    uint32_t* emitterHeaderAgain = reinterpret_cast<uint32_t*>(CSF_AT(uint32_t, ownerObj, 0x10));
    RpPTankAtomicLock(static_cast<int>(CSF_AT(uint32_t, emitterHeaderAgain, 0x18)),
                      reinterpret_cast<uint32_t*>(innerLockBytes),
                      2u,
                      0x40000000);

    // Batch context block: 5 dwords (ebp-0x24 .. ebp-0x14 inclusive of the flag byte).
    alignas(8) uint32_t batchContext[5];
    batchContext[0] = a3[0];
    batchContext[1] = a3[1];
    batchContext[2] = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(outerLockBytes));
    batchContext[3] = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(innerLockBytes));
    CSF_AT(uint8_t, batchContext, 0x10) = emitterFlagBit3 ? 1 : 0;

    RpPrtStdEmitterForAllParticleBatch(a2,
                                       ParticleBatchCallback(),
                                       static_cast<int>(reinterpret_cast<uintptr_t>(batchContext)));

    uint32_t* emitterHeaderFinal = reinterpret_cast<uint32_t*>(CSF_AT(uint32_t, ownerObj, 0x10));
    RpPTankAtomicUnlock(static_cast<int>(CSF_AT(uint32_t, emitterHeaderFinal, 0x18)));

    return a2;
}

HOOK(0x00704C30, GfxParticle_RenderEmitterBatches, "auto");
