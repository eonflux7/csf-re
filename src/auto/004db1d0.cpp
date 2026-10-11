#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Virtual slot 32 on TFncGirar's vtable: returns a type id for the given argument.
// Original: call [edx+0x80] with (this, a2) -> eax.
// Virtual slot 31 on the target object's vtable: consumes the type id.
// Original: call [edi+0x7c] with ecx = target, one stack arg = eax.
static void __fastcall TFncGirar_OnRotationComplete(TFncGirar* self, void* /*edx*/, int a2)
{
    void* target = CSF_AT(void*, self, 0x0C);
    if (target != nullptr)
    {
        void** targetVtbl = CSF_AT(void**, target, 0);
        int typeId = ((int(__fastcall*)(void*, void*, int))CSF_AT(void**, self, 0)[32])(self, nullptr, a2);
        ((void(__fastcall*)(void*, void*, int))targetVtbl[31])(target, nullptr, typeId);
    }
}

HOOK(0x004DB1D0, TFncGirar_OnRotationComplete, "auto");
