#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Forwards to the child object in field_0004 via its vtable slot 4 (byte offset 0x10),
// which takes (object, int* first, int* second) with __stdcall, and returns the first out value.
static int __fastcall C_00863cd4_GetFirstOutputValue(C_00863cd4* self, void* /*edx*/) {
    int v3 = 0;
    int v2 = 0;

    void* child = reinterpret_cast<void*>(self->field_0004);
    reinterpret_cast<void(__stdcall*)(void*, int*, int*)>(CSF_AT(void**, child, 0)[4])(child, &v3, &v2);

    return v3;
}

HOOK(0x711270, C_00863cd4_GetFirstOutputValue, "auto");
