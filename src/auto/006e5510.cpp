#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Callee: GfxFrame_InitRotationDegreesFromMatrix (0x6BF380), __fastcall, ret 4.
CSF_FUNC(char, __fastcall, GfxFrame_InitRotationDegreesFromMatrix, 0x006BF380, (void* self, void* , int matrixBase));

// __thiscall method, ret 4 (one stack arg a2, unused by the original).
static char __fastcall C_00861ce0_InitRotationFromMatrix(C_00861ce0* self, void* /*edx*/, int /*a2*/) {
    // Read the dword at +4 of the object referenced by field_000c.
    int matrixBase = CSF_AT(int, reinterpret_cast<void*>(self->field_000c), 4);
    return GfxFrame_InitRotationDegreesFromMatrix(self, nullptr, matrixBase);
}

HOOK(0x006E5510, C_00861ce0_InitRotationFromMatrix, "auto");
