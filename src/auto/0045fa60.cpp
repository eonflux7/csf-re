#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);
CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));

// Sends message 26 with a flag byte taken from the payload to the control resolved from the sender's handle.
// Returns the virtual call's result when a target exists, otherwise the resolved handle value.
static int __stdcall DispatchSceneMessageToHandleControl(int /*unused1*/, int /*unused2*/, uint32_t* messageArgs)
{
    uint32_t* sender = reinterpret_cast<uint32_t*>(messageArgs[0]);
    uint32_t senderInnerAddr = sender[1];
    uint8_t* senderInner = reinterpret_cast<uint8_t*>(senderInnerAddr);
    int32_t senderIndex = CSF_AT(int32_t, senderInner, 0x0C);
    int result = static_cast<int>(senderInnerAddr + 4);

    if (senderIndex >= 0)
    {
        result = CHandleTable_Resolve(static_cast<void*>(g_pCurrentScene), nullptr, result);
        if (result)
        {
            uint8_t* resolved = reinterpret_cast<uint8_t*>(result);
            void* controlObj = CSF_AT(void*, resolved, 0x34);
            if (controlObj)
            {
                uint32_t* messageData = reinterpret_cast<uint32_t*>(messageArgs[1]);
                uint8_t* payload = reinterpret_cast<uint8_t*>(messageData[1]);

                int32_t msg[4] = {};
                msg[0] = 26;
                msg[1] = 1;
                msg[3] = -1;
                *reinterpret_cast<uint8_t*>(&msg[2]) = payload[1];

                void** vtbl = CSF_AT(void**, controlObj, 0);
                return ((int(__fastcall*)(void*, void*, int32_t*))vtbl[42])(controlObj, nullptr, msg);
            }
        }
    }
    return result;
}

HOOK(0x0045FA60, DispatchSceneMessageToHandleControl, "auto");
