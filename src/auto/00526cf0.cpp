#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Releases the object referenced by *slot. If the object lies outside the
// [self[0], self[1]] pool range, its scalar deleting destructor (vtbl[0], flag 1)
// is called; otherwise its vtbl[9] release method is called. The slot is cleared
// in both cases, and the cached handle at self[2] is dropped if it matches.
static unsigned int* __fastcall ReleaseSlotObject(unsigned int* self, void* /*edx*/, unsigned int* slot)
{
    unsigned int* result = self;

    if (*slot != 0)
    {
        if (*slot == CSF_AT(uint32_t, self, 0x08))
            CSF_AT(uint32_t, self, 0x08) = 0;

        uint32_t objAddr = *slot;
        if (objAddr < CSF_AT(uint32_t, self, 0x00) || objAddr > CSF_AT(uint32_t, self, 0x04))
        {
            if (objAddr != 0)
            {
                void* obj = reinterpret_cast<void*>(objAddr);
                result = reinterpret_cast<unsigned int*>(
                    reinterpret_cast<int(__fastcall*)(void*, void*, int)>(CSF_AT(void**, obj, 0)[0])(obj, nullptr, 1));
                *slot = 0;
            }
        }
        else
        {
            void* obj = reinterpret_cast<void*>(objAddr);
            result = reinterpret_cast<unsigned int*>(
                reinterpret_cast<int(__fastcall*)(void*, void*)>(CSF_AT(void**, obj, 0)[9])(obj, nullptr));
            *slot = 0;
        }
    }

    return result;
}

HOOK(0x526CF0, ReleaseSlotObject, "auto");
