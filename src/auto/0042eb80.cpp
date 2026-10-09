#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

// Returns 0 when the counter field_0290 is 0 or 1 (unsigned compare, jbe).
// Otherwise returns the dword at offset +4 of the block pointed to by field_0288.
static int __fastcall CHud_GetLinkedWordIfCountAbove1(CHud* self, void* /*edx*/)
{
    if (self->field_0290 <= 1u)
        return 0;

    return static_cast<int>(*reinterpret_cast<uint32_t*>(self->field_0288 + 4));
}

HOOK(0x0042EB80, CHud_GetLinkedWordIfCountAbove1, "auto");
