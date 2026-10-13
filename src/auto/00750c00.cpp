#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"

// Calls vtable slot 0 (byte offset 0) of the object with argument 1 when the
// object pointer is non-null. Returns the slot's result, or 0 for a null object
// (the original returns an uninitialized eax there; 0 is the garbage-free choice).
static int __stdcall NetCogible_ReleaseManagedObject(int managedObject)
{
    if (managedObject != 0) {
        void* obj = reinterpret_cast<void*>(managedObject);
        return ((int(__fastcall*)(void*, void*, int))CSF_AT(void**, obj, 0)[0])(obj, nullptr, 1);
    }
    return 0;
}

HOOK(0x750C00, NetCogible_ReleaseManagedObject, "auto");
