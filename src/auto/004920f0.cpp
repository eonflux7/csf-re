#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Base class destructor (C_00833908 : C_0082d428), reached by tail jump.
CSF_FUNC(int, __fastcall, C_00833908_Destroy_48EC00, 0x0048EC00, (C_00833908* self, void*));

// Destructor of C_00833b88: restores vtables and secondary-base pointers,
// releases the object held at +0x204 (virtual slot 0, deleting flag 1),
// then tail-calls the base destructor.
static int __fastcall C_00833b88_Destructor(C_00833b88* self, void* /*edx*/)
{
    // Primary vtable and secondary-base vtable slots, in the original's store order.
    *(void***)self = CSF_VTBL(0x00833B88);
    self->field_0004 = (uint32_t)(uintptr_t)CSF_ADDR_DATA(0x00833B08);
    CSF_AT(void*, self, 0x008) = reinterpret_cast<void*>(CSF_ADDR_DATA(0x00833B04));
    self->field_000c = (uint32_t)(uintptr_t)CSF_ADDR_DATA(0x00833AF8);
    CSF_AT(void*, self, 0x010) = reinterpret_cast<void*>(CSF_ADDR_DATA(0x00833AF0));
    self->field_0014 = (uint32_t)(uintptr_t)CSF_ADDR_DATA(0x00833AEC);

    // Release the owned object at +0x204: call its slot 0 with (1), then clear the field.
    void** owned = CSF_AT(void**, self, 0x204);
    if (owned) {
        ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, owned, 0)[0])(owned, nullptr, 1);
        CSF_AT(void*, self, 0x204) = nullptr;
    }

    CSF_AT(void*, self, 0x1B4) = CSF_VTBL(0x0082D410);

    // Tail jump to base destructor (ecx = this, arguments forwarded unchanged).
    return C_00833908_Destroy_48EC00(self, nullptr);
}

HOOK(0x004920F0, C_00833b88_Destructor, "auto");
