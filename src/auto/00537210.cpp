// Reimplementation of sub_537210 (0x00537210)
// Simple getter: returns the byte stored at offset 0x183 of the object.

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

// __thiscall getter: one byte field at offset 0x183, no stack arguments (ret 0).
// Written as __fastcall (ecx = this, edx unused).
static char __fastcall C_Unknown_GetField0183(void* self, void* /*edx*/)
{
    return static_cast<char>(CSF_AT(uint8_t, self, 0x183));
}

HOOK(0x537210, C_Unknown_GetField0183, "auto");
