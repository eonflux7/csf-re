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

CSF_FUNC(int, __cdecl, FreeBlock_690F70, 0x00690F70, (void* block));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

static C_008672e4* __fastcall C_008672e4_DestroyWithOptionalFree(C_008672e4* self, void* /*edx*/, char a2)
{
    // Release the buffer owned by field_000c first, with the vtable already switched to this class.
    void* ownedBuffer = reinterpret_cast<void*>(self->field_000c);
    *reinterpret_cast<void***>(self) = CSF_VTBL(0x008672E4);
    FreeBlock_690F70(ownedBuffer);

    // Notify the attached object (field_0004), if any: its vtable slot 2 is called with ecx=obj, arg=self.
    void* attachedObject = reinterpret_cast<void*>(self->field_0004);
    if (attachedObject != nullptr)
    {
        auto notifySlot = reinterpret_cast<void (__fastcall*)(void*, void*, C_008672e4*)>(
            CSF_AT(void**, attachedObject, 0)[2]);
        notifySlot(attachedObject, nullptr, self);
    }

    // Fall back to the base-class vtable.
    *reinterpret_cast<void***>(self) = CSF_VTBL(0x008670CC);

    // Optional delete of the object storage (bit 0 of a2).
    if ((a2 & 1) != 0)
    {
        Mem_Free(self);
    }

    return self;
}

HOOK(0x737FF0, C_008672e4_DestroyWithOptionalFree, "auto");
