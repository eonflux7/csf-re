#include "hook.h"
#include "types/auto_classes.h"

// Forwards the query to the virtual slot at +0x10 of the object, passing
// a2, a3, a4 through and an out-int initialized to -1 (the original's local).
static int __fastcall C_008627d8_ForwardProjectionQuery(C_008627d8* self, void* /*edx*/, int a2, int a3, int a4) {
    int outValue = -1;
    return ((int(__fastcall*)(void*, void*, int, int, int*, int))self->vtbl[0x10 / 4])(self, nullptr, a2, a3, &outValue, a4);
}

HOOK(0x6F5540, C_008627d8_ForwardProjectionQuery, "auto");
