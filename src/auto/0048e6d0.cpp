#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);
CSF_FUNC(int, __fastcall, CScene_AddObjectFlags, 0x00452800, (CScene* self, void* , int handleAddress, int flags));

static int __fastcall C_00833908_AddSceneObjectFlags(C_00833908* self, void* /*edx*/)
{
    return CScene_AddObjectFlags(
        reinterpret_cast<CScene*>(g_pCurrentScene),
        nullptr,
        reinterpret_cast<int>(reinterpret_cast<std::uint8_t*>(self) + 0x1c),
        1);
}

HOOK(0x0048E6D0, C_00833908_AddSceneObjectFlags, "auto");
