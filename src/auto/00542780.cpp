#include <cstdint>

#include "hook.h"
#include "core/memory.h"
#include "globals.h"

// 0x550CC0: __cdecl, no args; returns the global menu object as an integer (0 when absent).
CSF_FUNC(int, __cdecl, GetGlobal8B0688, 0x550CC0, ());

// 0x542780: fetches the global menu object; if present, fetches it again and clears the byte flag
// at +0x14A2. The original tail-jumps into the tiny setter at 0x551AC0 (not a function start),
// whose body is just:  mov byte ptr [ecx+0x14a2], 0 ; ret  (leaves eax = the fetched object).
// That store is reproduced inline here, with ecx being the result of the second fetch.
static int __cdecl MenuMpCharacterSelect_ClearFlag14A2()
{
    int menuObj = GetGlobal8B0688();
    if (menuObj)
    {
        menuObj = GetGlobal8B0688();
        *reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(menuObj) + 0x14A2u) = 0;
    }
    return menuObj;
}

HOOK(0x542780, MenuMpCharacterSelect_ClearFlag14A2, "auto");
