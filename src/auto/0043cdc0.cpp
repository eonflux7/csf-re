#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, Minimap_IsPositionVisible, 0x0043AAD0, (void* self, void* edx, float* pos));
CSF_FUNC(char, __fastcall, Minimap_AddObjectIcon, 0x0043B2D0, (void* self, void* edx, void* obj, char flag, float value));
CSF_FUNC(uint32_t, __fastcall, Minimap_UpdateObjectOffscreen, 0x0043B720, (C_00830e5c* self, void* edx, C_008342e8* obj));
CSF_FUNC(int, __cdecl, GetGlobalValue_44ACE0, 0x0044ACE0, ());
CSF_FUNC(int, __fastcall, Global_CheckEntry, 0x0044AF70, (void* self, void* edx, int a2));
CSF_FUNC(uint32_t, __fastcall, GetSceneMenuContext, 0x0044FC60, (void* self, void* edx));
CSF_FUNC(float*, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(int, __fastcall, IsField1BCNonNegative, 0x00492B10, (void* self, void*));
CSF_FUNC(int, __cdecl, GameConfig_IsResolvedEntryActive, 0x00755880, (int entry));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static char __fastcall Minimap_UpdateObject(C_00830e5c* self, void* /*edx*/, C_008342e8* a2)
{
    char result = 0;
    bool proceed = false;

    if (!CSF_AT(uint32_t, a2, 0x50)) {
        proceed = true;
    } else if ((CSF_AT(uint32_t, a2, 0x170) & 0x2000) != 0 &&
               !(char)IsField1BCNonNegative(a2, nullptr)) {
        proceed = true;
    } else {
        result = (char)GameConfig_IsResolvedEntryActive((int)CSF_AT(uint32_t, a2, 0x50));
        if (result != 0)
            proceed = true;
    }

    if (!proceed)
        return result;

    float* pos = GetObjectPositionVector(a2, nullptr);
    if ((char)Minimap_IsPositionVisible(self, nullptr, pos)) {
        uint32_t ctx = GetSceneMenuContext(g_pCurrentScene, nullptr);
        if (CSF_AT(uint32_t, a2, 0x50) || reinterpret_cast<uintptr_t>(a2) == ctx)
            return Minimap_AddObjectIcon(self, nullptr, a2, 0, 0.0f);
        else
            return Minimap_AddObjectIcon(self, nullptr, a2, 1, 0.0f);
    } else {
        int global = GetGlobalValue_44ACE0();
        int arg = (int)(reinterpret_cast<uint8_t*>(a2) + 0x1C);
        if ((char)Global_CheckEntry(reinterpret_cast<void*>(global), nullptr, arg)) {
            uint32_t bits = 1;
            float tiny;
            std::memcpy(&tiny, &bits, sizeof(tiny));
            return Minimap_AddObjectIcon(self, nullptr, a2, 0, tiny);
        } else {
            return (char)Minimap_UpdateObjectOffscreen(self, nullptr, a2);
        }
    }
}

HOOK(0x0043CDC0, Minimap_UpdateObject, "auto");
