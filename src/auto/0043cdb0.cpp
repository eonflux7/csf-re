#include <cstdint>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Returns an object/struct pointer used by this accessor.
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());

// Loads the single-precision float stored at offset 0x974 (2420) of the
// object returned by GetValue8B0554 and returns it (as a double, matching
// the original x87 `fld dword ptr` / return-in-st(0) semantics).
static double __cdecl GetValue8B0554_Offset974()
{
    return *reinterpret_cast<float*>(static_cast<uintptr_t>(GetValue8B0554()) + 2420);
}

HOOK(0x0043CDB0, GetValue8B0554_Offset974, "auto");
