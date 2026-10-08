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

// Resolves a key (two stack args from the caller) to an entry pointer; NULL on failure.
CSF_FUNC(void*, __cdecl, MenuMessageBox_ResolveEntryByKey, 0x00692440, (int a1, int a2));

// Tests the entry against the flag byte at 0x0082E65C; nonzero means pass.
CSF_FUNC(int, __cdecl, MenuMessageBox_TestEntryAgainstFlag, 0x00695500, (uint16_t* entry, char* flagByte));

// Attaches the entry into the sub-object slot; returns eax as the callee left it.
CSF_FUNC(int, __cdecl, Sub_695680, 0x00695680, (char* slot, char* entry));

CSF_GLOBAL(uint8_t, g_byte_82E65C, 0x0082E65C);

// Thiscall (ecx = this, two stack args, ret 8).
// Resolves an entry; if it passes the flag test, stores it in the sub-object at +0x90
// and sets the byte flag at +0x80. Returns the last eax value, as the original does.
static int __fastcall CMenuMessageBox_TryAttachResolvedEntry(void* self, void* /*edx*/, int a2, int a3)
{
    void* entry = MenuMessageBox_ResolveEntryByKey(a2, a3);
    if (entry == nullptr)
    {
        return 0;
    }

    int flagResult = MenuMessageBox_TestEntryAgainstFlag(
        reinterpret_cast<uint16_t*>(entry),
        reinterpret_cast<char*>(&g_byte_82E65C));
    if (flagResult == 0)
    {
        return flagResult;
    }

    int result = Sub_695680(
        reinterpret_cast<char*>(static_cast<uint8_t*>(self) + 0x90),
        reinterpret_cast<char*>(entry));
    CSF_AT(uint8_t, self, 0x80) = 1;
    return result;
}

HOOK(0x00529060, CMenuMessageBox_TryAttachResolvedEntry, "auto");
