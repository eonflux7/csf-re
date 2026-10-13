#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(uint32_t*, __fastcall, ScratchObj_CopyFromSource, 0x0068E2E0, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(uint32_t*, __fastcall, ScratchObj_Reset, 0x0068E430, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(void, __cdecl, SceneLoader_NoOp, 0x00506AC0, ());

// Returns the player's X position as a float in st0 (the original uses fld dword ptr).
static float __fastcall GetPlayerPositionX(void* self, void* /*edx*/)
{
    const bool hasCachedPosition = CSF_AT(uint8_t, self, 0x7FC) != 0;
    bool needsCleanup = false;
    float positionX = 0.0f;

    // Scratch frame: [0..0xC) is the scratch object, [0xC..0x1C) is the field passed by pointer.
    alignas(8) uint8_t scratchFrame[0x1C];
    uint8_t* scratchObj = scratchFrame;
    uint8_t* scratchField = scratchFrame + 0xC;

    if (hasCachedPosition) {
        positionX = CSF_AT(float, self, 0x800);
    } else {
        needsCleanup = true;
        ScratchObj_Reset(static_cast<uint8_t*>(self) + 0x1B8, nullptr,
                         reinterpret_cast<uint32_t*>(scratchField));
        ScratchObj_CopyFromSource(scratchObj, nullptr,
                                  reinterpret_cast<uint32_t*>(scratchField));
        SceneLoader_NoOp();
        positionX = CSF_AT(float, scratchObj, 0);
    }

    if (needsCleanup) {
        SceneLoader_NoOp();
    }
    return positionX;
}

HOOK(0x004D1AE0, GetPlayerPositionX, "auto");
