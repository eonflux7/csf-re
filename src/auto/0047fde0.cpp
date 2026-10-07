#include <cstdint>
#include "types/auto_classes.h"
#include "hook.h"

// Forwards to the virtual method at vtable offset 0x1E8 and returns its int result.
// The single stack argument (a2) is not used by the callee but must still be popped.
static int __fastcall CallVirtualMethod1E8(C_00833228* self, void* /*edx*/, int /*a2*/)
{
    return ((int(__fastcall*)(void*, void*))self->vtbl[0x1E8 / 4])(self, nullptr);
}

HOOK(0x0047FDE0, CallVirtualMethod1E8, "auto");
