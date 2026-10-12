#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __cdecl, RwRasterGetNumLevels, 0x006276A0, (int a1));
CSF_FUNC(int, __cdecl, RwRasterLock, 0x00627810, (int a1, int a2, int a3));
CSF_FUNC(int, __cdecl, RwRasterSetFromImage, 0x00633040, (int a1, uint8_t* a2));
CSF_FUNC(int, __cdecl, RwRasterUnlock, 0x00627490, (int a1));

// Locks each mip level (1..levelCount-1) of the raster in EBX. For levels up to
// maxLevelToSet that have an image pointer in the caller's array, applies that image.
// Returns the number of levels.
static uint32_t __cdecl ParticleRender_SetRasterLevelImages(hook::Regs* regs, const uint32_t* args) {
    uint32_t* imageSlot = reinterpret_cast<uint32_t*>(args[0]);
    int raster = static_cast<int>(regs->ebx);
    int maxLevelToSet = static_cast<int>(args[1]);

    int levelCount = RwRasterGetNumLevels(0);
    for (int level = 1; level < levelCount; ++level, ++imageSlot) {
        if (RwRasterLock(raster, level, 1)) {
            if (level <= maxLevelToSet) {
                if (*imageSlot) {
                    RwRasterSetFromImage(raster, reinterpret_cast<uint8_t*>(*imageSlot));
                }
            }
            RwRasterUnlock(raster);
        }
    }
    return static_cast<uint32_t>(levelCount);
}

HOOK_REGS(0x00705A10, ParticleRender_SetRasterLevelImages, 0, "gfx_particle_render");
