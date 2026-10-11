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

CSF_FUNC(uint32_t*, __fastcall, sub_7A2A20, 0x007A2A20, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(uint32_t*, __fastcall, sub_68E2E0, 0x0068E2E0, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(void, __cdecl, SceneLoader_NoOp, 0x00506AC0, ());
CSF_FUNC(double, __cdecl, sub_7A2AE0, 0x007A2AE0, (float a1));

// The 360.0 divisor the original loads with `fld qword ptr [0x834E00]`.
CSF_GLOBAL(double, g_FullTurnDegrees, 0x00834E00);

// Address of the CRT fmod helper (__CIfmod) the original calls with the operands on the x87 stack.

// Reads a float from a temporary record filled from the sub-object at self+0x48,
// passes it through sub_7A2AE0 and returns it modulo 360.0.
static double __fastcall NetCogibleDDL_GetWrappedAngleDegrees(void* self, void* /*edx*/) {
    alignas(8) uint8_t recordBuf[0xC];   // [ebp-0x1C] object passed as `this` to sub_68E2E0
    alignas(8) uint8_t scratchBuf[0x10]; // [ebp-0x10] out argument shared by both fillers

    void* member = static_cast<uint8_t*>(self) + 0x48;
    sub_7A2A20(member, nullptr, reinterpret_cast<uint32_t*>(scratchBuf));
    sub_68E2E0(recordBuf, nullptr, reinterpret_cast<uint32_t*>(scratchBuf));
    SceneLoader_NoOp();

    float storedValue = CSF_AT(float, recordBuf, 4);
    float angleValue = (float)sub_7A2AE0(storedValue);

    SceneLoader_NoOp();

    // fmod(angleValue, 360.0): the original pushes both operands on the x87 stack
    // and calls __CIfmod (s_CrtFmodHelper); the C++ equivalent is the plain fmod.
    double wrapped = std::fmod((double)angleValue, g_FullTurnDegrees);
    return wrapped;
}

HOOK(0x740AA0, NetCogibleDDL_GetWrappedAngleDegrees, "auto");
