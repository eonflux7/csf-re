#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, RpPrtStdAtomicGetEmitter, 0x0057C290, (int a1));
CSF_FUNC(int, __cdecl, RpPTankAtomicLock, 0x005821D0, (int a1, uint32_t* a2, unsigned int a3, int a4));
CSF_FUNC(int, __cdecl, RpPTankAtomicUnlock, 0x005823E0, (int a1));
CSF_FUNC(void, __fastcall, GfxPrtStdParticles_BindEmitterContext, 0x006F35C0, (void* self, void* edx, int a2));

static int __fastcall GfxPrtStdParticles_TranslateParticles(
    void* self, void* /*edx*/, float* translation)
{
    alignas(8) uint8_t contextBytes[0x28];
    alignas(8) uint8_t lockBytes[0x8];

    int emitter = RpPrtStdAtomicGetEmitter(CSF_AT(int, self, 0x24));
    GfxPrtStdParticles_BindEmitterContext(contextBytes, nullptr, emitter);

    void* particleData = CSF_AT(void*, contextBytes, 0x10);
    bool usesMatrices = (CSF_AT(uint32_t, particleData, 0) & 8) != 0;

    RpPTankAtomicLock(
        CSF_AT(int, particleData, 0x18),
        reinterpret_cast<uint32_t*>(lockBytes),
        usesMatrices ? 8 : 1,
        0x40000000);

    float*& current = CSF_AT(float*, lockBytes, 0);
    unsigned int index = 0;
    if (CSF_AT(uint32_t, particleData, 0x8))
    {
        do
        {
            if (usesMatrices)
            {
                CSF_AT(float, current, 0x30) =
                    translation[0] + CSF_AT(float, current, 0x30);
                CSF_AT(float, current, 0x34) =
                    translation[1] + CSF_AT(float, current, 0x34);
                CSF_AT(float, current, 0x38) =
                    translation[2] + CSF_AT(float, current, 0x38);
            }
            else
            {
                CSF_AT(float, current, 0) =
                    CSF_AT(float, current, 0) + translation[0];
                CSF_AT(float, current, 0x4) =
                    CSF_AT(float, current, 0x4) + translation[1];
                CSF_AT(float, current, 0x8) =
                    CSF_AT(float, current, 0x8) + translation[2];
            }

            current = reinterpret_cast<float*>(
                reinterpret_cast<uint8_t*>(current) +
                CSF_AT(int, lockBytes, 0x4));
            ++index;
        }
        while (index < CSF_AT(uint32_t, particleData, 0x8));
    }

    return RpPTankAtomicUnlock(CSF_AT(int, particleData, 0x18));
}

HOOK(0x006F44C0, GfxPrtStdParticles_TranslateParticles, "auto");
