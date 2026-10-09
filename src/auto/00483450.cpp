#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Commits a pending character action state. If a pending state is set (field 0x98 != -1),
// it becomes the current state (0x28), the pending slot is cleared, a virtual notification
// is sent, and any queued next-state object (0xA0) is moved to 0x9C and activated. If a
// state was active (0x24 != -1), its virtual 'get child' result is also notified. Finally
// the virtual 'set state' hook is called with (2, 0) and its result is returned.
static int __fastcall CharActionFsm_CommitPendingTransition(void* self, void* /*edx*/)
{
    int pendingState = CSF_AT(int, self, 0x98);
    if (pendingState == -1)
        return pendingState;

    CSF_AT(int, self, 0x28) = pendingState;
    CSF_AT(int, self, 0x98) = -1;

    // vtbl slot 122 (0x1E8): no-argument notification on this.
    void** vtbl = CSF_AT(void**, self, 0);
    ((void(__fastcall*)(void*, void*))vtbl[122])(self, nullptr);

    void* queuedState = CSF_AT(void*, self, 0xA0);
    if (queuedState != nullptr)
    {
        CSF_AT(void*, self, 0x9C) = queuedState;
        CSF_AT(void*, self, 0xA0) = nullptr;

        // vtbl slot 12 (0x30) on the queued state object, argument 1.
        void** queuedVtbl = CSF_AT(void**, queuedState, 0);
        ((void(__fastcall*)(void*, void*, int))queuedVtbl[12])(queuedState, nullptr, 1);

        if (CSF_AT(int, self, 0x24) != -1)
        {
            // vtbl slot 54 (0xD8) on this returns a child object; slot 12 on it, argument 1.
            void* child = ((void* (__fastcall*)(void*, void*))CSF_AT(void**, self, 0)[54])(self, nullptr);
            void** childVtbl = CSF_AT(void**, child, 0);
            ((void(__fastcall*)(void*, void*, int))childVtbl[12])(child, nullptr, 1);
        }
    }

    // vtbl slot 77 (0x134) on this, arguments (2, 0).
    return ((int(__fastcall*)(void*, void*, int, int))CSF_AT(void**, self, 0)[77])(self, nullptr, 2, 0);
}

HOOK(0x483450, CharActionFsm_CommitPendingTransition, "auto");
