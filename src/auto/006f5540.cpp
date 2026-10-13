#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static int __fastcall Projection_006f5540(C_008627d8* self, void*, int a, int b, int c) {
    int out = -1;
    using Call = int (__thiscall*)(C_008627d8*, int, int, int*, int);
    return reinterpret_cast<Call>(reinterpret_cast<void**>(self->vtbl)[16/4])(self, a, b, &out, c);
}
HOOK(0x006F5540, Projection_006f5540, "auto");
