#include <cstdint>
#include "globals.h"
#include "hook.h"

// int __cdecl RwRasterGetWidth(int raster)  -- library routine at 0x005870D0
CSF_FUNC(int, __cdecl, RwRasterGetWidth, 0x5870D0, (int raster));

// 0x006A0000: thin __cdecl thunk that tail-jumps into RwRasterGetWidth.
static int __cdecl GetRasterWidthThunk(int raster)
{
    return RwRasterGetWidth(raster);
}

HOOK(0x6A0000, GetRasterWidthThunk, "auto");
