#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t, __fastcall, GetSceneMenuContext, 0x0044FC60, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobal8B0688, 0x00550CC0, ());
CSF_FUNC(int, __cdecl, GetMpGameModeIndex, 0x005515F0, ());

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static int __fastcall Minimap_ResolveTarget(
    void* self, void* /*edx*/, void* target,
    uint8_t* specialMarker, uint8_t* normalMarker)
{
    int icon = 0;
    *specialMarker = 0;
    *normalMarker = 1;

    if (!target
        || !GetSceneMenuContext(g_pCurrentScene, nullptr)
        || reinterpret_cast<uintptr_t>(target)
            == GetSceneMenuContext(g_pCurrentScene, nullptr)
        || !GetGlobal8B0688())
    {
        return icon;
    }

    void* sceneContext = reinterpret_cast<void*>(
        GetSceneMenuContext(g_pCurrentScene, nullptr));
    uint32_t localComponent = CSF_AT(uint32_t, sceneContext, 0x18);
    void* localActor = reinterpret_cast<void*>(
        localComponent ? localComponent - 1712 : 0);

    void* targetComponent = CSF_AT(void*, target, 0x18);
    if (targetComponent)
    {
        int targetType = CSF_AT(int, targetComponent, 0x0C);
        if (targetType)
        {
            if (targetType == 14)
            {
                if (CSF_AT(uint8_t, targetComponent, -48) == 1)
                {
                    icon = CSF_AT(int, CSF_AT(void*, self, 0x0C), 0x2C);
                }
                else if (CSF_AT(uint8_t, targetComponent, -48) == 2)
                {
                    icon = CSF_AT(int, CSF_AT(void*, self, 0x0C), 0x30);
                }
                *specialMarker = 1;
            }
        }
        else
        {
            bool showTeamIcon = CSF_AT(uint8_t, self, 0x4C) != 0;
            if (!showTeamIcon)
            {
                GetGlobal8B0688();
                if (GetMpGameModeIndex())
                {
                    uint8_t localTeam = CSF_AT(uint8_t, localActor, 1091);
                    showTeamIcon = localTeam == 0
                        || localTeam == CSF_AT(uint8_t, targetComponent, -621);
                }
            }

            if (showTeamIcon)
            {
                if (CSF_AT(uint8_t, targetComponent, -621) == 1)
                {
                    if (CSF_AT(uint8_t, targetComponent, -600))
                    {
                        if (CSF_AT(uint8_t, targetComponent, -600) == 1)
                        {
                            icon = CSF_AT(int, CSF_AT(void*, self, 0x0C), 0x08);
                        }
                        else if (CSF_AT(uint8_t, targetComponent, -600) == 2)
                        {
                            icon = CSF_AT(int, CSF_AT(void*, self, 0x0C), 0x0C);
                        }
                    }
                    else
                    {
                        icon = CSF_AT(int, CSF_AT(void*, self, 0x0C), 0x04);
                    }
                }
                else if (CSF_AT(uint8_t, targetComponent, -621) == 2)
                {
                    if (CSF_AT(uint8_t, targetComponent, -600))
                    {
                        if (CSF_AT(uint8_t, targetComponent, -600) == 1)
                        {
                            icon = CSF_AT(int, CSF_AT(void*, self, 0x0C), 0x14);
                        }
                        else if (CSF_AT(uint8_t, targetComponent, -600) == 2)
                        {
                            icon = CSF_AT(int, CSF_AT(void*, self, 0x0C), 0x18);
                        }
                    }
                    else
                    {
                        icon = CSF_AT(int, CSF_AT(void*, self, 0x0C), 0x10);
                    }
                }
            }

            if (CSF_AT(int, targetComponent, -72) != -1)
            {
                if (CSF_AT(uint8_t, targetComponent, -621) == 1)
                    icon = CSF_AT(int, CSF_AT(void*, self, 0x0C), 0x24);
                else
                    icon = CSF_AT(int, CSF_AT(void*, self, 0x0C), 0x28);

                *normalMarker = 0;
            }
        }
    }

    if (CSF_AT(int, target, 0x50) == 24)
        return CSF_AT(int, CSF_AT(void*, self, 0x0C), 0x20);

    return icon;
}

HOOK(0x0043B9A0, Minimap_ResolveTarget, "auto");
