#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RtAnimInterpolatorDestroy, 0x0061EE90, (int a1));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

static char __fastcall C_00860bb8_ClearLoadedObjects(
    C_00860bb8* self, void* /*edx*/, char releaseStorage)
{
    for (uint32_t index = 0; index < self->field_005c; ++index)
    {
        void* object = CSF_AT(
            void*, reinterpret_cast<void*>(self->field_0054), index * 4);

        if (object)
        {
            int interpolator = CSF_AT(int, object, 0x10);
            if (interpolator)
            {
                for (uint32_t later = index + 1;
                     later < self->field_005c; ++later)
                {
                    void* other = CSF_AT(
                        void*, reinterpret_cast<void*>(self->field_0054),
                        later * 4);
                    if (CSF_AT(int, other, 0x10) == interpolator)
                        CSF_AT(int, other, 0x10) = 0;
                }
                RtAnimInterpolatorDestroy(interpolator);
            }

            ((void(__fastcall*)(void*, void*, int))
                CSF_AT(void**, object, 0)[1])(object, nullptr, 1);
        }
    }

    char result = releaseStorage;
    self->field_005c = 0;
    if (releaseStorage)
    {
        void* storage = reinterpret_cast<void*>(self->field_0054);
        self->field_0058 = 0;
        result = 0;
        if (storage)
        {
            result = static_cast<char>(Mem_Free(storage));
            self->field_0054 = 0;
        }
    }
    return result;
}

HOOK(0x006CB3A0, C_00860bb8_ClearLoadedObjects, "auto");
