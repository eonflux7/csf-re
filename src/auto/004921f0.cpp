#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Accumulates elapsed time into the timer at +0x200 while the object is active.
// Once the timer passes the threshold at +0x1FC, it asks the object at +0x48 whether
// request 53 is allowed, checks the object returned by vGetField016c, and if both
// accept, forwards request 53. It then calls the vSlotC4 hook. If the object is not
// active, the timer is reset to zero.
static void __fastcall C_00833b88_TickTimedRequest(C_00833b88* self, void* /*edx*/, float elapsedSeconds)
{
    // Slot 0xB8: activity check, returns bool (tested with test al, al).
    const bool isActive =
        ((bool(__fastcall*)(void*, void*))CSF_AT(void**, self, 0)[0xB8 / 4])(self, nullptr);

    if (!isActive) {
        CSF_AT(float, self, 0x200) = 0.0f;
        return;
    }

    CSF_AT(float, self, 0x200) = elapsedSeconds + CSF_AT(float, self, 0x200);
    if (!(CSF_AT(float, self, 0x200) > CSF_AT(float, self, 0x1FC))) {
        return;
    }

    void* requestTarget = reinterpret_cast<void*>(self->field_0048);
    // Slot 0x4C on the object at +0x48, with request id 53.
    const bool targetAccepts =
        ((bool(__fastcall*)(void*, void*, int))CSF_AT(void**, requestTarget, 0)[0x4C / 4])(
            requestTarget, nullptr, 0x35);

    if (targetAccepts && self->vGetField016c()) {
        void* subObject = reinterpret_cast<void*>(self->vGetField016c());
        // Slot 0x40 on the sub-object returned by vGetField016c.
        const bool subObjectAccepts =
            ((bool(__fastcall*)(void*, void*))CSF_AT(void**, subObject, 0)[0x40 / 4])(subObject, nullptr);
        if (subObjectAccepts) {
            self->vSlot8C(0x35, 0, -1, 0);
        }
    }

    // Slot 0xC4: post-tick hook, no result used.
    ((void(__fastcall*)(void*, void*))CSF_AT(void**, self, 0)[0xC4 / 4])(self, nullptr);
}

HOOK(0x4921F0, C_00833b88_TickTimedRequest, "auto");
