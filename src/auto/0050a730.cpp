#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CoreZlibLog_BeginGroup, 0x00695470, (void* self, void* edx, char* a2, int a3, char a4));
CSF_FUNC(unsigned int, __fastcall, CoreZlibLog_EndGroup_695260, 0x00695260, (void* self, void* edx));
CSF_FUNC(unsigned int, __fastcall, CoreZlibLog_EndGroup_6951D0, 0x006951D0, (void* self, void* edx));
CSF_FUNC(unsigned int, __cdecl, CheatsLog_WriteControlEntries, 0x00428900, (S_00694cd0* logger, int* entries, unsigned int entryCount));

CSF_GLOBAL(const char*, g_pszKey_CONTROLS, 0x00899A94);
CSF_GLOBAL(const char*, g_pszKey_CONTROLES, 0x00899A44);

// Writes the controls section of the options file: opens the CONTROLS and
// CONTROLES groups, dumps 0x20 control entries starting at this+4, then closes both groups.
static char __fastcall COptionsFile_WriteControlsSection(uint32_t* self, void* /*edx*/, S_00694cd0* a2)
{
    CoreZlibLog_BeginGroup(a2, nullptr, const_cast<char*>(g_pszKey_CONTROLS), 0, 0);
    CoreZlibLog_BeginGroup(a2, nullptr, const_cast<char*>(g_pszKey_CONTROLES), 1, 0);
    CheatsLog_WriteControlEntries(a2, reinterpret_cast<int*>(reinterpret_cast<uint8_t*>(self) + 4), 0x20u);
    CoreZlibLog_EndGroup_695260(a2, nullptr);
    CoreZlibLog_EndGroup_6951D0(a2, nullptr);
    return 1;
}

HOOK(0x50A730, COptionsFile_WriteControlsSection, "auto");
