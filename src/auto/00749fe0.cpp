#include <cstdint>
#include "globals.h"

#include "hook.h"
#include "core/memory.h"

// Resets a player-registration table: marks byte 0 as 1 and fills 17 dword slots
// at +0x04..+0x44 with -1 (no registered player). Returns -1.
static int __fastcall MpSpawnPoints_ClearRegisteredPlayers(void* self, void* /*edx*/) {
    CSF_AT(uint8_t, self, 0x00) = 1;
    CSF_AT(int32_t, self, 0x44) = -1;
    CSF_AT(int32_t, self, 0x40) = -1;
    CSF_AT(int32_t, self, 0x3C) = -1;
    CSF_AT(int32_t, self, 0x38) = -1;
    CSF_AT(int32_t, self, 0x34) = -1;
    CSF_AT(int32_t, self, 0x30) = -1;
    CSF_AT(int32_t, self, 0x2C) = -1;
    CSF_AT(int32_t, self, 0x28) = -1;
    CSF_AT(int32_t, self, 0x24) = -1;
    CSF_AT(int32_t, self, 0x20) = -1;
    CSF_AT(int32_t, self, 0x1C) = -1;
    CSF_AT(int32_t, self, 0x18) = -1;
    CSF_AT(int32_t, self, 0x14) = -1;
    CSF_AT(int32_t, self, 0x10) = -1;
    CSF_AT(int32_t, self, 0x0C) = -1;
    CSF_AT(int32_t, self, 0x08) = -1;
    CSF_AT(int32_t, self, 0x04) = -1;
    return -1;
}

HOOK_KEEP_ECX(0x749FE0, MpSpawnPoints_ClearRegisteredPlayers, 0, "net_ddl_object_manager");
