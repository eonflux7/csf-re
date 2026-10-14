#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(uint32_t, __fastcall, Campaign_ClearStrings, 0x0043E360, (uint32_t* self, void*));

// ret 8; each element occupies 0xC4 bytes. EAX is unspecified for an empty range.
static uint32_t __stdcall Destroy(uint32_t* first, uint32_t* last) {
    uint32_t result = 0;
    for (; first != last; first += 49)
        result = Campaign_ClearStrings(first, nullptr);
    return result;
}
HOOK(0x0043F860, Destroy, "stl");
