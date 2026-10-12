#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int, __fastcall, GfxCollision_MergeSortedIntersections, 0x006ED600, (uint8_t* self, void* , uint8_t* other));

// Projects every item in the array through its virtual slot 13, merges each non-null
// result into the embedded view sub-object, then returns the sub-object if it reports
// "not done" (slot 2 == 0), otherwise nullptr.
static void* __fastcall GfxCamera_ProjectViewIntersections(C_008627d8* self, void* /*edx*/, int a3, int a4, int a5, int a6)
{
    uint8_t* viewObject = reinterpret_cast<uint8_t*>(&self->field_0014);

    // Slot 1 of the view sub-object (vtable +4), no arguments.
    ((void(__fastcall*)(void*, void*))CSF_AT(void**, viewObject, 0)[1])(viewObject, nullptr);

    uint32_t index = 0;
    if (self->field_000c != 0)
    {
        do
        {
            void* item = reinterpret_cast<void**>(self->field_0004)[index];
            // Slot 13 (vtable +52) on the item with four stack arguments.
            int projected = ((int(__fastcall*)(void*, void*, int, int, int, int))CSF_AT(void**, item, 0)[13])(item, nullptr, a3, a4, a5, a6);
            if (projected)
            {
                GfxCollision_MergeSortedIntersections(
                    viewObject,
                    nullptr,
                    reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(projected)));
            }
            ++index;
        } while (index < self->field_000c);
    }

    // Slot 2 of the view sub-object (vtable +8): returns a byte flag.
    uint8_t isFinished = ((uint8_t(__fastcall*)(void*, void*))CSF_AT(void**, viewObject, 0)[2])(viewObject, nullptr);
    return isFinished == 0 ? viewObject : nullptr;
}

HOOK(0x006F58F0, GfxCamera_ProjectViewIntersections, "auto");
