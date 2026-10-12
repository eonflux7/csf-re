#include <cstdint>
#include "hook.h"

// Returns 1 when the item id allows a direct action, else 0.
static int __cdecl AppConfig_IsItemDirectActionAllowed(int itemId) {
    if (itemId == 6 || itemId == 7 || itemId == 10 || itemId == 16 ||
        itemId == 17 || itemId == 18 || itemId == 24 || itemId == 25 ||
        itemId == 32 || itemId == 23 || itemId == 22 || itemId == 27 ||
        itemId == 26) {
        return 1;
    }
    return 0;
}

HOOK(0x761130, AppConfig_IsItemDirectActionAllowed, "auto");

