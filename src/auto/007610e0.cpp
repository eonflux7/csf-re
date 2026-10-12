#include "hook.h"

// Returns non-zero when the region code is one of the nine codes the game
// treats as a special region (values 6, 7, 10, 16, 17, 18, 24, 25, 32).
static int __cdecl AppConfig_IsSpecialRegionCode(int regionCode) {
    return regionCode == 6 || regionCode == 7 || regionCode == 10 ||
           regionCode == 16 || regionCode == 17 || regionCode == 18 ||
           regionCode == 24 || regionCode == 25 || regionCode == 32;
}

HOOK(0x7610E0, AppConfig_IsSpecialRegionCode, "auto");
