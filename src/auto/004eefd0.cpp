#include <cstdint>
#include "globals.h"
#include "hook.h"

// Bound callee: CTextDatabase_SetTextDatabasePath (__thiscall, pops 8 bytes).
CSF_FUNC(void, __fastcall, CTextDatabase_SetTextDatabasePath, 0x0069BD90,
         (void* self, void* edx, unsigned int a2, char* a3));

// Sets the text database paths: slot 1 gets "Texts/Menus.fli" (the original pushes the string's
// address, 0x83EE8C, not a pointer stored there), slot 8 gets "Texts/Global.fli".
static void __fastcall CTextDatabase_LoadGlobalTextPaths(void* self, void* /*edx*/)
{
    CTextDatabase_SetTextDatabasePath(self, nullptr, 1u,
                                    (char*)CSF_STR(0x0083EE8C, "Texts/Menus.fli"));
    CTextDatabase_SetTextDatabasePath(self, nullptr, 8u,
                                    (char*)CSF_STR(0x0083EE78, "Texts/Global.fli"));
}

HOOK(0x004EEFD0, CTextDatabase_LoadGlobalTextPaths, "auto");
