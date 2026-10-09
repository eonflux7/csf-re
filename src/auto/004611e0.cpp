#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static uint8_t* WordToBytePtr(uint32_t word)
{
    return reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(word));
}

static int __stdcall DispatchPayloadByteToHandleObject(int /*unused1*/, int /*unused2*/, uint32_t* argBlock)
{
    uint8_t* entry = WordToBytePtr(argBlock[0]);
    uint8_t* inner = WordToBytePtr(CSF_AT(uint32_t, entry, 4));
    int32_t innerFlag = CSF_AT(int32_t, inner, 12);
    int result = static_cast<int>(reinterpret_cast<uintptr_t>(inner) + 4);

    if (innerFlag >= 0) {
        result = CHandleTable_Resolve(g_pCurrentScene, nullptr, result);
        if (result != 0 && CSF_AT(int32_t, WordToBytePtr(static_cast<uint32_t>(result)), 0x50) == 0x20) {
            uint8_t* payloadBlock = WordToBytePtr(argBlock[1]);
            uint8_t* payload = WordToBytePtr(CSF_AT(uint32_t, payloadBlock, 4));
            uint8_t payloadByte = CSF_AT(uint8_t, payload, 1);

            int32_t eventArgs[3];
            eventArgs[0] = 7;
            eventArgs[1] = payloadByte;
            eventArgs[2] = 0;

            uint8_t* targetObject = WordToBytePtr(static_cast<uint32_t>(result));
            void** subVtbl = CSF_AT(void**, targetObject, 4);
            return reinterpret_cast<int(__fastcall*)(void*, void*, int32_t*)>(subVtbl[18])(
                targetObject + 4, nullptr, eventArgs);
        }
    }
    return result;
}

HOOK(0x004611E0, DispatchPayloadByteToHandleObject, "auto");
