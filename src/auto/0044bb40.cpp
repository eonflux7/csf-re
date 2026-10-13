#include <cstdint>
#include "globals.h"
#include "hook.h"

// Stores a2 at this+0x1C unless the current value there is 2.
// Returns a2 when the store happens; otherwise the original returns whatever
// was left in eax on entry, which is garbage, so 0 is returned here.
static int __fastcall SetValueUnlessTwo(void* self, void* /*edx*/, int a2) {
    if (CSF_AT(int, self, 0x1C) != 2) {
        CSF_AT(int, self, 0x1C) = a2;
        return a2;
    }
    return 0;
}

HOOK(0x44BB40, SetValueUnlessTwo, "auto");
