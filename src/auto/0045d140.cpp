#include <cstdint>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __fastcall, SceneEntity_LookupSlotEntry, 0x0049ED10, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, Object_IsEnabled, 0x00755880, (int a1));

static int __stdcall Scene_ResolveHandleAndSetFlag(int a1, int a2, uint32_t* a3)
{
    (void)a2;

    // a1 + 4 holds a pointer to a state block; its byte +1 is the output flag.
    void* stateOwner = reinterpret_cast<void*>(static_cast<uintptr_t>(a1));
    CSF_AT(uint8_t, reinterpret_cast<void*>(static_cast<uintptr_t>(CSF_AT(uint32_t, stateOwner, 4))), 1) = 0;

    // handle record = *(*a3 + 4) + 4
    uint32_t handleRecordAddr =
        CSF_AT(uint32_t, reinterpret_cast<void*>(static_cast<uintptr_t>(a3[0])), 4) + 4;

    int returnValue = static_cast<int>(handleRecordAddr);

    int handleValue = CSF_AT(int32_t, reinterpret_cast<void*>(static_cast<uintptr_t>(handleRecordAddr)), 8);
    if (handleValue >= 0)
    {
        int resolvedAddr = CHandleTable_Resolve(g_pCurrentScene, nullptr, static_cast<int>(handleRecordAddr));
        returnValue = resolvedAddr;

        C_008342e8* sceneEntity = reinterpret_cast<C_008342e8*>(static_cast<uintptr_t>(resolvedAddr));
        if (sceneEntity != nullptr)
        {
            int objectEnabled = Object_IsEnabled(static_cast<int>(CSF_AT(uint32_t, sceneEntity, 0x50)));
            returnValue = objectEnabled;

            if (static_cast<uint8_t>(objectEnabled) != 0)
            {
                // *(*(a3[1] + 4) + 4) read as float, then truncated to integer
                void* inner = reinterpret_cast<void*>(static_cast<uintptr_t>(
                    CSF_AT(uint32_t, reinterpret_cast<void*>(static_cast<uintptr_t>(a3[1])), 4)));
                float slotValue = CSF_AT(float, inner, 4);

                uint32_t slotIndex = static_cast<uint32_t>(static_cast<int64_t>(slotValue));
                returnValue = static_cast<int>(slotIndex);

                if (slotIndex < static_cast<uint32_t>(sceneEntity->field_01a0))
                {
                    int entryAddr = SceneEntity_LookupSlotEntry(sceneEntity, nullptr, static_cast<int>(slotIndex));
                    returnValue = entryAddr;

                    bool entryNonNegative =
                        CSF_AT(int32_t, reinterpret_cast<void*>(static_cast<uintptr_t>(entryAddr)), 8) >= 0;

                    // Re-read a1 + 4 after the calls, as the original does.
                    CSF_AT(uint8_t, reinterpret_cast<void*>(static_cast<uintptr_t>(CSF_AT(uint32_t, stateOwner, 4))), 1) =
                        entryNonNegative ? 1 : 0;
                }
            }
        }
    }

    return returnValue;
}

HOOK(0x0045D140, Scene_ResolveHandleAndSetFlag, "auto");
