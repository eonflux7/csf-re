#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_GLOBAL(uint32_t, g_008B0678, 0x008B0678);

// Releases the cached sound instance held in g_008B0678: calls its vtable slot 0
// with flag 1 (destroy-with-free), then clears the global.
static int __cdecl SoundManager_ReleaseActiveSound()
{
    int result = 0;
    if (g_008B0678 != 0)
    {
        void* soundInstance = reinterpret_cast<void*>(g_008B0678);
        result = ((int(__fastcall*)(void*, void*, int))CSF_AT(void**, soundInstance, 0)[0])(soundInstance, nullptr, 1);
        g_008B0678 = 0;
    }
    return result;
}

HOOK(0x00525140, SoundManager_ReleaseActiveSound, "auto");
