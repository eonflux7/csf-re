#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"

// 0x004A9EB0 — returns pointer to the game's "TBasico" string literal (at 0x8355DC).
static const char* __cdecl C_4A9EB0_GetTBasico()
{
    return reinterpret_cast<const char*>(CSF_ADDR_DATA(0x8355DC));
}

HOOK(0x4A9EB0, C_4A9EB0_GetTBasico, "auto");
