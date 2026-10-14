#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(void, __cdecl, SceneLoader_NoOp, 0x00506AC0, ());
CSF_FUNC(uint32_t*, __fastcall, ScratchObj_CopyFromSource, 0x0068E2E0, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(uint32_t*, __fastcall, Sub48_FillScratchFromSubobject, 0x007A2A20, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(double, __cdecl, Scalar_PassThroughDouble, 0x007A2AE0, (float a1));
CSF_FUNC(double, __fastcall, __CIfmod, 0x007AFFFA, (void* self, void* edx));

CSF_GLOBAL(double, g_DegreesPerTurn, 0x834E00);

// Reads a float-sized scalar from the subobject at +0x48 and returns it reduced modulo 360 degrees.
static double __fastcall NetCogible_ReadAngleDegreesMod360(void* self, void* /*edx*/)
{
    // Stack objects: [ebp-0x1C] (0xC bytes) and [ebp-0x10] (0x10 bytes).
    alignas(8) uint8_t sourceBytes[0xC];
    alignas(8) uint8_t scratchBytes[0x10];

    Sub48_FillScratchFromSubobject(static_cast<uint8_t*>(self) + 0x48, nullptr,
                                   reinterpret_cast<uint32_t*>(scratchBytes));
    ScratchObj_CopyFromSource(sourceBytes, nullptr,
                              reinterpret_cast<uint32_t*>(scratchBytes));
    SceneLoader_NoOp();

    float firstScalar = *reinterpret_cast<float*>(sourceBytes);
    double converted = Scalar_PassThroughDouble(firstScalar);
    float angleDegrees = static_cast<float>(converted);

    SceneLoader_NoOp();

    double reduced = std::fmod(static_cast<double>(angleDegrees), g_DegreesPerTurn);
    return reduced;
}

HOOK(0x740AF0, NetCogible_ReadAngleDegreesMod360, "auto");
