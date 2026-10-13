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

// Releases the object in field_000c and every object in the pointer array at
// field_0010 (count field_0018), calling each one's slot 1 with flag 1, then
// clears the slots and the count. Returns the last eax value the original leaves.
static int __fastcall C_00863180_ReleaseEmitterObjects(C_00863180* self, void* /*edx*/)
{
    uint32_t index = 0;
    int result = 0;

    void* singleObject = reinterpret_cast<void*>(self->field_000c);
    if (singleObject != nullptr)
    {
        result = ((int(__fastcall*)(void*, void*, int))CSF_AT(void**, singleObject, 0)[1])(singleObject, nullptr, 1);
        self->field_000c = 0;
    }

    const uint32_t count = self->field_0018;
    if (count != 0)
    {
        do
        {
            void* element = reinterpret_cast<void*>(reinterpret_cast<uint32_t*>(self->field_0010)[index]);
            if (element != nullptr)
            {
                ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, element, 0)[1])(element, nullptr, 1);
                result = static_cast<int>(self->field_0010);
                reinterpret_cast<uint32_t*>(self->field_0010)[index] = 0;
            }
            ++index;
        } while (index < count);
    }

    self->field_0018 = 0;
    return result;
}

HOOK(0x007028C0, C_00863180_ReleaseEmitterObjects, "auto");
