#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, GfxFrame_InitRotationDegreesFromMatrix, 0x006BF380, (void* self, void* , int matrixBase));
CSF_FUNC(int, __cdecl, RwFrameForAllChildren, 0x006237C0, (int a1, int (__cdecl* a2)(int, int), int a3));

static char __fastcall C_00861dd8_InitializeTrackEntry(
    void* self, void* /*edx*/, void* matrix, int trackIndex)
{
    if (!matrix || trackIndex < 0)
        return 0;

    GfxFrame_InitRotationDegreesFromMatrix(
        static_cast<uint8_t*>(self) + 0x0C,
        nullptr,
        reinterpret_cast<int>(matrix));

    int rootFrame = CSF_AT(int, self, 0x18);
    CSF_AT(int, self, 0x04) = trackIndex;

    uint8_t* childArg = static_cast<uint8_t*>(self) + 0x2C;
    RwFrameForAllChildren(
        rootFrame,
        reinterpret_cast<int (__cdecl*)(int, int)>(CSF_ADDR_FN(0x006E7EF0)),
        reinterpret_cast<int>(&childArg));

    return 1;
}

HOOK(0x006E7F90, C_00861dd8_InitializeTrackEntry, "auto");
