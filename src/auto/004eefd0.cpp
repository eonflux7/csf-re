#include <cstdint>
#include "globals.h"
#include "hook.h"

// Bound callee: SceneLoader_SetTextDatabasePath (__thiscall, pops 8 bytes).
CSF_FUNC(void, __fastcall, SceneLoader_SetTextDatabasePath, 0x0069BD90,
         (void* self, void* edx, unsigned int a2, char* a3));

// Global holding the default text database path (dword_83EE8C).
CSF_GLOBAL(char*, g_DefaultTextDatabasePath, 0x0083EE8C);

// Sets the text database paths: slot 1 gets the default path global,
// slot 8 gets "Texts/Global.fli".
static void __fastcall CTextDatabase_LoadGlobalTextPaths(void* self, void* /*edx*/)
{
    SceneLoader_SetTextDatabasePath(self, nullptr, 1u, g_DefaultTextDatabasePath);
    SceneLoader_SetTextDatabasePath(self, nullptr, 8u,
                                    (char*)CSF_STR(0x0083EE78, "Texts/Global.fli"));
}

HOOK(0x004EEFD0, CTextDatabase_LoadGlobalTextPaths, "auto");
