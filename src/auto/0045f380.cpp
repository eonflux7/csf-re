#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

// __stdcall, 3 args, ret 12. Reads a nested event payload, resolves a handle in the
// current scene, and forwards code 22 plus an integer payload to the resolved object's slot 42 method.
static int __stdcall DispatchSceneObjectEventCode22(int /*unusedA1*/, int /*unusedA2*/, uint32_t* eventArgs)
{
    uint32_t outerRef = eventArgs[0];
    uint32_t innerB = CSF_AT(uint32_t, reinterpret_cast<void*>(outerRef), 4);
    int32_t flag = CSF_AT(int32_t, reinterpret_cast<void*>(innerB), 12);

    int result = static_cast<int>(innerB + 4);
    if (flag >= 0)
    {
        result = CHandleTable_Resolve(g_pCurrentScene, nullptr, result);
        if (result)
        {
            void* component = CSF_AT(void*, reinterpret_cast<void*>(result), 0x34);
            if (component)
            {
                uint32_t secondArg = eventArgs[1];
                uint32_t payloadSource = CSF_AT(uint32_t, reinterpret_cast<void*>(secondArg), 4);

                int callArgs[2];
                callArgs[0] = 22;
                callArgs[1] = 0;
                callArgs[1] = CSF_AT(int, reinterpret_cast<void*>(payloadSource), 4);

                // vtable slot at byte offset 0xA8 -> index 42
                return ((int(__fastcall*)(void*, void*, int*))(CSF_AT(void**, component, 0)[42]))(
                    component, nullptr, callArgs);
            }
        }
    }
    return result;
}

HOOK(0x45F380, DispatchSceneObjectEventCode22, "auto");
