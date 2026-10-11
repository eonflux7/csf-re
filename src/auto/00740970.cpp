#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, Sub_64B250, 0x0064B250, (void* self, void* edx));

// Sets the byte at +0x440 to newValue, but only when Sub_64B250(self) reports true.
// Returns the callee's truth flag if it is false, otherwise newValue.
static char __fastcall CMenuMpStats_SetFlagByte1088IfReady(CMenuMpStats* self, void* /*edx*/, char newValue)
{
    char result = static_cast<char>(Sub_64B250(self, nullptr));
    if (result) {
        result = newValue;
        if (newValue != CSF_AT(char, self, 0x440)) {
            CSF_AT(char, self, 0x440) = newValue;
        }
    }
    return result;
}

HOOK(0x740970, CMenuMpStats_SetFlagByte1088IfReady, "auto");
