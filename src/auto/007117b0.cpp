#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// 0x007117B0
// Reads the object pointer stored at field_0008. If non-null, invokes the
// virtual method at vtable byte offset 0x28 (slot 10) on it, passing the object
// pointer itself as the single __stdcall stack argument, and returns its result.
// If the stored pointer is null, returns 0.
static int __fastcall C_00863d18_InvokeStoredObjectVirtual28(C_00863d18* self, void* edx)
{
    uint32_t storedObject = self->field_0008;
    if (storedObject)
    {
        // Original: mov eax,[ecx+8]; mov ecx,[eax]; push eax; call [ecx+0x28]. The object's vtable is in ecx
        // at the call, the object is the one stack argument (stdcall callee), edx is the entry value.
        void** vtbl = CSF_AT(void**, reinterpret_cast<void*>(storedObject), 0);
        return reinterpret_cast<int(__fastcall*)(void*, void*, uint32_t)>(vtbl[0x28 / 4])(
            vtbl, edx, storedObject);
    }
    return static_cast<int>(storedObject);
}

HOOK(0x7117B0, C_00863d18_InvokeStoredObjectVirtual28, "auto");
