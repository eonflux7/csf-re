#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, Sub_64B250, 0x0064B250, (void* self, void* edx));

// Stores the 16-bit value in the menu's slot at +0x452 when Sub_64B250 reports true.
// Returns the value when stored, otherwise the (zero) result of the check.
static char __fastcall CMenuMpStats_StoreValueIfReady(CMenuMpStats* self, void* /*edx*/, int16_t value)
{
    char result = static_cast<char>(Sub_64B250(self, nullptr));
    if (result) {
        CSF_AT(int16_t, self, 0x452) = value;
        return static_cast<char>(value);
    }
    return result;
}

HOOK(0x740A80, CMenuMpStats_StoreValueIfReady, "auto");
