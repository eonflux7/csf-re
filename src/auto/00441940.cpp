#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Original 0x00441520: __thiscall(this, const char* fileName), callee pops 4 bytes.
CSF_FUNC(char, __fastcall, GameBddLoader_ParseResourceScript, 0x00441520, (void* self, void* edx, void* a2));

// 0x00441940: loads the campaign resource script "Menus/campanya.fbs" for this loader.
// Original ret 0, so this __thiscall-style method pops nothing.
static char __fastcall GameBddLoader_LoadResources(S_00441520* self, void* /*edx*/)
{
    // The original references S_00441520's vtable address at the call site.
    (void)CSF_VTBL(0x00441520);

    const char* scriptName = CSF_STR(0x00831024, "Menus/campanya.fbs");
    return GameBddLoader_ParseResourceScript(self, nullptr,
                                             reinterpret_cast<void*>(const_cast<char*>(scriptName)));
}

HOOK(0x00441940, GameBddLoader_LoadResources, "auto");
