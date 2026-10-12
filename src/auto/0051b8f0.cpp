#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Base constructor of C_0086f128, bound by address (__thiscall, this in ecx, initialValue on stack).
CSF_FUNC(void*, __fastcall, C_0086f128_Construct, 0x00762ED0, (void* self, void* /*edx*/, int initialValue));

// Constructs the menu listener: runs the base constructor, then installs C_00840e0c's vtable.
// Returns this (eax = esi in the original).
static C_00840e0c* __fastcall GameBddLoader_ConstructMenuListener(C_00840e0c* self, void* /*edx*/, int a2)
{
    C_0086f128_Construct(self, nullptr, a2);
    *reinterpret_cast<void**>(self) = CSF_VTBL(0x00840E0C);
    return self;
}

HOOK(0x0051B8F0, GameBddLoader_ConstructMenuListener, "auto");
