#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static int __fastcall IssueCommandForResolvedHandle(void* self, void* /*edx*/, [[maybe_unused]] int a2, [[maybe_unused]] int a3, uint32_t* a4)
{
    uint32_t* firstArg = reinterpret_cast<uint32_t*>(a4[0]);
    uint32_t v4 = firstArg[1];
    int32_t result = CSF_AT(int32_t, reinterpret_cast<void*>(v4), 12);
    uint32_t v6 = v4 + 4;

    if (result >= 0)
    {
        result = CHandleTable_Resolve(g_pCurrentScene, nullptr, static_cast<int>(v6));
        if (result != 0)
        {
            void* resolvedObj = reinterpret_cast<void*>(result);
            void* targetObj = reinterpret_cast<void*>(CSF_AT(uint32_t, resolvedObj, 0x34));
            if (targetObj != nullptr)
            {
                uint32_t* secondRecord = reinterpret_cast<uint32_t*>(a4[1]);
                uint32_t* secondEntry = reinterpret_cast<uint32_t*>(secondRecord[1]);
                uint32_t* thirdRecord = reinterpret_cast<uint32_t*>(a4[2]);
                uint32_t* thirdEntry = reinterpret_cast<uint32_t*>(thirdRecord[1]);

                int32_t callArgs[3];
                callArgs[1] = -1;
                callArgs[2] = 0;
                callArgs[0] = 2;
                callArgs[1] = static_cast<int32_t>(secondEntry[1]);
                callArgs[2] = static_cast<int32_t>(thirdEntry[1]);

                using SlotFn = int(__fastcall*)(void* self, void* edx, int32_t* args);
                result = reinterpret_cast<SlotFn>(CSF_AT(void**, targetObj, 0)[42])(targetObj, nullptr, callArgs);

                CSF_AT(uint32_t, self, 0x60) = CSF_AT(uint32_t, reinterpret_cast<void*>(v6), 4);
                CSF_AT(uint32_t, self, 0x64) = CSF_AT(uint32_t, reinterpret_cast<void*>(v6), 8);
                CSF_AT(uint8_t, self, 0x82) = 0;
            }
        }
    }
    return result;
}

HOOK(0x0045E840, IssueCommandForResolvedHandle, "auto");
