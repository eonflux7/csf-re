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

// 0x00695680 — helper (cdecl, pops no stack bytes) that assigns/copies the
// text payload into the sub-object passed as its first argument.
CSF_FUNC(int, __cdecl, Sub_695680, 0x695680, (char* a1, char* a2));

// 0x00529030 — CMenuMessageBox method: forwards the argument to the embedded
// text sub-object at +0x90, then raises the flag byte at +0x80 and returns the
// helper's result unchanged (eax is passed straight through).
static int __fastcall CMenuMessageBox_SetText(void* self, void* /*edx*/, int a2)
{
    int result = Sub_695680(
        reinterpret_cast<char*>(static_cast<uint8_t*>(self) + 0x90),
        reinterpret_cast<char*>(static_cast<uintptr_t>(a2)));
    CSF_AT(uint8_t, self, 0x80) = 1;
    return result;
}

HOOK(0x529030, CMenuMessageBox_SetText, "auto");
