#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Reads the child object stored at self+0x14, fetches an object from its
// virtual method at vtable slot 0x54/4 (=21), then invokes that object's
// virtual method at vtable slot 0xC/4 (=3) twice: first with 0, then with 1.
static int __fastcall MenuSystem_ToggleChildFlag(void* self, void* /*edx*/)
{
    // First round: child -> slot 21 -> returned object -> slot 3 with arg 0
    void* child = CSF_AT(void*, self, 0x14);
    void** childVtbl = CSF_AT(void**, child, 0);
    void* obj = ((void* (__fastcall*)(void*, void*))childVtbl[21])(child, nullptr);

    void** objVtbl = CSF_AT(void**, obj, 0);
    ((void (__fastcall*)(void*, void*, int))objVtbl[3])(obj, nullptr, 0);

    // Second round: re-read the child (callee may have changed self+0x14),
    // repeat the lookup and call slot 3 with arg 1, returning its result.
    child = CSF_AT(void*, self, 0x14);
    childVtbl = CSF_AT(void**, child, 0);
    obj = ((void* (__fastcall*)(void*, void*))childVtbl[21])(child, nullptr);

    objVtbl = CSF_AT(void**, obj, 0);
    return ((int (__fastcall*)(void*, void*, int))objVtbl[3])(obj, nullptr, 1);
}

HOOK(0x50D300, MenuSystem_ToggleChildFlag, "auto");
