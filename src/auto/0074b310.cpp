#include <cstdint>
#include "globals.h"
#include "hook.h"

// Bind as (from packet): GameConfig_WriteDwordField(void* self, void* edx, char a2)
CSF_FUNC(int, __fastcall, GameConfig_WriteDwordField, 0x006563F0, (void* self, void* edx, char a2));

// Forwards the first stack argument as `self` (ecx) and the dword stored at the
// object's first field as the single stack argument. The remaining three
// stack arguments are accepted but unused, matching the original's `ret 0x10`.
static int __fastcall NetPlayerDDL_ProcessForwardedArguments(uint32_t* self, void* /*edx*/,
                                                             uint32_t a2, uint32_t a3,
                                                             uint32_t a4, uint32_t a5)
{
    return GameConfig_WriteDwordField(reinterpret_cast<void*>(a2), nullptr,
                                      static_cast<char>(*self));
}

HOOK(0x0074B310, NetPlayerDDL_ProcessForwardedArguments, "auto");
