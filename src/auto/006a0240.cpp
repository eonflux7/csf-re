#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RwRasterGetWidth, 0x005870D0, (int raster));

static int __fastcall C_0085ec18_GetRasterWidth(C_0085ec18* self, void* /*edx*/) {
    if (self->field_0004) {
        return RwRasterGetWidth((int)self->field_0004);
    }
    return 0;
}

HOOK(0x006A0240, C_0085ec18_GetRasterWidth, "auto");
