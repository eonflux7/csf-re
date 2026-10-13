#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, CoreMemory_AllocUntagged, 0x006910F0, (std::size_t size));
CSF_FUNC(uint32_t*, __fastcall, C_008639b0_Construct, 0x0070E730, (C_008639b0* self, void* /*edx*/, int a2));

// Allocates a 72-byte C_008639b0 instance and constructs it with the given argument.
// Returns the constructed object, or nullptr when the allocation fails.
static uint32_t* __cdecl Audio_CreateSoundInstance(int soundArg)
{
    uint32_t allocatedAddress = static_cast<uint32_t>(CoreMemory_AllocUntagged(72));
    if (allocatedAddress != 0)
    {
        C_008639b0* newInstance = reinterpret_cast<C_008639b0*>(allocatedAddress);
        return C_008639b0_Construct(newInstance, nullptr, soundArg);
    }
    return nullptr;
}

HOOK(0x0070E910, Audio_CreateSoundInstance, "auto");
