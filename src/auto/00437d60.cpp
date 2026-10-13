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

// CompareStringSubstring: __fastcall, 4 stack args (ret 16).
CSF_FUNC(int, __fastcall, CompareStringSubstring, 0x0042E200,
         (void* string, void* edx, unsigned int position, unsigned int count,
          const char* other, unsigned int otherCount));

// Length of a NUL-terminated string, computed inline as the original does
// (the original does not call strlen; no CRT call is added here).
static unsigned int MeasureStringLengthInline(const char* text) {
    const char* cursor = text;
    while (*cursor != '\0') {
        ++cursor;
    }
    return static_cast<unsigned int>(cursor - text);
}

// Returns 1 when the named effect's string at object offset 0x14 matches
// the given name (substring compare starting at position 0), else 0.
static int __cdecl FxParticleManager_HasNamedEffect(int a1, const char* a2) {
    void* self = reinterpret_cast<void*>(a1);
    unsigned int nameLength = MeasureStringLengthInline(a2);
    int matchResult = CompareStringSubstring(self,
                                             const_cast<char*>(a2),
                                             0,
                                             CSF_AT(uint32_t, self, 0x14),
                                             a2,
                                             nameLength);
    return matchResult != 0 ? 1 : 0;
}

HOOK(0x437D60, FxParticleManager_HasNamedEffect, "auto");
