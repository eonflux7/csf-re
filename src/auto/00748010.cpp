#include <cstdint>
#include "globals.h"
#include "hook.h"

static int __fastcall MpSpawnPoints_ResetStatsCategory(void* self, void* /*edx*/, char category) {
    int result;
    if (category == 1) {
        result = CSF_AT(int, self, 0x44);
        if (result) {
            CSF_AT(int, self, 0x44) = 0;
        }
    } else {
        result = CSF_AT(int, self, 0x50);
        if (result) {
            CSF_AT(int, self, 0x50) = 0;
        }
    }
    return result;
}

HOOK(0x00748010, MpSpawnPoints_ResetStatsCategory, "auto");
