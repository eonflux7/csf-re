#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(float*, __fastcall, GfxMatrix_SetBoundsAndDefaultAxes, 0x006ABAA0, (char* self, void* , float maxX, float minX, float maxY, float minY, float maxZ, float minZ));
CSF_FUNC(bool, __fastcall, GfxMatrix_ForwardValueWithAddress, 0x006ACA20, (void* self, void* , int a2));
CSF_FUNC(int, __cdecl, GfxLights_CommitLightToWorld, 0x006D0E70, (void* a1, int a2));
CSF_FUNC(uint32_t*, __fastcall, Light_GetWorldMatrixPointer, 0x006FD330, (void* self, void* edx));

static bool __cdecl GfxLights_TryAddLightWithBounds(void* lightObject, float* bounds)
{
    if (!lightObject || !bounds)
        return false;

    // Stack-resident matrix object (0x3C bytes in the original frame).
    alignas(8) uint8_t matrixBytes[0x3C];
    char* matrix = reinterpret_cast<char*>(matrixBytes);

    GfxMatrix_SetBoundsAndDefaultAxes(matrix, nullptr,
        bounds[20], bounds[23], bounds[21], bounds[24], bounds[22], bounds[25]);

    uint32_t* worldPointer = Light_GetWorldMatrixPointer(lightObject, nullptr);
    bool added = GfxMatrix_ForwardValueWithAddress(matrix, nullptr,
        static_cast<int>(reinterpret_cast<uintptr_t>(worldPointer)));

    if (added)
        GfxLights_CommitLightToWorld(lightObject,
            static_cast<int>(reinterpret_cast<uintptr_t>(bounds)));

    return added;
}

HOOK(0x6D0F40, GfxLights_TryAddLightWithBounds, "auto");
