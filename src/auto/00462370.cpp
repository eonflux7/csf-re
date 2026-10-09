#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __fastcall, InitializeTransientState, 0x004619C0, (void* self, void*));
CSF_FUNC(int, __fastcall, SceneFallbackLookup, 0x004686A0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, SceneFindStateRecord, 0x004686F0, (void* self, void* edx, int a2, int a3));

// Reads the lookup key: *(*(a3[1] + 4) + 4)
static int SceneDispatch_ReadKey(uint32_t* a3)
{
    void* argPtr = reinterpret_cast<void*>(static_cast<uintptr_t>(a3[1]));
    uint32_t firstLevel = CSF_AT(uint32_t, argPtr, 4);
    void* firstLevelPtr = reinterpret_cast<void*>(static_cast<uintptr_t>(firstLevel));
    return static_cast<int>(CSF_AT(uint32_t, firstLevelPtr, 4));
}

static int __stdcall Scene_DispatchHandleEvent(int a1, int a2, uint32_t* a3)
{
    (void)a1;
    (void)a2;

    void* headerPtr = reinterpret_cast<void*>(static_cast<uintptr_t>(a3[0]));
    uint32_t headerTarget = CSF_AT(uint32_t, headerPtr, 4);
    void* targetPtr = reinterpret_cast<void*>(static_cast<uintptr_t>(headerTarget));
    int32_t v4 = CSF_AT(int32_t, targetPtr, 12);

    int result = static_cast<int>(headerTarget + 4);
    if (v4 >= 0)
    {
        result = CHandleTable_Resolve(g_pCurrentScene, nullptr, result);
        if (result)
        {
            int foundRaw = SceneFindStateRecord(CSF_AT(void*, g_pCurrentScene, 0xBC),
                                                nullptr,
                                                result,
                                                SceneDispatch_ReadKey(a3));
            if (foundRaw)
            {
                void* found = reinterpret_cast<void*>(static_cast<uintptr_t>(foundRaw));
                result = InitializeTransientState(found, nullptr);
                CSF_AT(uint8_t, found, 0x80) = 0;
            }
            else
            {
                (void)CSF_VTBL(0x004686A0);
                return SceneFallbackLookup(CSF_AT(void*, g_pCurrentScene, 0xBC),
                                           nullptr,
                                           SceneDispatch_ReadKey(a3));
            }
        }
    }
    return result;
}

HOOK(0x00462370, Scene_DispatchHandleEvent, "auto");
