#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"

// Original callees, bound by address.
CSF_FUNC(int, __cdecl, MenuExtras_ParseDescriptionId, 0x00698D90, (const char* text));
CSF_FUNC(int, __cdecl, MenuMessageBox_ResolveEntryByKey, 0x00692440, (int code, int flag));

// Parses the text description id, then resolves the resulting code through the
// message box table with the caller's flag. Returns the resolver's result.
static int __cdecl Text_ConvertToDisplayEncoding(int text, int flag)
{
    int parsedCode = MenuExtras_ParseDescriptionId(reinterpret_cast<const char*>(text));
    return MenuMessageBox_ResolveEntryByKey(parsedCode, flag);
}

HOOK(0x00692580, Text_ConvertToDisplayEncoding, "auto");
