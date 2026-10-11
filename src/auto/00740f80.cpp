#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// MpSpawnPoints::SetPlayerStateFlag -- stores the byte flag at +0x6F1 and returns it.
// Original: mov al, [ebp+8]; mov [ecx+0x6F1], al; ret 4  (returns the value in al only).
static char __fastcall MpSpawnPoints_SetPlayerStateFlag(void* self, void* /*edx*/, char value)
{
    CSF_AT(uint8_t, self, 0x6F1) = static_cast<uint8_t>(value);
    return value;
}

HOOK_KEEP_ECX(0x740F80, MpSpawnPoints_SetPlayerStateFlag, 1, "net_cogible_ddl");
