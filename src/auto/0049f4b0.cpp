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

CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));
CSF_FUNC(int, __fastcall, C_00833908_Destroy_48EC00, 0x0048EC00, (C_00833908* self, void*));

// Destroys a C_008342e8 object: resets its vtables and sub-object pointers, releases the
// array of 28-byte elements at field_0198 (each element's first dword set to C_0082d410's
// vtable), then chains to the base-class destructor.
static int __fastcall C_008342e8_Destroy(C_008342e8* self, void* /*edx*/) {
    *reinterpret_cast<void***>(self) = CSF_VTBL(0x008342E8);
    self->field_0004 = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(CSF_VTBL(0x00834268)));
    CSF_AT(uint32_t, self, 0x08) = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(CSF_VTBL(0x00834260)));
    self->field_000c = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(CSF_VTBL(0x00834254)));
    CSF_AT(uint32_t, self, 0x10) = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(CSF_VTBL(0x0083424C)));
    self->field_0014 = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(CSF_VTBL(0x00834248)));

    uint32_t arrayHandle = self->field_0198;
    if (arrayHandle != 0) {
        uint8_t* elementsBase = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(arrayHandle));
        int32_t elementCount = *reinterpret_cast<int32_t*>(elementsBase - 4);
        uint8_t* elementCursor = elementsBase + 28 * elementCount;
        for (int32_t i = elementCount; i > 0; --i) {
            elementCursor -= 28;
            *reinterpret_cast<void***>(elementCursor) = CSF_VTBL(0x0082D410);
        }
        CoreMemory_Free(elementsBase - 4);
        self->field_0198 = 0;
    }
    return C_00833908_Destroy_48EC00(static_cast<C_00833908*>(self), nullptr);
}

HOOK(0x0049F4B0, C_008342e8_Destroy, "auto");
