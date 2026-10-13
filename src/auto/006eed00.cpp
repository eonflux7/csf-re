#include <cstdint>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(std::int32_t, g_PTankPluginOffset, 0x008B09EC);

static int __fastcall PTank_GetPluginFieldPlus216(S_006ada10* self, void* /*edx*/)
{
    // self+4 holds a pointer to the PTank plugin block; the plugin field lives at
    // a global, runtime-determined offset inside that block.
    std::uint32_t blockAddress = CSF_AT(std::uint32_t, self, 0x4);
    std::uint8_t* block = reinterpret_cast<std::uint8_t*>(static_cast<std::uintptr_t>(blockAddress));
    int pluginFieldValue = *reinterpret_cast<int*>(block + g_PTankPluginOffset);
    return pluginFieldValue + 0xD8;
}

HOOK(0x006EED00, PTank_GetPluginFieldPlus216, "auto");
