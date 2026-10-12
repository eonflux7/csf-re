#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(void, __cdecl, _free, 0x007B17CC, (void* Block));

static void __fastcall S_004713e0_ReleaseOwnedStorage(
    S_004713e0* self, void* /*edx*/)
{
    for (uint32_t i = 0; i < self->field_0034; ++i)
    {
        auto* child = reinterpret_cast<S_004713e0*>(
            reinterpret_cast<uint32_t*>(self->field_002c)[i]);

        if (child)
        {
            if (child->field_0000)
            {
                if (child->field_0028)
                {
                    _free(reinterpret_cast<void*>(child->field_0004));
                }
                else if (child->field_0004)
                {
                    Mem_Free(reinterpret_cast<void*>(child->field_0004));
                    child->field_0004 = 0;
                }
            }

            Mem_Free(child);
            reinterpret_cast<uint32_t*>(self->field_002c)[i] = 0;
        }
    }

    if (self->field_002c)
    {
        Mem_Free(reinterpret_cast<void*>(self->field_002c));
        self->field_002c = 0;
    }

    if (self->field_0000)
    {
        if (self->field_0028)
        {
            _free(reinterpret_cast<void*>(self->field_0004));
        }
        else if (self->field_0004)
        {
            Mem_Free(reinterpret_cast<void*>(self->field_0004));
            self->field_0004 = 0;
        }
    }
}

HOOK(0x0069B3B0, S_004713e0_ReleaseOwnedStorage, "auto");
