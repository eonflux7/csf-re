#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Binding for the state-change virtual call target slot (vtable offset 0xD8)
// is done inline below; no other original functions are called directly.

static int __fastcall CharActionFsm_CommitPendingState(void* self, void* /*edx*/);

HOOK(0x004829B0, CharActionFsm_CommitPendingState, "auto");

// If a pending state was queued at +0xA0, make it the current state (+0x9C),
// clear the pending slot, notify it with the "enter" call (vtable slot 12, arg 1),
// and when the active-state index at +0x24 is not -1, ask the owner for its
// sub-object (vtable slot 54) and notify that object the same way.
// Returns the result of the last notification call, or 0 when nothing was pending.
static int __fastcall CharActionFsm_CommitPendingState(void* self, void* /*edx*/)
{
    int result = CSF_AT(int, self, 0xA0);
    if (result != 0)
    {
        CSF_AT(int, self, 0x9C) = result;
        CSF_AT(int, self, 0xA0) = 0;

        void* currentState = reinterpret_cast<void*>(static_cast<uintptr_t>(result));
        result = ((int(__fastcall*)(void*, void*, int))CSF_AT(void**, currentState, 0)[12])(currentState, nullptr, 1);

        if (CSF_AT(int, self, 0x24) != -1)
        {
            void* owner = ((void* (__fastcall*)(void*, void*))CSF_AT(void**, self, 0)[54])(self, nullptr);
            result = ((int(__fastcall*)(void*, void*, int))CSF_AT(void**, owner, 0)[12])(owner, nullptr, 1);
            return result;
        }
    }
    return result;
}
