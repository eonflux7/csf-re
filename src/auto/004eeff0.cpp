#include <cstdint>
#include "globals.h"
#include "hook.h"

// Bound callee: SceneLoader_SetTextDatabasePath (__thiscall, pops 8 bytes).
CSF_FUNC(void, __fastcall, SceneLoader_SetTextDatabasePath, 0x0069BD90,
         (void* self, void* edx, unsigned int a2, char* a3));

// Registers the MultiPla and SysRed text database paths on this object.
static void __fastcall CTextDatabase_SetMultiPlaAndSysRedPaths(void* self, void* /*edx*/)
{
    SceneLoader_SetTextDatabasePath(self, nullptr, 2u,
                                    (char*)CSF_STR(0x0083EEB0, "Texts/MultiPla.fli"));
    SceneLoader_SetTextDatabasePath(self, nullptr, 0x20u,
                                    (char*)CSF_STR(0x0083EE9C, "Texts/SysRed.fli"));
}

HOOK(0x004EEFF0, CTextDatabase_SetMultiPlaAndSysRedPaths, "auto");
