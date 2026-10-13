#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, GfxPtank_TestBoundaryFace, 0x006D7450, (int* self, void*, int faceIndex, float* point));

static char __fastcall TextDatabase_TestHorizontalPosition(S_00770760* self, void* /*edx*/, float a2, float a3)
{
    // Original reads the count once, before the loop.
    uint32_t faceCount = self->field_0048;

    // Point (x, y) stored on the stack, passed by pointer to each face test.
    float point[2];
    point[0] = a2;
    point[1] = a3;

    char result = 0;
    uint32_t faceIndex = 0;
    while (faceIndex < faceCount) {
        if (result)
            break;
        // Sub-object embedded at +0x30 is the `this` for the face test.
        int* faceSet = reinterpret_cast<int*>(reinterpret_cast<uint8_t*>(self) + 0x30);
        result = GfxPtank_TestBoundaryFace(faceSet, nullptr, static_cast<int>(faceIndex), point);
        faceIndex++;
    }
    return result;
}

HOOK(0x00770760, TextDatabase_TestHorizontalPosition, "auto");
