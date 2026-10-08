#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Computes a rectangle from two anchor objects (the object at +0x34 supplies
// two position pairs) and forwards it to the object at +0x1C via vtable slot 0x208.
static int __fastcall C_008414e8_QueryRectFromAnchors(C_008414e8* self, void* /*edx*/)
{
    int32_t rect[4] = { 0, 0, 0, 0 };

    // First anchor: vtable slot 0x20 returns a pointer to an (x, y) pair.
    void* anchorObject = reinterpret_cast<void*>(self->field_0034);
    int32_t* originPair = reinterpret_cast<int32_t*>(
        ((void* (__fastcall*)(void*, void*))CSF_AT(void**, anchorObject, 0)[0x20 / 4])(anchorObject, nullptr));
    rect[0] = originPair[0];
    rect[1] = originPair[1];

    // Second anchor: vtable slot 0x28 returns a pointer to an (x, y) offset pair.
    anchorObject = reinterpret_cast<void*>(self->field_0034);
    int32_t* offsetPair = reinterpret_cast<int32_t*>(
        ((void* (__fastcall*)(void*, void*))CSF_AT(void**, anchorObject, 0)[0x28 / 4])(anchorObject, nullptr));
    rect[2] = rect[0] + offsetPair[0];
    rect[3] = rect[1] + offsetPair[1];

    // Forward the rectangle to the target object at +0x1C (vtable slot 0x208).
    void* targetObject = reinterpret_cast<void*>(self->field_001c);
    return ((int (__fastcall*)(void*, void*, int32_t*, int))CSF_AT(void**, targetObject, 0)[0x208 / 4])(
        targetObject, nullptr, rect, 0);
}

HOOK(0x0052C830, C_008414e8_QueryRectFromAnchors, "auto");
