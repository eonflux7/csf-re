#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GfxObjectProps_GetObjectPropertyWord, 0x006B8800, (int objectIndex));

// Maps the light's object property word (0..3) to a class code.
// Returns 3 when the property word is missing or outside 0..3.
static int __fastcall CLightObject_GetObjectPropertyClass(CLightObject* self, void* /*edx*/) {
    int defaultClass = 3;
    int lightIndex = CSF_AT(int, self, 0x0C);
    uint32_t* propertyWord = reinterpret_cast<uint32_t*>(GfxObjectProps_GetObjectPropertyWord(lightIndex));
    if (!propertyWord) {
        return defaultClass;
    }
    switch (*propertyWord) {
    case 0:
        return 1;
    case 1:
        return 2;
    case 2:
        return 0;
    case 3:
        return 3;
    default:
        return defaultClass;
    }
}

HOOK(0x006FCEF0, CLightObject_GetObjectPropertyClass, "auto");
