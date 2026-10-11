#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Import slot holding the address of Sleep (called with 10 ms).
CSF_GLOBAL(void*, g_SleepImportSlot, 0x82d05c);

// Destructor-like teardown for the DirectSound enum/effects handle wrapper.
// Asks the handle to stop (slot 18), polls its status (slot 9) up to three
// times with 10 ms sleeps while the busy bit (bit 0) stays set, then releases
// the handle (slot 2). Restores the base-class vtable and returns the release result.
static int __fastcall C_00863cd4_StopPollingAndRelease(C_00863cd4* self, void* /*edx*/)
{
    int result = static_cast<int>(self->field_0004);
    int pollCount = 0;

    *reinterpret_cast<void***>(self) = CSF_VTBL(0x863CD4);

    if (result)
    {
        uint32_t handle = self->field_0004;
        void* handleObj = reinterpret_cast<void*>(handle);
        reinterpret_cast<void(__stdcall*)(void*)>(CSF_AT(void**, handleObj, 0)[18])(handleObj);

        // Poll the busy state.
        handle = self->field_0004;
        handleObj = reinterpret_cast<void*>(handle);
        int busyFlags = 0;
        reinterpret_cast<void(__stdcall*)(void*, int*)>(CSF_AT(void**, handleObj, 0)[9])(handleObj, &busyFlags);

        while ((busyFlags & 1) != 0)
        {
            if (pollCount >= 3)
                break;
            reinterpret_cast<void(__stdcall*)(uint32_t)>(g_SleepImportSlot)(10);

            handle = self->field_0004;
            handleObj = reinterpret_cast<void*>(handle);
            busyFlags = 0;
            ++pollCount;
            reinterpret_cast<void(__stdcall*)(void*, int*)>(CSF_AT(void**, handleObj, 0)[9])(handleObj, &busyFlags);
        }

        handle = self->field_0004;
        handleObj = reinterpret_cast<void*>(handle);
        result = reinterpret_cast<int(__stdcall*)(void*)>(CSF_AT(void**, handleObj, 0)[2])(handleObj);
        self->field_0004 = 0;
    }

    *reinterpret_cast<void***>(self) = CSF_VTBL(0x863C80);
    return result;
}

HOOK(0x711500, C_00863cd4_StopPollingAndRelease, "auto");
