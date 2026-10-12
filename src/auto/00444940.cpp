#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());

static uint8_t* AsBytePointer(int address)
{
    return reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(address));
}

static int __fastcall NotifySceneByteChanged(uint8_t* self, void* /*edx*/, char a2)
{
    CSF_AT(uint8_t, self, 0x09) = static_cast<uint8_t>(a2);

    if (a2)
    {
        // Pending scene object at GlobalState+0x74: notify it first if present.
        uint8_t* globalState = AsBytePointer(GetGlobalState_50D190());
        void* pendingScene = CSF_AT(void*, globalState, 0x74);
        if (pendingScene)
        {
            ((void(__fastcall*)(void*, void*))CSF_AT(void**, pendingScene, 0)[0x0C / 4])(pendingScene, nullptr);
        }

        // Activate the subsystem at GlobalState+0x10 (vtbl slot 0x70).
        uint8_t* globalStateForSubsystem = AsBytePointer(GetGlobalState_50D190());
        void* subsystem = CSF_AT(void*, globalStateForSubsystem, 0x10);
        ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, subsystem, 0)[0x70 / 4])(subsystem, nullptr, 1);

        // Then set the state on GlobalState itself (vtbl slot 0x5C).
        void* globalStateObj = AsBytePointer(GetGlobalState_50D190());
        ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, globalStateObj, 0)[0x5C / 4])(globalStateObj, nullptr, 1);
    }
    else
    {
        uint8_t* globalState = AsBytePointer(GetGlobalState_50D190());
        void* subsystem = CSF_AT(void*, globalState, 0x10);
        ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, subsystem, 0)[0x70 / 4])(subsystem, nullptr, 0);

        void* globalStateObj = AsBytePointer(GetGlobalState_50D190());
        ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, globalStateObj, 0)[0x5C / 4])(globalStateObj, nullptr, 0);
    }

    int globalStateValue = GetGlobalState_50D190();
    void* listener = CSF_AT(void*, AsBytePointer(globalStateValue), 0x14);
    if (listener)
    {
        int isNotSet = (CSF_AT(uint8_t, self, 0x09) == 0) ? 1 : 0;
        ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, listener, 0)[0x9C / 4])(listener, nullptr, isNotSet);
        return ((int(__fastcall*)(void*, void*))CSF_AT(void**, listener, 0)[0xA8 / 4])(listener, nullptr);
    }
    return globalStateValue;
}

HOOK(0x00444940, NotifySceneByteChanged, "auto");
