#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, GfxCollision_MergeSortedIntersections, 0x006ED600, (uint8_t* self, void* , uint8_t* other));

// Refreshes the embedded projection sub-object at +0x14, asks every child in the
// array at +0x04 (count at +0x0C) for its intersections, merges each non-null result
// into the sub-object's collision list, then returns the sub-object if its state
// check (vtable slot 2) reports it is not closed, otherwise nullptr.
static uint32_t* __fastcall GfxCamera_CollectChildIntersections(C_008627d8* self, void* /*edx*/, int arg0, int arg1, int arg2)
{
    void* subObj = reinterpret_cast<uint8_t*>(self) + 0x14;

    // subObj->Refresh()  (vtable slot 1)
    ((void(__fastcall*)(void*, void*))CSF_AT(void**, subObj, 0)[1])(subObj, nullptr);

    uint32_t childIndex = 0;
    if (self->field_000c != 0)
    {
        do
        {
            void** childArray = reinterpret_cast<void**>(self->field_0004);
            void* childObj = childArray[childIndex];

            // childObj->GetIntersections(arg0, arg1, arg2)  (vtable slot 11)
            int intersectionList = ((int(__fastcall*)(void*, void*, int, int, int))CSF_AT(void**, childObj, 0)[11])(childObj, nullptr, arg0, arg1, arg2);

            if (intersectionList != 0)
            {
                GfxCollision_MergeSortedIntersections(reinterpret_cast<uint8_t*>(subObj), nullptr, reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(intersectionList)));
            }

            ++childIndex;
        } while (childIndex < self->field_000c);
    }

    // subObj->IsClosed()  (vtable slot 2)
    uint8_t isClosed = ((uint8_t(__fastcall*)(void*, void*))CSF_AT(void**, subObj, 0)[2])(subObj, nullptr);
    return isClosed == 0 ? reinterpret_cast<uint32_t*>(subObj) : nullptr;
}

HOOK(0x6F5960, GfxCamera_CollectChildIntersections, "auto");
