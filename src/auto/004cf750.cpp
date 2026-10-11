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

// Outer handler reached by tail jump: takes the outer object and one char argument.
CSF_FUNC(void*, __fastcall, Outer_HandleSubobjectRequest, 0x004A8B90, (void* self, void* edx, char a2));

// Subobject lives at +4 inside C_00835060; step back to the outer object and forward the call unchanged.
static void* __fastcall C_00835060_ForwardFromSubobject(char* self, void* /*edx*/, char a2)
{
    C_00835060* outer = reinterpret_cast<C_00835060*>(self - 4);
    return Outer_HandleSubobjectRequest(outer, nullptr, a2);
}

HOOK(0x004CF750, C_00835060_ForwardFromSubobject, "auto");
