#include "hook.h"
#include "types/auto_classes.h"

// Forwards a clip/projection query to the virtual slot at +0x34 of the object.
// Passes a2, a3 and a4 through, plus an out-int initialized to -1 (the original's local at [ebp-4]).
static int __fastcall C_008627d8_ForwardClipQuery(C_008627d8* self, void* /*edx*/, int a2, int a3, int a4) {
    int outValue = -1;
    return ((int(__fastcall*)(void*, void*, int, int, int*, int))self->vtbl[0x34 / 4])(self, nullptr, a2, a3, &outValue, a4);
}

HOOK(0x6F55A0, C_008627d8_ForwardClipQuery, "auto");
