#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t, __fastcall, GetSceneMenuContext, 0x0044FC60, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __cdecl, GameConfig_IsResolvedEntryActive, 0x00755880, (int entry));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static int __fastcall Minimap_GetRadarMarkerResource(
    void* self, void* /*edx*/, uint32_t* entity)
{
    if (!entity)
        return 0;

    if (CSF_AT(uint8_t, reinterpret_cast<void*>(GetGlobalState_50D190()), 0xD0))
    {
        if (CSF_AT(uint32_t,
                   reinterpret_cast<void*>(GetSceneMenuContext(g_pCurrentScene, nullptr)),
                   0x78) != CSF_AT(uint32_t, entity, 0x78) ||
            !CSF_AT(int, reinterpret_cast<void*>(GetGlobalState_50D190()), 0xCC))
        {
            return 0;
        }
        return CSF_AT(int, CSF_AT(void*, self, 0x0C), 0x14);
    }

    switch (CSF_AT(uint32_t, entity, 0x78))
    {
    case 0:
        if (!(CSF_AT(uint32_t, entity, 0x170) & 0x2000))
        {
            if (static_cast<uint8_t>(
                    GameConfig_IsResolvedEntryActive(CSF_AT(int, entity, 0x50))))
                return CSF_AT(int, CSF_AT(void*, self, 0x0C), 0x18);
            return 0;
        }
        if (CSF_AT(uint32_t,
                   reinterpret_cast<void*>(GetSceneMenuContext(g_pCurrentScene, nullptr)),
                   0x2E0) & 0x40000)
            return CSF_AT(int, CSF_AT(void*, self, 0x0C), 0x2C);
        return CSF_AT(int, CSF_AT(void*, self, 0x0C), 0x14);

    case 1:
        if (!(CSF_AT(uint32_t, entity, 0x170) & 0x2000))
        {
            if (static_cast<uint8_t>(
                    GameConfig_IsResolvedEntryActive(CSF_AT(int, entity, 0x50))))
                return CSF_AT(int, CSF_AT(void*, self, 0x0C), 0x1C);
            return 0;
        }
        if (CSF_AT(uint32_t,
                   reinterpret_cast<void*>(GetSceneMenuContext(g_pCurrentScene, nullptr)),
                   0x2E0) & 0x40000)
            return CSF_AT(int, CSF_AT(void*, self, 0x0C), 0x30);

        switch (CSF_AT(uint32_t, entity, 0x1D8))
        {
        case 0:
        {
            void* actor = CSF_AT(void*, entity, 0x34);
            if (actor &&
                reinterpret_cast<uint8_t (__fastcall*)(void*, void*)>(
                    CSF_AT(void**, actor, 0)[176 / 4])(actor, nullptr) != 0)
            {
                return CSF_AT(int, CSF_AT(void*, self, 0x0C), 0x08);
            }
            return CSF_AT(int, CSF_AT(void*, self, 0x0C), 0x04);
        }
        case 1:
            return CSF_AT(int, CSF_AT(void*, self, 0x0C), 0x08);
        case 2:
            return CSF_AT(int, CSF_AT(void*, self, 0x0C), 0x0C);
        case 3:
            return CSF_AT(int, CSF_AT(void*, self, 0x0C), 0x10);
        default:
            return 0;
        }

    case 2:
        if (CSF_AT(uint32_t, entity, 0x170) & 0x2000)
        {
            if (CSF_AT(uint32_t,
                       reinterpret_cast<void*>(GetSceneMenuContext(g_pCurrentScene, nullptr)),
                       0x2E0) & 0x40000)
                return CSF_AT(int, CSF_AT(void*, self, 0x0C), 0x2C);
            return CSF_AT(int, CSF_AT(void*, self, 0x0C), 0x20);
        }
        if (static_cast<uint8_t>(
                GameConfig_IsResolvedEntryActive(CSF_AT(int, entity, 0x50))))
            return CSF_AT(int, CSF_AT(void*, self, 0x0C), 0x24);
        return 0;

    default:
        return 0;
    }
}

HOOK(0x0043C780, Minimap_GetRadarMarkerResource, "auto");
