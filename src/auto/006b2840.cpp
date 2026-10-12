#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(uint32_t, g_SkinDelegateIdentifier, 0x008BB460);

static bool __fastcall C_00860178_ResolveDelegate(
    C_00860178* self,
    void* /*edx*/,
    uint32_t* identifier,
    uint32_t* delegateOut)
{
    const uint32_t delegate = *identifier == g_SkinDelegateIdentifier
        ? static_cast<uint32_t>(reinterpret_cast<uintptr_t>(self))
        : 0;

    *delegateOut = delegate;
    return delegate != 0;
}

HOOK(0x006B2840, C_00860178_ResolveDelegate, "auto");
