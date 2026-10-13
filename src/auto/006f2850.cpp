#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Stores the intro page index/value at this+0x24 and returns it.
static int __fastcall MenuManager_SetIntroPageData(C_0085ec18* self, void* /*edx*/, int a2) {
    CSF_AT(int, self, 0x24) = a2;
    return a2;
}

HOOK(0x006F2850, MenuManager_SetIntroPageData, "auto");
