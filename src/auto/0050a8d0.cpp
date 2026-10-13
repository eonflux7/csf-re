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

// CoreZlibLog serializer (writes "key = value" pairs into a CSF Data\Options log).
CSF_FUNC(int, __fastcall, CoreZlibLog_BeginGroup, 0x00695470, (void* self, void* edx, char* a2, int a3, char a4));
CSF_FUNC(unsigned int, __fastcall, CoreZlibLog_WriteFloat, 0x00695330, (void* self, void* edx, float a2, char* a3));
CSF_FUNC(unsigned int, __fastcall, CoreZlibLog_WriteInteger, 0x006952F0, (void* self, void* edx, int a2, char* a3));
CSF_FUNC(unsigned int, __fastcall, CoreZlibLog_EndGroup_6951D0, 0x006951D0, (void* self, void* edx));

// Option key strings (const char* globals in .data).
CSF_GLOBAL(const char*, g_pszKey_SOUND, 0x00899A8C);
CSF_GLOBAL(const char*, g_pszKey_MUSIC, 0x00899A48);
CSF_GLOBAL(const char*, g_pszKey_FX, 0x00899A4C);
CSF_GLOBAL(const char*, g_pszKey_OUTPUT, 0x00899A50);
CSF_GLOBAL(const char*, g_pszKey_REVERB, 0x00899A54);
CSF_GLOBAL(const char*, g_pszKey_EAX, 0x00899A58);

// Writes the [SOUND] group of the options file from the sound settings block at self.
// Layout: +0x04 music volume (float), +0x08 fx volume (float), +0x0C output mode (int),
// +0x10 reverb flag (byte), +0x11 EAX flag (byte).
static bool __fastcall OptionsFile_WriteSoundSection(void* self, void* /*edx*/, S_00694cd0* a2)
{
    CoreZlibLog_BeginGroup(a2, nullptr, const_cast<char*>(g_pszKey_SOUND), 0, 0);
    CoreZlibLog_WriteFloat(a2, nullptr, CSF_AT(float, self, 0x4), const_cast<char*>(g_pszKey_MUSIC));
    CoreZlibLog_WriteFloat(a2, nullptr, CSF_AT(float, self, 0x8), const_cast<char*>(g_pszKey_FX));
    CoreZlibLog_WriteInteger(a2, nullptr, CSF_AT(int, self, 0xC), const_cast<char*>(g_pszKey_OUTPUT));
    CoreZlibLog_WriteInteger(a2, nullptr, (CSF_AT(uint8_t, self, 0x10) != 0) ? 1 : 0, const_cast<char*>(g_pszKey_REVERB));
    CoreZlibLog_WriteInteger(a2, nullptr, (CSF_AT(uint8_t, self, 0x11) != 0) ? 1 : 0, const_cast<char*>(g_pszKey_EAX));
    CoreZlibLog_EndGroup_6951D0(a2, nullptr);
    return true;
}

HOOK(0x0050A8D0, OptionsFile_WriteSoundSection, "auto");
