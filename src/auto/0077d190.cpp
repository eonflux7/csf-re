#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, ReleaseElement, 0x00778140, (C_0082d410* self, void*));

static uint32_t __stdcall DestroyOld_0077d190(uint32_t begin, uint32_t end)
{
    // Empty ranges have no defined result; otherwise preserve the whole EAX.
    uint32_t result = 0;
    for (; begin != end; begin += 72u)
        result = ReleaseElement(reinterpret_cast<C_0082d410*>(begin), nullptr);
    return result;
}
HOOK(0x0077D190, DestroyOld_0077d190, "stl");
