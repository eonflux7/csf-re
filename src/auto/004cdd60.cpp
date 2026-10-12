#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(bool, __fastcall, C_00833908_IsLinkedObjectActiveAndUnblocked, 0x0048F4C0, (C_00833908* self, void* edx));
CSF_FUNC(void, __fastcall, C_0083ab58_ProcessSearchOrder, 0x004CDCD0, (void* self, void* edx, int a2));

// Attempts one forward step of the search sweep on the linked object, then runs the
// order-processing step for this task.
static void __fastcall C_0083ab58_TryAdvanceSearchSweep(C_0083ab58* self, void* /*edx*/, int a2)
{
    C_00833908* linkedObject = reinterpret_cast<C_00833908*>(self->field_0014);

    if (C_00833908_IsLinkedObjectActiveAndUnblocked(linkedObject, nullptr))
    {
        // Query the attached target object (vtable slot 0x24 / 4 = 9) with argument 0.
        void* targetObject = reinterpret_cast<void*>(CSF_AT(uint32_t, linkedObject, 0x48));
        bool targetReady = ((bool(__fastcall*)(void*, void*, int))CSF_AT(void**, targetObject, 0)[9])(
            targetObject, nullptr, 0);

        if (targetReady)
        {
            // Forward request on the linked object (vtable slot 0x8C / 4 = 35).
            ((void(__fastcall*)(void*, void*, int, int, int, int))CSF_AT(void**, linkedObject, 0)[35])(
                linkedObject, nullptr, 0, 0, -1, 0);
        }
    }

    C_0083ab58_ProcessSearchOrder(self, nullptr, a2);
}

HOOK(0x004CDD60, C_0083ab58_TryAdvanceSearchSweep, "auto");
