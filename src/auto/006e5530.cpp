#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, _rpAtomicResyncInterpolatedSphere, 0x0058EB40, (int a1));

static int __fastcall C_00861ce0_ResyncSphereAndGetSphereAddress(C_00861ce0* self, void* /*edx*/)
{
    uint8_t* atomic = reinterpret_cast<uint8_t*>(self->field_000c);
    if ((CSF_AT(uint8_t, atomic, 0x50) & 2) != 0)
    {
        _rpAtomicResyncInterpolatedSphere(static_cast<int>(self->field_000c));
    }
    return static_cast<int>(self->field_000c + 28);
}

HOOK(0x006E5530, C_00861ce0_ResyncSphereAndGetSphereAddress, "auto");
