#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, Sub_64B250, 0x0064B250, (void* self, void* edx));

// Stores four stack arguments into CMenuMpStats slots 0xC0..0xCC and sets the flag at 0xD0,
// but only when the base-class readiness check (Sub_64B250) succeeds.
static int __fastcall CMenuMpStats_StoreArgsIfReady(CMenuMpStats* self, void* /*edx*/,
                                                    int a2, int a3, int a4, int a5) {
    int result = Sub_64B250(self, nullptr);
    if (result) {
        CSF_AT(int, self, 0xC0) = a2;
        CSF_AT(int, self, 0xC4) = a3;
        CSF_AT(int, self, 0xC8) = a4;
        CSF_AT(int, self, 0xCC) = a5;
        CSF_AT(uint8_t, self, 0xD0) = 1;
        return a5;
    }
    return result;
}

HOOK(0x740150, CMenuMpStats_StoreArgsIfReady, "auto");
