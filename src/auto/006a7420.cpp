#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Allocates a 4-byte context object from the game heap and stamps its vtable.
// Returns nullptr if the allocation fails.

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));

static uint32_t* __cdecl NetGathering_CreateAccountEditContext()
{
    uint32_t* context = static_cast<uint32_t*>(Mem_Alloc(4));
    if (context == nullptr)
        return nullptr;
    *context = reinterpret_cast<uint32_t>(CSF_VTBL(0x85FF44));
    return context;
}

HOOK(0x006A7420, NetGathering_CreateAccountEditContext, "auto");
