#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Destructor for C_0085e7d0: resets the vtable, then if the owned object at +0x14
// is non-null, calls its virtual slot 0 with argument 1 and clears the pointer.
static int __fastcall C_0085e7d0_Destroy(C_0085e7d0* self, void* /*edx*/)
{
    void* ownedObject = reinterpret_cast<void*>(self->field_0014);
    self->vtbl = CSF_VTBL(0x0085E7D0);

    int result = 0;
    if (ownedObject)
    {
        result = ((int(__fastcall*)(void*, void*, int))CSF_AT(void**, ownedObject, 0)[0])(ownedObject, nullptr, 1);
        self->field_0014 = 0;
    }
    return result;
}

HOOK(0x0069AE90, C_0085e7d0_Destroy, "auto");
