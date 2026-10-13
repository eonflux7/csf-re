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

// Helpers bound by address. The names are chosen here; the signatures are the packet's.
CSF_FUNC(unsigned int, __fastcall, CoreZlibLog_AppendPlain, 0x00694DC0, (void* self, void* edx, char* a2));
CSF_FUNC(int, __fastcall, CoreZlibLog_AppendFormatted, 0x00694EA0, (void* self, void* edx, char* a2));
CSF_FUNC(int, __fastcall, CoreZlibLog_WriteBracketTag, 0x00695030, (void* self, void* edx, int a2, char* a3, char a4, char a5, int a6));

// Begins a log group: writes a prefix, then the group name, then an opening bracket tag.
// a3 == 0 -> "[" tag, a3 == 1 -> "(" tag, otherwise returns a3 - 1.
static int __fastcall S_00694cd0_BeginGroup(S_00694cd0* self, void* /*edx*/, const char* a2, int a3, char a4)
{
    // Prefix: a space if the indexed entry flag is set, otherwise the indent text at this+0x14.
    const bool entryFlagSet = CSF_AT(uint8_t, self, static_cast<size_t>(self->field_0004) * 8u + 0x11c) != 0;
    if (entryFlagSet)
    {
        CoreZlibLog_AppendPlain(self, nullptr, const_cast<char*>(CSF_STR(0x0083F284, " ")));
    }
    else
    {
        CoreZlibLog_AppendPlain(self, nullptr,
            reinterpret_cast<char*>(reinterpret_cast<uint8_t*>(self) + 0x14));
    }

    // Group name: raw append when a4 is set, formatted append otherwise.
    if (a4)
    {
        CoreZlibLog_AppendPlain(self, nullptr, const_cast<char*>(a2));
    }
    else
    {
        CoreZlibLog_AppendFormatted(self, nullptr, const_cast<char*>(a2));
    }

    if (a3 == 0)
    {
        return CoreZlibLog_WriteBracketTag(self, nullptr, 0, const_cast<char*>(CSF_STR(0x008446C4, "[")), a4, 0, 0);
    }
    if (a3 == 1)
    {
        return CoreZlibLog_WriteBracketTag(self, nullptr, 1, const_cast<char*>(CSF_STR(0x00838FFC, "(")), a4, 0, 0);
    }
    return a3 - 1;
}

HOOK(0x00695470, S_00694cd0_BeginGroup, "auto");
