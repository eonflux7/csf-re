#include "hook.h"
#include "types/auto_classes.h"

// Forwards a view-window query to the virtual slot at +0x3C of the object, passing
// a2, a3 and a4 through and an out-int initialized to -1 (the original's local at [ebp-4]).
static int __fastcall C_008627d8_ForwardViewWindowQuery(C_008627d8* self, void* /*edx*/, int a2, int a3, int a4) {
    int outValue = -1;
    return ((int(__fastcall*)(void*, void*, int, int, int*, int))self->vtbl[0x3C / 4])(self, nullptr, a2, a3, &outValue, a4);
}

HOOK(0x6F5600, C_008627d8_ForwardViewWindowQuery, "auto");
