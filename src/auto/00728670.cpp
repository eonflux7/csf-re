#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Base helper called at the start of the reset (this-pointer in ecx, no stack args).
CSF_FUNC(int, __fastcall, C_00865870_ResetBase, 0x713EA0, (int* self, void* /*edx*/));

// Resets the derived state: clears the float at +0x44 (written as a dword 0) and the
// byte flag at +0x48, after letting the base reset run. Always returns 0.
static int __fastcall C_00865870_ClearState(C_00865870* self, void* /*edx*/)
{
    C_00865870_ResetBase(reinterpret_cast<int*>(self), nullptr);

    // dword store of 0 into the float slot (eax == 0)
    *reinterpret_cast<uint32_t*>(&self->field_0044) = 0;
    self->field_0048 = 0;
    return 0;
}

HOOK(0x728670, C_00865870_ClearState, "auto");
