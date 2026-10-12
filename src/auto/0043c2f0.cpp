#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(int, __fastcall, Minimap_SelfPointCheck, 0x0043AAD0, (void* self, void* edx, float* a2));
CSF_FUNC(char, __fastcall, Minimap_DrawObject, 0x0043B2D0, (void* self, void* edx, void* a2, char a3, float a4));
CSF_FUNC(uint32_t, __fastcall, Minimap_HandleOutOfRange, 0x0043B720, (C_00830e5c* self, void* edx, C_008342e8* a2));
CSF_FUNC(int, __stdcall, IsFlaggedInFirstThreeStates, 0x0043C280, (void* object));
CSF_FUNC(uint32_t, __fastcall, GetSceneMenuContext, 0x0044FC60, (void* self, void* edx));
CSF_FUNC(float*, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(int, __fastcall, IsField1BCNonNegative, 0x00492B10, (void* self, void*));
CSF_FUNC(int, __cdecl, GameConfig_IsResolvedEntryActive, 0x00755880, (int entry));

static char __fastcall Minimap_ProcessObject(C_00830e5c* self, void* /*edx*/, C_008342e8* obj)
{
    uint32_t context = GetSceneMenuContext(g_pCurrentScene, nullptr);
    char result = static_cast<char>(context);

    if (reinterpret_cast<uint32_t>(obj) == context)
        return result;

    bool process = false;
    if (!CSF_AT(uint32_t, obj, 0x50)) {
        process = true;
    } else if ((CSF_AT(uint32_t, obj, 0x170) & 0x2000) != 0 &&
               !static_cast<uint8_t>(IsField1BCNonNegative(obj, nullptr))) {
        process = true;
    } else {
        int active = GameConfig_IsResolvedEntryActive(static_cast<int>(CSF_AT(uint32_t, obj, 0x50)));
        result = static_cast<char>(active);
        process = static_cast<uint8_t>(active) != 0;
    }

    if (!process)
        return result;

    float* position = GetObjectPositionVector(obj, nullptr);
    if (static_cast<uint8_t>(Minimap_SelfPointCheck(self, nullptr, position))) {
        uint32_t ctx = GetSceneMenuContext(g_pCurrentScene, nullptr);
        if ((CSF_AT(uint32_t, reinterpret_cast<void*>(ctx), 0x2E0) & 0x40000) != 0) {
            char flagged = static_cast<char>(IsFlaggedInFirstThreeStates(obj));
            return Minimap_DrawObject(self, nullptr, obj, flagged == 0 ? 1 : 0, 0.0f);
        }
        return Minimap_DrawObject(self, nullptr, obj, 1, 0.0f);
    }
    return static_cast<char>(Minimap_HandleOutOfRange(self, nullptr, obj));
}

HOOK(0x0043C2F0, Minimap_ProcessObject, "auto");
