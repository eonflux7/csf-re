#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RpHAnimHierarchyDetach, 0x0056CF50, (int a1));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(void, __fastcall, C_00860170_Construct, 0x006B2830, (C_00860170* self, void*));

// Destroys the anim-interpolator slice list, detaches the hierarchy, then
// re-runs the base-class (C_00860170) constructor on this object.
static void __fastcall C_00861e08_Destroy(C_00861e08* self, void* /*edx*/)
{
    uint32_t elementArray = CSF_AT(uint32_t, self, 0x0C);
    *(void***)self = CSF_VTBL(0x00863430);

    if (elementArray)
    {
        for (int i = 0;; ++i)
        {
            uint8_t* hierarchy = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, self, 0x04));
            if (!(i < CSF_AT(int, hierarchy, 4)))
                break;

            uint32_t element = reinterpret_cast<uint32_t*>(CSF_AT(uint32_t, self, 0x0C))[i];
            if (element)
            {
                // Virtual slot 1 (+0x4) on the element object, with arg 1.
                void* elementObj = reinterpret_cast<void*>(element);
                ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, elementObj, 0)[1])(elementObj, nullptr, 1);
                reinterpret_cast<uint32_t*>(CSF_AT(uint32_t, self, 0x0C))[i] = 0;
            }
        }

        if (CSF_AT(uint32_t, self, 0x0C))
        {
            Mem_Free(reinterpret_cast<void*>(CSF_AT(uint32_t, self, 0x0C)));
            CSF_AT(uint32_t, self, 0x0C) = 0;
        }
    }

    if (CSF_AT(uint32_t, self, 0x04))
        RpHAnimHierarchyDetach(static_cast<int>(CSF_AT(uint32_t, self, 0x04)));

    *(void***)self = CSF_VTBL(0x00861E08);
    C_00860170_Construct(reinterpret_cast<C_00860170*>(self), nullptr);
}

HOOK(0x708250, C_00861e08_Destroy, "auto");
