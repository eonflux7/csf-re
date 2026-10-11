#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Returns the nested-thread-subscription count stored at +0x0C4.
// Original: 006cad10  mov eax, [ecx+0xc4]; ret
static C_008622e8* __fastcall C_00860bb8_GetNumNestedThreadSubscriptions(C_00860bb8* self, void* /*edx*/) {
    return reinterpret_cast<C_008622e8*>(self->field_00c4);
}

HOOK(0x006CAD10, C_00860bb8_GetNumNestedThreadSubscriptions, "auto");
