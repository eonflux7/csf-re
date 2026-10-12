#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Returns the localized action name string for the given index.
// Original: char* __cdecl GetCurrentActionName(int a1) -> g_psz_DE_PIE[a1]
static char* GetCurrentActionName(int actionIndex) {
    const char* const* actionNameTable =
        reinterpret_cast<const char* const*>(CSF_ADDR_DATA(0x8A9610));
    return const_cast<char*>(actionNameTable[actionIndex]);
}

HOOK(0x761390, GetCurrentActionName, "auto");
