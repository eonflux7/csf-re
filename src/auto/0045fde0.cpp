#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __cdecl, QueryObjectFlagByte, 0x00755980, (int a1));

// Looks up the object behind a handle stored in the record passed as the third
// stack argument. If the object is valid and its flag query succeeds, it forwards
// a message (code 43) to the object's vtable slot 42, then copies two dwords from
// the handle entry into self+0x60/+0x64 and clears the byte at self+0x82.
static int __fastcall ProcessTrackedHandleUpdate(void* self, void* /*edx*/,
                                                 int /*a2*/, int /*a3*/, uint32_t* a4)
{
    uint8_t* recordPtr = reinterpret_cast<uint8_t*>(a4[0]);
    uint8_t* handleEntry = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, recordPtr, 4));
    int resultValue = CSF_AT(int, handleEntry, 12);
    uint8_t* handleSlot = handleEntry + 4;

    if (resultValue >= 0)
    {
        resultValue = CHandleTable_Resolve(g_pCurrentScene, nullptr,
                                           reinterpret_cast<int>(handleSlot));
        int resolvedObj = resultValue;
        if (resolvedObj != 0)
        {
            uint8_t* obj = reinterpret_cast<uint8_t*>(resolvedObj);
            resultValue = CSF_AT(int, obj, 52);
            if (resultValue != 0)
            {
                resultValue = QueryObjectFlagByte(CSF_AT(int, obj, 80));
                if (static_cast<uint8_t>(resultValue) != 0)
                {
                    uint32_t eventArgs[2];
                    uint8_t* payloadRecord = reinterpret_cast<uint8_t*>(a4[1]);
                    uint8_t* payloadSource =
                        reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, payloadRecord, 4));

                    eventArgs[0] = 43;
                    eventArgs[1] = CSF_AT(uint32_t, payloadSource, 4);

                    void* targetObj = CSF_AT(void*, obj, 52);
                    void** vtbl = CSF_AT(void**, targetObj, 0);
                    reinterpret_cast<void(__fastcall*)(void*, void*, uint32_t*)>(vtbl[42])(
                        targetObj, nullptr, eventArgs);

                    resultValue = static_cast<int>(CSF_AT(uint32_t, handleSlot, 4));
                    CSF_AT(int, self, 0x60) = resultValue;
                    CSF_AT(int, self, 0x64) = CSF_AT(int, handleSlot, 8);
                    CSF_AT(uint8_t, self, 0x82) = 0;
                }
            }
        }
    }
    return resultValue;
}

HOOK(0x0045FDE0, ProcessTrackedHandleUpdate, "auto");
