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

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(int, __fastcall, ReleaseObjectReference, 0x006923C0, (void* self, void* edx));

// Releases every referenced child object in the slot array, then drops the
// virtual-owned sub-object and the slot array itself.
static int __fastcall C_00830d68_Destroy(C_00830d68* self, void* /*edx*/)
{
    self->vtbl = CSF_VTBL(0x00830D68);

    uint32_t slotIndex = 0;
    if (self->field_0014)
    {
        do
        {
            uint32_t entry = reinterpret_cast<uint32_t*>(self->field_000c)[slotIndex];
            if (entry)
            {
                ReleaseObjectReference(reinterpret_cast<void*>(entry + 4), nullptr);
                reinterpret_cast<uint32_t*>(self->field_000c)[slotIndex] = 0;
            }
            ++slotIndex;
        } while (slotIndex < self->field_0014);
    }

    void* ownedObject = reinterpret_cast<void*>(self->field_001c);
    if (ownedObject)
    {
        ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, ownedObject, 0)[0])(ownedObject, nullptr, 1);
        self->field_001c = 0;
    }

    int result = static_cast<int>(self->field_000c);
    if (result)
    {
        result = Mem_Free(reinterpret_cast<void*>(self->field_000c));
        self->field_000c = 0;
    }
    return result;
}

HOOK(0x0043ABF0, C_00830d68_Destroy, "auto");
