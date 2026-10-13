#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Callee: GfxFrame_InitRotationDegreesFromMatrix, __fastcall-style, pops its 4-byte stack arg.
CSF_FUNC(char, __fastcall, GfxFrame_InitRotationDegreesFromMatrix, 0x006BF380, (void* self, void* /*edx*/, int matrixBase));

// The original overwrites ecx with its first stack argument (a2) before the call, so the
// callee's `self` is a2, not the object's this pointer. The stack argument is the dword at +4
// of the object referenced by this->field_000c.
static char __fastcall C_00861ce0_InitRotationFromMatrix(C_00861ce0* self, void* /*edx*/, int a2)
{
    void* matrixOwner = reinterpret_cast<void*>(self->field_000c);
    int matrixBase = CSF_AT(int, matrixOwner, 0x4);
    return GfxFrame_InitRotationDegreesFromMatrix(reinterpret_cast<void*>(a2), nullptr, matrixBase);
}

HOOK(0x6E5510, C_00861ce0_InitRotationFromMatrix, "auto");
