#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Original calls S_00692b80_FindProperty and sub_782610 by address.
CSF_FUNC(S_00692b80*, __fastcall, S_00692b80_FindProperty, 0x006932B0, (S_00692b80* self, void* , char* name));
CSF_FUNC(void, __stdcall, sub_782610, 0x00782610, (void* a1, int a2));

CSF_GLOBAL(const char*, g_pszKey_FACIL, 0x008A9C14);
CSF_GLOBAL(const char*, g_pszKey_NORMAL, 0x008A9C18);
CSF_GLOBAL(const char*, g_pszKey_DIFICIL, 0x008A9C1C);
CSF_GLOBAL(const char*, g_pszKey_COMANDO, 0x008A9C20);

// Original: __stdcall with ecx carrying the object pointer (passed on to sub_782610).
// Written as __fastcall: ecx = self, edx unused, two stack args -> ret 8.
static void __fastcall ApplyPropertySetsWithOffsets(void* self, void* /*edx*/, S_00692b80* a1, int a2)
{
    S_00692b80* property = S_00692b80_FindProperty(a1, nullptr, const_cast<char*>(g_pszKey_FACIL));
    if (property)
        sub_782610(property, a2);

    property = S_00692b80_FindProperty(a1, nullptr, const_cast<char*>(g_pszKey_NORMAL));
    if (property)
        sub_782610(property, a2 + 20);

    property = S_00692b80_FindProperty(a1, nullptr, const_cast<char*>(g_pszKey_DIFICIL));
    if (property)
        sub_782610(property, a2 + 40);

    property = S_00692b80_FindProperty(a1, nullptr, const_cast<char*>(g_pszKey_COMANDO));
    if (property)
        sub_782610(property, a2 + 60);
}

HOOK(0x007826A0, ApplyPropertySetsWithOffsets, "auto");
