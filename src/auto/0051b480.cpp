#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// CWeaponDefTable base constructor (__fastcall, self in ecx, one stack arg `owner`).
CSF_FUNC(void*, __fastcall, CWeaponDefTable_Construct, 0x007844B0, (void* self, void* /*edx*/, int owner));

// Constructs a CWeaponDatabase: runs the base-class constructor, installs the
// derived vtable, and zeroes the six array bookkeeping pointers.
static void* __fastcall CWeaponDatabase_Construct(void* self, void* /*edx*/, int owner)
{
    CWeaponDefTable_Construct(self, nullptr, owner);
    *reinterpret_cast<void**>(self) = CSF_VTBL(0x00840D88);
    CSF_AT(uint32_t, self, 0x10) = 0;
    CSF_AT(uint32_t, self, 0x14) = 0;
    CSF_AT(uint32_t, self, 0x18) = 0;
    CSF_AT(uint32_t, self, 0x20) = 0;
    CSF_AT(uint32_t, self, 0x24) = 0;
    CSF_AT(uint32_t, self, 0x28) = 0;
    return self;
}

HOOK(0x0051B480, CWeaponDatabase_Construct, "auto");
