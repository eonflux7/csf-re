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
CSF_GLOBAL(uint8_t, g_byte_82E65C, 0x0082E65C);

CSF_FUNC(int, __fastcall, CompareStringSubstring, 0x0042E200, (void* string, void* edx, unsigned int position, unsigned int count, const char* other, unsigned int otherCount));
CSF_FUNC(uint32_t, __fastcall, GetSceneMenuContext, 0x0044FC60, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(int, __cdecl, GetGlobal8B0558, 0x004F4920, ());
CSF_FUNC(void, __fastcall, CharActionFsm_ApplyNameAndScale, 0x004F49A0, (void* self, void* edx, int a4, float a5));
CSF_FUNC(int, __cdecl, Get_008b0678, 0x00525130, ());
CSF_FUNC(int, __fastcall, MenuMissionInfo_SetGlobalIndex, 0x00525DD0, (void* self, void* , int value));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

// Checks whether this action's scene context matches the current scene menu.
// If so, looks up a matching entry via the value object's virtual method,
// applies its name and scale to the global target, and sets mission index 91.
static int __fastcall CharActionFsm_TryApplyMissionAction(S_00486c30* self, void* /*edx*/, float a2)
{
    uint32_t contextId = self->field_0008;
    uint32_t sceneContext = GetSceneMenuContext(g_pCurrentScene, nullptr);
    int result = static_cast<int>(sceneContext);

    if (contextId == sceneContext)
    {
        int valueObject = GetValue8B0554();
        uint8_t* valueBase = reinterpret_cast<uint8_t*>(valueObject);
        void* receiver = CSF_AT(void*, valueBase, 8128);

        using MatchLookupFn = int(__fastcall*)(void*, void*, float);
        void** receiverVtbl = CSF_AT(void**, receiver, 0);
        int entryPtr = reinterpret_cast<MatchLookupFn>(receiverVtbl[2])(receiver, nullptr, a2);

        uint8_t* entry = reinterpret_cast<uint8_t*>(entryPtr) + 32;
        uint32_t entryCount = CSF_AT(uint32_t, entry, 20);

        result = CompareStringSubstring(entry, nullptr, 0, entryCount,
                                        reinterpret_cast<const char*>(&g_byte_82E65C), 0);
        if (result)
        {
            uint8_t* globalValueBase = reinterpret_cast<uint8_t*>(GetGlobalValue_8BC1E0());
            float a2a = CSF_AT(float, globalValueBase, 6364);

            uint8_t* nameArg = (CSF_AT(uint32_t, entry, 24) < 0x10u)
                                   ? entry + 4
                                   : reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, entry, 4));

            GetGlobal8B0558();
            CharActionFsm_ApplyNameAndScale(reinterpret_cast<void*>(GetGlobal8B0558()), nullptr,
                                            reinterpret_cast<int>(nameArg), a2a);

            int menuObject = Get_008b0678();
            return MenuMissionInfo_SetGlobalIndex(reinterpret_cast<void*>(menuObject), nullptr, 91);
        }
    }
    return result;
}

HOOK(0x486C30, CharActionFsm_TryApplyMissionAction, "auto");
