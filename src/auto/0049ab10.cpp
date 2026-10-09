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

CSF_FUNC(int, __fastcall, ResetObjectState, 0x00499F00, (void* self, void* edx, char a2));
CSF_FUNC(int, __fastcall, RefreshLinkedState, 0x004993E0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, MissionInfo_IsPrerequisiteMet, 0x0044FB00, ());
CSF_FUNC(uint32_t, __fastcall, GetSceneMenuContext, 0x0044FC60, (void* self, void* edx));
CSF_FUNC(C_00833908*, __fastcall, CCharActionFsm_SetActionMode, 0x00484420, (CCharActionFsm* self, void* , char a2));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static CPlayerCharacter* __fastcall CPlayerCharacter_RefreshSceneMenuAndActivate(CPlayerCharacter* self, void* /*edx*/)
{
    ResetObjectState(self, nullptr, 0);

    if ((self->field_02e0 & 0x2000) != 0)
        RefreshLinkedState(self, nullptr);

    uint32_t result = static_cast<uint32_t>(MissionInfo_IsPrerequisiteMet());
    if (static_cast<uint8_t>(result) != 0)
    {
        result = GetSceneMenuContext(g_pCurrentScene, nullptr);
        if (reinterpret_cast<uint32_t>(self) == result)
        {
            CCharActionFsm* actionFsm = reinterpret_cast<CCharActionFsm*>(self->field_0044);
            if (actionFsm != nullptr)
            {
                C_00833908* activated = CCharActionFsm_SetActionMode(actionFsm, nullptr, 1);
                return reinterpret_cast<CPlayerCharacter*>(activated);
            }
        }
    }
    return reinterpret_cast<CPlayerCharacter*>(result);
}

HOOK(0x0049AB10, CPlayerCharacter_RefreshSceneMenuAndActivate, "auto");
