#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static int __fastcall Projection_006f55d0(C_008627d8* self, void*, int a, int b) {
    int out = -1;
    using Call = int (__thiscall*)(C_008627d8*, int, int*, int);
    return reinterpret_cast<Call>(reinterpret_cast<void**>(self->vtbl)[0x2c/4])(self, a, &out, b);
}
HOOK(0x006F55D0, Projection_006f55d0, "auto");
