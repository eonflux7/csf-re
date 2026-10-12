#include "globals.h"
#include "hook.h"

// Forwards two offsets derived from *a2 to virtual slot 5 (byte offset 0x14) of `self`.
// The vtable target is __thiscall with two stack args, so it pops them itself (ret 8).
static int __fastcall CameraProjection_QueryViewWindow(void* self, void* /*edx*/, int* a2)
{
    int base = *a2;
    return ((int(__fastcall*)(void*, void*, int, int))CSF_AT(void**, self, 0)[0x14 / 4])(
        self, nullptr, base + 48, base + 32);
}

HOOK(0x6F5D90, CameraProjection_QueryViewWindow, "auto");
