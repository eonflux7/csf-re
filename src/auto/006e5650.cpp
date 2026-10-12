#include <cstdint>
#include "globals.h"
#include "hook.h"

// Callee: GfxEnvironment_SetSectorObjectFlag4 (__cdecl, returns char)
CSF_FUNC(char, __cdecl, GfxEnvironment_SetSectorObjectFlag4, 0x006C0160, (int a1, char a2));

// Forwards a flag to GfxEnvironment_SetSectorObjectFlag4, using the object
// stored at +0x0C of this as the first argument.
static char __fastcall GfxObjectInstances_SetSectorFlag4ViaOwner(void* self, void* /*edx*/, char flagValue)
{
    int ownerPointer = static_cast<int>(CSF_AT(uint32_t, self, 0x0C));
    return GfxEnvironment_SetSectorObjectFlag4(ownerPointer, flagValue);
}

HOOK(0x6E5650, GfxObjectInstances_SetSectorFlag4ViaOwner, "auto");
