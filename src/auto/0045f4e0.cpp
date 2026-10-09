#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

// __thiscall(this, a2, a3, a4) with ret 12: a2/a3 are unused by the original.
static int __fastcall SceneHandle_ApplyTargetRequest(void* self, void* /*edx*/, int /*a2*/, int /*a3*/, uint32_t* a4)
{
    // a4 -> request object; request->+4 -> entry whose +4 is the handle block.
    uint8_t* requestObj = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(a4[0]));
    uint8_t* entry = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(
        *reinterpret_cast<uint32_t*>(requestObj + 4)));

    int result = *reinterpret_cast<int*>(entry + 12);
    uint8_t* handleAddr = entry + 4;

    if (result >= 0)
    {
        result = CHandleTable_Resolve(g_pCurrentScene, nullptr, static_cast<int>(reinterpret_cast<uintptr_t>(handleAddr)));
        if (result)
        {
            uint8_t* resolved = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(result));
            int v8 = CSF_AT(int, resolved, 0x34);
            if (v8)
            {
                // Second request argument: a4[1] -> +4 -> float at +4.
                uint8_t* sourceObj = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(a4[1]));
                uint8_t* sourceSub = CSF_AT(uint8_t*, sourceObj, 4);
                float sourceValue = CSF_AT(float, sourceSub, 4);

                // Message buffer: { int 23; float -sourceValue }
                int messageArgs[2];
                messageArgs[0] = 23;
                float negated = -sourceValue;
                std::memcpy(&messageArgs[1], &negated, sizeof(float));

                void* target = reinterpret_cast<void*>(static_cast<uintptr_t>(v8));
                result = ((int(__fastcall*)(void*, void*, int*))(CSF_AT(void**, target, 0)[42]))(
                    target, nullptr, messageArgs);

                CSF_AT(uint32_t, self, 0x60) = CSF_AT(uint32_t, handleAddr, 4);
                CSF_AT(uint32_t, self, 0x64) = CSF_AT(uint32_t, handleAddr, 8);
                CSF_AT(uint8_t, self, 0x82) = 0;
            }
        }
    }
    return result;
}

HOOK(0x0045F4E0, SceneHandle_ApplyTargetRequest, "auto");
