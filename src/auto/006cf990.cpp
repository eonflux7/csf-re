#include <cstdint>
#include "globals.h"
#include "hook.h"

// Callee: __stricmp (cdecl), returns 0 when the strings match ignoring case.
CSF_FUNC(int, __cdecl, __stricmp, 0x007B0C41, (const char* left, const char* right));

// Maps a shape name ("SPHERE", <data-string at 0x82DBE8>, "ELLIPSOID") to a shape kind.
// Returns 0 for SPHERE, 2 for the string at 0x82DBE8, 1 for ELLIPSOID, -1 otherwise
// (and for a null name).
static int __stdcall CollisionShapes_ParseShapeType(char* shapeName)
{
    int shapeKind = -1;
    if (!shapeName)
        return shapeKind;

    if (__stricmp(shapeName, CSF_STR(0x0082DBD4, "SPHERE")) == 0)
    {
        shapeKind = 0;
    }
    else if (__stricmp(shapeName, (const char*)CSF_ADDR_DATA(0x0082DBE8)) == 0)
    {
        shapeKind = 2;
    }

    if (__stricmp(shapeName, CSF_STR(0x00860D84, "ELLIPSOID")) == 0)
        return 1;
    return shapeKind;
}

HOOK(0x006CF990, CollisionShapes_ParseShapeType, "auto");
