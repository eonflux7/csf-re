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

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);
CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, uint32_t handleKey));
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());

// Original: int __thiscall (this, int, int, uint32_t* a4), ret 12
static int __fastcall SceneHandle_RequestAndCopyPair(void* self, void* /*edx*/, int /*unusedA2*/, int /*unusedA3*/, uint32_t* a4)
{
    // v5 = *(*a4 + 4): record reached through the first pointer of the argument block
    void* sourceRecord = reinterpret_cast<void*>(CSF_AT(uint32_t, reinterpret_cast<void*>(a4[0]), 4));
    int result = CSF_AT(int, sourceRecord, 12);

    if (result >= 0)
    {
        uint8_t* handleKey = static_cast<uint8_t*>(sourceRecord) + 4;  // v7 = v5 + 4

        result = CHandleTable_Resolve(g_pCurrentScene, nullptr, reinterpret_cast<uint32_t>(handleKey));
        void* targetEntity = reinterpret_cast<void*>(result);          // v8
        if (targetEntity != nullptr)
        {
            result = CSF_AT(int, targetEntity, 0x34);
            if (result != 0)
            {
                int queryRecord[4];
                queryRecord[0] = 25;
                queryRecord[1] = -1;
                queryRecord[2] = 0;
                queryRecord[3] = -1;

                void* requestNode = reinterpret_cast<void*>(CSF_AT(uint32_t, reinterpret_cast<void*>(a4[1]), 4));
                queryRecord[1] = CSF_AT(int, requestNode, 4);

                void* subsystem = reinterpret_cast<void*>(GetValue8B0554());
                void* inputDevice = CSF_AT(void*, subsystem, 8132);
                result = ((int(__fastcall*)(void*, void*, int))CSF_AT(void**, inputDevice, 0)[2])(inputDevice, nullptr, queryRecord[1]);

                if (result != -1)
                {
                    // Original order: read a4[2] -> +4 -> +4 first, then reload targetEntity+0x34.
                    void* thirdNode = reinterpret_cast<void*>(CSF_AT(uint32_t, reinterpret_cast<void*>(a4[2]), 4));
                    int thirdValue = CSF_AT(int, thirdNode, 4);

                    void* targetComponent = CSF_AT(void*, targetEntity, 0x34);
                    queryRecord[3] = thirdValue;

                    result = ((int(__fastcall*)(void*, void*, int*))CSF_AT(void**, targetComponent, 0)[42])(targetComponent, nullptr, queryRecord);

                    CSF_AT(uint32_t, self, 0x60) = CSF_AT(uint32_t, handleKey, 4);
                    CSF_AT(uint32_t, self, 0x64) = CSF_AT(uint32_t, handleKey, 8);
                    CSF_AT(uint8_t, self, 0x82) = 0;
                }
            }
        }
    }

    return result;
}

HOOK(0x0045F9A0, SceneHandle_RequestAndCopyPair, "auto");
