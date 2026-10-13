#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Forwards a value obtained from this object's field query (slot 0x28, called with
// (a3, 0)) into a virtual method on the object passed as a2 (slot 0xA0, one argument).
// Stack args: a2 (int*), a3 (int). Callee pops 8.
static int __fastcall C_00833908_ForwardFieldValueToObject(C_00833908* self, void* /*edx*/, int* a2, int a3)
{
    // The original keeps the first stack argument in esi and loads its vtable into edi
    // before making any calls; the vtable slot reads below use that same object.
    // Slot 0x28 / 4 = 10: this->vtbl[10](a3, 0) with ecx = self.
    int fieldValue = ((int(__fastcall*)(void*, void*, int, int))CSF_AT(void**, self, 0)[10])(
        self, nullptr, a3, 0);

    // Slot 0xA0 / 4 = 40: a2->vtbl[40](fieldValue) with ecx = a2.
    return ((int(__fastcall*)(void*, void*, int))CSF_AT(void**, a2, 0)[40])(
        a2, nullptr, fieldValue);
}

HOOK(0x0048E1D0, C_00833908_ForwardFieldValueToObject, "auto");
