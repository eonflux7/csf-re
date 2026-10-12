#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"

CSF_FUNC(int, __cdecl, RpPrtStdAtomicGetEmitter, 0x0057C290, (int a1));
CSF_FUNC(int, __cdecl, RpPrtStdEmitterForAllParticleBatch, 0x00580160,
         (int a1, int (__cdecl* a2)(int, uint32_t*, int), int a3));
CSF_FUNC(int, __cdecl, RpPTankAtomicLock, 0x005821D0, (int a1, uint32_t* a2, unsigned int a3, int a4));
CSF_FUNC(int, __cdecl, RpPTankAtomicUnlock, 0x005823E0, (int a1));
CSF_FUNC(void, __fastcall, sub_6F35C0, 0x006F35C0, (void* self, void* edx, int a2));

// Per-batch callback passed to RpPrtStdEmitterForAllParticleBatch (sub_703980 in the original).
using ParticleBatchCallback = int (__cdecl*)(int, uint32_t*, int);

static int __cdecl GfxParticle_RenderEmitterBatches(int a1, int /*a2*/)
{
    int emitter = RpPrtStdAtomicGetEmitter(a1);

    // Scratch object filled by sub_6F35C0; its +0x10 dword holds a tank pointer.
    alignas(8) uint8_t emitterInfoBytes[0x28];
    sub_6F35C0(emitterInfoBytes, nullptr, emitter);

    uint8_t* tankBase = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(CSF_AT(uint32_t, emitterInfoBytes, 0x10)));

    uint32_t lockBuffer[2];
    RpPTankAtomicLock(static_cast<int>(CSF_AT(uint32_t, tankBase, 0x18)), lockBuffer, 2, 0x40000000);

    uint32_t* bufferPtr = lockBuffer;
    RpPrtStdEmitterForAllParticleBatch(
        emitter,
        reinterpret_cast<ParticleBatchCallback>(CSF_ADDR_FN(0x703980)),
        static_cast<int>(reinterpret_cast<uintptr_t>(&bufferPtr)));

    return RpPTankAtomicUnlock(static_cast<int>(CSF_AT(uint32_t, tankBase, 0x18)));
}

HOOK(0x704370, GfxParticle_RenderEmitterBatches, "auto");
