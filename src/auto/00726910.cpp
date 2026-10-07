#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// 0x00726910: obtains a child object via virtual slot 0x58 (22) on this,
// and, if present, forwards a2 to virtual slot 0x24 (9) on that child.
// Returns the child pointer (as an int), or 0 when none was produced.
static int __fastcall C_008655e8_GetChildAndDispatch(C_008655e8* self, void* /*edx*/, int a2)
{
    int child = ((int (__fastcall*)(void*, void*))self->vtbl[0x58 / 4])(self, nullptr);
    if (child)
    {
        void** childVtbl = CSF_AT(void**, child, 0);
        ((void (__fastcall*)(void*, void*, int))childVtbl[0x24 / 4])(
            reinterpret_cast<void*>(child), nullptr, a2);
    }
    return child;
}

HOOK(0x00726910, C_008655e8_GetChildAndDispatch, "auto");
