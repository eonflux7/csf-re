#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Virtual slot 32 on the task's own vtable: maps the argument a3 to a type id.
// Original: call [edx+0x80] with (this, a3) -> eax.
// Virtual slot 31 on the child object's vtable: consumes the type id.
// Original: call [edi+0x7c] with ecx = child, one stack arg = eax.
// a2 is pushed by the caller and popped by ret 8 but never read.
static void __fastcall TCharTask_ForwardTypeIdToChild(TCharTask* self, void* /*edx*/, int a2, int a3)
{
    void* child = CSF_AT(void*, self, 0x0C);
    if (child != nullptr)
    {
        void** childVtbl = CSF_AT(void**, child, 0);
        int typeId = ((int(__fastcall*)(void*, void*, int))CSF_AT(void**, self, 0)[32])(self, nullptr, a3);
        ((void(__fastcall*)(void*, void*, int))childVtbl[31])(child, nullptr, typeId);
    }
}

HOOK(0x004DB330, TCharTask_ForwardTypeIdToChild, "auto");
