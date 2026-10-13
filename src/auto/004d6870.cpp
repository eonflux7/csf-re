#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, SceneLoader_ConfigureFocusEntry, 0x0048E460, (void* self, void* edx, int* a2));

static char __fastcall C_0083c478_UpdateFocusEntryState(C_0083c478* self, void* /*edx*/, uint32_t* a2)
{
    void* focusHost = reinterpret_cast<void*>(self->field_0014);

    // Probe the nested object at host+0x18 through vtable slot 2 (+0x08).
    void* probeObject = CSF_AT(void*, focusHost, 0x18);
    bool probeResult = ((bool(__fastcall*)(void*, void*))CSF_AT(void**, probeObject, 0)[2])(probeObject, nullptr);

    if (probeResult)
    {
        // Host reports a live focus source: forward to its slot-10 provider's slot-6 method.
        void* sourceObject = ((void* (__fastcall*)(void*, void*))CSF_AT(void**, focusHost, 0)[10])(focusHost, nullptr);
        if (sourceObject == nullptr)
            return 0;

        void* updaterObject = ((void* (__fastcall*)(void*, void*))CSF_AT(void**, focusHost, 0)[10])(focusHost, nullptr);
        return ((char(__fastcall*)(void*, void*))CSF_AT(void**, updaterObject, 0)[6])(updaterObject, nullptr);
    }

    // Otherwise consume the pending flag and, if set, notify the source and reconfigure focus.
    uint8_t pendingFlag = self->field_0054;
    if (pendingFlag == 0)
        return 0;

    self->field_0054 = 0;

    void* sourceObject = ((void* (__fastcall*)(void*, void*))CSF_AT(void**, focusHost, 0)[10])(focusHost, nullptr);
    if (sourceObject != nullptr)
    {
        void* updaterObject = ((void* (__fastcall*)(void*, void*))CSF_AT(void**, focusHost, 0)[10])(focusHost, nullptr);
        ((char(__fastcall*)(void*, void*))CSF_AT(void**, updaterObject, 0)[6])(updaterObject, nullptr);
    }

    uint32_t focusArgs[3];
    focusArgs[0] = a2[18];
    focusArgs[1] = a2[19];
    focusArgs[2] = a2[20];

    return static_cast<char>(SceneLoader_ConfigureFocusEntry(focusHost, nullptr, reinterpret_cast<int*>(focusArgs)));
}

HOOK(0x4D6870, C_0083c478_UpdateFocusEntryState, "auto");
