#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

// Returns 0 when the sphere's radius (float at +0x0C) is exactly zero; otherwise forwards the
// query to the object's virtual sphere-intersection method (vtable slot 0x2C).
static int __fastcall C_00862028_QuerySphereIfRadiusNonZero(C_00862028* self, void* /*edx*/, int sphereArg, int extraArg) {
    float radius = CSF_AT(float, reinterpret_cast<void*>(sphereArg), 0x0C);
    if (radius == g_fZero) {
        return 0;
    }

    int sphereForCall = sphereArg;
    int outSphereSlot = -1;
    return ((int(__fastcall*)(void*, void*, int, int*, int))(CSF_AT(void**, self, 0)[0x2C / 4]))(
        self, nullptr, sphereForCall, &outSphereSlot, extraArg);
}

HOOK(0x006EC010, C_00862028_QuerySphereIfRadiusNonZero, "auto");
