#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());

// Stores the scene flag byte at this+9, toggles two subsystem states on the global state,
// then forwards the inverted flag to the listener at global state +20 and returns its result
// (or the global state pointer itself when there is no listener).
static int __fastcall NotifySceneByteChanged(void* self, void* /*edx*/, char a2)
{
    CSF_AT(uint8_t, self, 9) = static_cast<uint8_t>(a2);

    int result;
    if (a2)
    {
        // Optional sub-object at global state +116 gets its virtual slot 3 called.
        int pendingObject = CSF_AT(int, reinterpret_cast<uint8_t*>(GetGlobalState_50D190()), 0x74);
        if (pendingObject)
        {
            void* pending = reinterpret_cast<void*>(pendingObject);
            ((void (__fastcall*)(void*, void*))CSF_AT(void**, pending, 0)[12 / 4])(pending, nullptr);
        }

        // Subsystem at global state +16: virtual slot 28 with argument 1.
        int subsystemValue = CSF_AT(int, reinterpret_cast<uint8_t*>(GetGlobalState_50D190()), 0x10);
        void* subsystem = reinterpret_cast<void*>(subsystemValue);
        ((void (__fastcall*)(void*, void*, int))CSF_AT(void**, subsystem, 0)[112 / 4])(subsystem, nullptr, 1);

        // Global state itself: virtual slot 23 with argument 1.
        void* globalState = reinterpret_cast<void*>(GetGlobalState_50D190());
        ((void (__fastcall*)(void*, void*, int))CSF_AT(void**, globalState, 0)[92 / 4])(globalState, nullptr, 1);
    }
    else
    {
        int subsystemValue = CSF_AT(int, reinterpret_cast<uint8_t*>(GetGlobalState_50D190()), 0x10);
        void* subsystem = reinterpret_cast<void*>(subsystemValue);
        ((void (__fastcall*)(void*, void*, int))CSF_AT(void**, subsystem, 0)[112 / 4])(subsystem, nullptr, 0);

        void* globalState = reinterpret_cast<void*>(GetGlobalState_50D190());
        ((void (__fastcall*)(void*, void*, int))CSF_AT(void**, globalState, 0)[92 / 4])(globalState, nullptr, 0);
    }

    result = GetGlobalState_50D190();
    int listenerValue = CSF_AT(int, reinterpret_cast<uint8_t*>(result), 0x14);
    if (listenerValue)
    {
        void* listener = reinterpret_cast<void*>(listenerValue);
        int invertedFlag = (CSF_AT(uint8_t, self, 9) == 0) ? 1 : 0;
        ((void (__fastcall*)(void*, void*, int))CSF_AT(void**, listener, 0)[156 / 4])(listener, nullptr, invertedFlag);
        return ((int (__fastcall*)(void*, void*))CSF_AT(void**, listener, 0)[168 / 4])(listener, nullptr);
    }
    return result;
}

HOOK(0x444940, NotifySceneByteChanged, "auto");
