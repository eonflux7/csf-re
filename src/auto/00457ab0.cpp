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

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(char, __fastcall, C_GetByte023C, 0x00495490, (char* self, void*));
CSF_FUNC(int, __fastcall, SceneObject_IsSecondCheckTrue, 0x0048E140, (void* self, void* edx));

static C_00833b88* __stdcall HudPortraits_ResolveReadyObject(int handle)
{
    void* resolved = reinterpret_cast<void*>(
        CHandleTable_Resolve(g_pCurrentScene, nullptr, handle));

    if (!resolved)
        return nullptr;

    if (!C_GetByte023C(reinterpret_cast<char*>(resolved), nullptr))
        return nullptr;

    if (!SceneObject_IsSecondCheckTrue(resolved, nullptr))
        return nullptr;

    return reinterpret_cast<C_00833b88*>(resolved);
}

HOOK(0x457AB0, HudPortraits_ResolveReadyObject, "auto");
