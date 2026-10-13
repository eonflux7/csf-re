#include <cstdint>
#include <cstring>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Global: byte offset of the PTank plugin pointer inside the owner object.
CSF_GLOBAL(std::int32_t, g_PTankPluginOffset, 0x008B09EC);

static int __fastcall GfxRenderEngine_ApplyParticleTransform(S_006ada10* self, void* /*edx*/, const void** a2)
{
    // self+4 holds the owner object pointer.
    std::uint8_t* owner = reinterpret_cast<std::uint8_t*>(
        static_cast<std::uintptr_t>(CSF_AT(std::uint32_t, self, 0x4)));

    // Owner stores the PTank plugin pointer at g_PTankPluginOffset.
    std::uint8_t* plugin = reinterpret_cast<std::uint8_t*>(
        static_cast<std::uintptr_t>(CSF_AT(std::uint32_t, owner, g_PTankPluginOffset)));

    // Copy 0x40 bytes of transform data from *a2 into plugin+0xF8.
    std::memcpy(plugin + 0xF8, *a2, 0x40);

    // Reload the plugin pointer and set flag bit 0x8000 at +0x40.
    std::uint8_t* result = reinterpret_cast<std::uint8_t*>(
        static_cast<std::uintptr_t>(CSF_AT(std::uint32_t, owner, g_PTankPluginOffset)));
    CSF_AT(std::uint32_t, result, 0x40) |= 0x8000u;

    return static_cast<int>(reinterpret_cast<std::uintptr_t>(result));
}

HOOK(0x6EF1E0, GfxRenderEngine_ApplyParticleTransform, "auto");
