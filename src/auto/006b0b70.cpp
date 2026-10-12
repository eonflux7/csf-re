#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Callee bindings (by address, signatures as given in the packet).
CSF_FUNC(unsigned int, __fastcall, S_006b0320_Prepare, 0x006B0B10, (void* self, void* edx));
CSF_FUNC(int, __fastcall, S_006b0320_ApplyArg, 0x006B03B0, (void* self, void* edx, int a2));

// Original: prepares the object, applies the argument, returns the object's address
// (&this->field_0000, which is offset 0 of the struct).
static unsigned int* __fastcall S_006b0320_PrepareAndApply(void* self, void* /*edx*/, int a2)
{
    S_006b0320_Prepare(self, nullptr);
    S_006b0320_ApplyArg(self, nullptr, a2);
    return reinterpret_cast<unsigned int*>(self);
}

HOOK(0x006B0B70, S_006b0320_PrepareAndApply, "auto");
