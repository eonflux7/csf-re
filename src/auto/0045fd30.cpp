#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __cdecl, IsObjectStateFlagSet, 0x00755980, (int a1));
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

// Reads a request block from the event, resolves its handle in the current scene,
// and if the target has a live sub-object, sends it a 42 message carrying a 3-int vector.
// Then copies two request-block ints into this+0x60/+0x64 and clears this+0x82.
static int __fastcall DispatchPositionMessageToHandle(void* self, void* /*edx*/, int /*a2*/, int /*a3*/, uint32_t* a4)
{
    uint8_t* eventBlock = reinterpret_cast<uint8_t*>(*a4);
    uint8_t* requestBlock = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, eventBlock, 4));
    int result = CSF_AT(int, requestBlock, 12);
    uint8_t* handleBlock = requestBlock + 4;

    if (result >= 0) {
        int handle = static_cast<int>(reinterpret_cast<uintptr_t>(handleBlock));
        result = CHandleTable_Resolve(g_pCurrentScene, nullptr, handle);

        if (result != 0) {
            uint8_t* targetObject = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(result));
            result = CSF_AT(int, targetObject, 0x34);

            if (result != 0) {
                result = IsObjectStateFlagSet(CSF_AT(int, targetObject, 0x50));

                if (static_cast<uint8_t>(result) != 0) {
                    uint8_t* payloadHolder = reinterpret_cast<uint8_t*>(a4[1]);
                    int* vector = reinterpret_cast<int*>(static_cast<uintptr_t>(CSF_AT(uint32_t, payloadHolder, 4) + 4));

                    int message[4] = {42, 0, 0, 0};
                    message[1] = vector[0];
                    message[2] = vector[1];
                    void* subObject = CSF_AT(void*, targetObject, 0x34);
                    message[3] = vector[2];

                    // vtbl slot 0xA8 / 4 = 42, __thiscall(this, int* message)
                    using MessageHandler = int(__fastcall*)(void* self, void* edx, int* message);
                    result = reinterpret_cast<MessageHandler>(CSF_AT(void**, subObject, 0)[42])(subObject, nullptr, message);

                    CSF_AT(uint32_t, self, 0x60) = CSF_AT(uint32_t, handleBlock, 4);
                    CSF_AT(uint32_t, self, 0x64) = CSF_AT(uint32_t, handleBlock, 8);
                    CSF_AT(uint8_t, self, 0x82) = 0;
                }
            }
        }
    }
    return result;
}

HOOK(0x0045FD30, DispatchPositionMessageToHandle, "auto");
