#include <cstdint>
#include "globals.h"
#include <cstddef>

#include "hook.h"
#include "core/memory.h"

// Returns the water raster size in pixels from the water object's mode field at +0x98.
// Values 1..2 select a 256-pixel raster; anything else (including <= 0) selects 512.
static int __fastcall Water_GetRasterSize(void* self, void* /*edx*/) {
    int rasterMode = CSF_AT(int, self, 0x98);
    if (rasterMode > 0 && rasterMode <= 2) {
        return 256;
    }
    return 512;
}

HOOK(0x6D59D0, Water_GetRasterSize, "auto");
