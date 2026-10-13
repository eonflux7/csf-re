#include "globals.h"
#include "hook.h"

CSF_GLOBAL(int, g_008bb400, 0x008BB400);
CSF_FUNC(int, __cdecl, RwRasterGetWidth, 0x005870D0, (int raster));

static int __cdecl CoreFilesystemRtfs_GetDefaultRasterWidth()
{
    if (g_008bb400)
        return RwRasterGetWidth(g_008bb400);
    else
        return 0;
}

HOOK(0x69F9F0, CoreFilesystemRtfs_GetDefaultRasterWidth, "auto");
