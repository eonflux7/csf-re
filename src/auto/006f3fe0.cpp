#include <cstdint>
#include "globals.h"
#include <cstring>

#include "hook.h"

CSF_FUNC(int, __cdecl, RpPrtStdAtomicGetEmitter, 0x0057C290, (int a1));
CSF_FUNC(void, __fastcall, GfxPtank_ComputeEmitterPlacement, 0x006F35C0, (void* self, void* edx, int a2));

// Returns the placement state of the particle tank (10 dwords) into the caller's buffer.
static void* __fastcall GfxPtank_GetPlacementState(void* self, void* /*edx*/, void* outState)
{
    // Emitter handle lives at +0x24 of the tank object.
    int emitter = RpPrtStdAtomicGetEmitter(CSF_AT(int, self, 0x24));

    alignas(8) uint8_t placementBytes[0x28];
    GfxPtank_ComputeEmitterPlacement(placementBytes, nullptr, emitter);

    std::memcpy(outState, placementBytes, 0x28);
    return outState;
}

HOOK(0x006F3FE0, GfxPtank_GetPlacementState, "auto");
