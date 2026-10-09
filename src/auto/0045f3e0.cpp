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

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

// __thiscall with 3 stack args (ret 12): a2, a3 are unused by the body but part of the ABI.
static int __fastcall ApplyHandleTargetAndClearFlag(void* self, void* /*edx*/, int /*a2*/, int /*a3*/, uint32_t* a4)
{
    // a4[0] -> owner record; owner+4 -> source record pointer
    uint8_t* ownerObj = reinterpret_cast<uint8_t*>(a4[0]);
    uint32_t recordAddr = CSF_AT(uint32_t, ownerObj, 4);
    int result = CSF_AT(int, reinterpret_cast<uint8_t*>(recordAddr), 12);
    uint32_t handleAddr = recordAddr + 4;

    if (result >= 0)
    {
        result = CHandleTable_Resolve(g_pCurrentScene, nullptr, static_cast<int>(handleAddr));
        if (result != 0)
        {
            uint8_t* targetObj = reinterpret_cast<uint8_t*>(result);
            uint32_t targetInterface = CSF_AT(uint32_t, targetObj, 0x34);
            if (targetInterface != 0)
            {
                // a4[1] -> holder; holder+4 -> float source
                uint8_t* holder = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, reinterpret_cast<uint8_t*>(a4[1]), 4));
                float sampledValue = CSF_AT(float, holder, 4);

                int callArgs[2];
                callArgs[0] = 17;
                callArgs[1] = 0;
                callArgs[1] = static_cast<int>(sampledValue);

                void* targetVoid = reinterpret_cast<void*>(targetInterface);
                result = ((int(__fastcall*)(void*, void*, int*))CSF_AT(void**, targetVoid, 0)[42])(
                    targetVoid, nullptr, callArgs);

                CSF_AT(uint32_t, self, 0x60) = CSF_AT(uint32_t, reinterpret_cast<uint8_t*>(handleAddr), 4);
                CSF_AT(uint32_t, self, 0x64) = CSF_AT(uint32_t, reinterpret_cast<uint8_t*>(handleAddr), 8);
                CSF_AT(uint8_t, self, 0x82) = 0;
            }
        }
    }
    return result;
}

HOOK(0x0045F3E0, ApplyHandleTargetAndClearFlag, "auto");
