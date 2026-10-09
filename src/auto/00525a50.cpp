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

CSF_FUNC(uint32_t, __fastcall, GetSceneMenuContext, 0x0044FC60, (void* self, void* edx));
CSF_FUNC(char, __stdcall, sub_525370, 0x00525370, (int a1));
CSF_FUNC(int, __fastcall, SceneLoader_GetResourceById, 0x007567C0, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

static int __fastcall C_00833908_SetSceneResourceIfSceneMatches(C_00833908* self, void* /*edx*/, int sceneId, int resourceId)
{
    int result = static_cast<int>(GetSceneMenuContext(g_pCurrentScene, nullptr));

    if (sceneId == result)
    {
        int globalValue = GetGlobalValue_8BC1E0();
        result = SceneLoader_GetResourceById(reinterpret_cast<uint8_t*>(globalValue) + 0x22A4, nullptr, resourceId);

        if (result != -1)
        {
            int currentResource = static_cast<int>(self->field_002c);
            result = currentResource;

            bool replace = false;
            if (currentResource == -1)
            {
                replace = true;
            }
            else
            {
                char currentIsPositional = sub_525370(currentResource);
                if (currentIsPositional)
                {
                    replace = true;
                }
                else
                {
                    char newIsPositional = sub_525370(resourceId);
                    if (!newIsPositional)
                        replace = true;
                }
            }

            if (replace)
                self->field_002c = static_cast<uint32_t>(resourceId);
        }
    }

    return result;
}

HOOK(0x525A50, C_00833908_SetSceneResourceIfSceneMatches, "auto");
