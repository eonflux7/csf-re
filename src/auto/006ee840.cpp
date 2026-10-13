#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, CoreMemory_AllocUntagged, 0x006910F0, (std::size_t size));
CSF_FUNC(uint32_t*, __fastcall, C_0085eb30_InitNamedMutex, 0x0069E3E0, (C_0085eb30* self, void* /*edx*/, const char* lpName));

// Allocates an 8-byte C_0085eb30 instance and initializes it with the given name.
// Returns the initialized object, or nullptr when the allocation fails.
static uint32_t* __cdecl GfxEnvironment_GetNamedResource(const char* lpName)
{
    uint32_t allocatedAddress = static_cast<uint32_t>(CoreMemory_AllocUntagged(8));
    if (allocatedAddress != 0)
    {
        C_0085eb30* newResource = reinterpret_cast<C_0085eb30*>(allocatedAddress);
        return C_0085eb30_InitNamedMutex(newResource, nullptr, lpName);
    }
    return nullptr;
}

HOOK(0x006EE840, GfxEnvironment_GetNamedResource, "auto");
