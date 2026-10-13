#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Releases every child slot held by this interpolator (calling child slot 1 with
// argument 1 on each non-null entry and nulling the entry), then resets the
// blend weight field to 1.0. Returns the slot-list count when children were
// present, otherwise the slot-list pointer (0 when no slot array exists).
static int __fastcall C_00863430_ReleaseChildrenAndResetWeight(C_00863430* self, void* /*edx*/)
{
    int result = (int)self->field_000c;
    if (result != 0)
    {
        result = (int)self->field_0004;
        int index = 0;
        while (index < CSF_AT(int, reinterpret_cast<void*>(self->field_0004), 4))
        {
            void* child = reinterpret_cast<void*>(reinterpret_cast<uint32_t*>(self->field_000c)[index]);
            if (child)
            {
                ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, child, 0)[1])(child, nullptr, 1);
                reinterpret_cast<uint32_t*>(self->field_000c)[index] = 0;
            }
            result = CSF_AT(int, reinterpret_cast<void*>(self->field_0004), 4);
            ++index;
        }
    }
    self->field_0010 = 1.0f;
    return result;
}

HOOK(0x708490, C_00863430_ReleaseChildrenAndResetWeight, "auto");
