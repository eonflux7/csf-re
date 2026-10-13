#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void**, __fastcall, C_008680a0_ResetInnerHolder, 0x00749150, (void* self, void* edx));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(void, __fastcall, C_00867890_ResetVtableAndClearField4, 0x00740840, (C_00867890* self, void*));
CSF_FUNC(int, __fastcall, BaseDestructor_7A2DE0, 0x007A2DE0, (void* self, void* edx));

// Destructor body of C_008680a0: resets the inner holder at +0x45C, frees its
// debug block at +0x460, resets the C_00867890 subobject at +0x440, restores the
// base vtable and tail-calls the base destructor.
static int __fastcall C_008680a0_Destroy(C_008680a0* self, void* /*edx*/)
{
    CSF_AT(void**, self, 0x0) = CSF_VTBL(0x008680A0);
    CSF_AT(void**, self, 0x440) = CSF_VTBL(0x00868090);

    uint8_t* selfBytes = reinterpret_cast<uint8_t*>(self);
    C_008680a0_ResetInnerHolder(selfBytes + 0x45C, nullptr);

    void* heldBlock = CSF_AT(void*, self, 0x460);
    if (heldBlock)
        Mem_FreeDebug(heldBlock);
    CSF_AT(void*, self, 0x460) = nullptr;

    C_00867890_ResetVtableAndClearField4(reinterpret_cast<C_00867890*>(selfBytes + 0x440), nullptr);

    CSF_AT(void**, self, 0x0) = CSF_VTBL(0x00868050);
    return BaseDestructor_7A2DE0(self, nullptr);
}

HOOK(0x00749320, C_008680a0_Destroy, "auto");
