#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// void *__thiscall sub_4B2980(void *this) — returns the outer object for a sub-object pointer.
CSF_FUNC(void*, __fastcall, C_008310b8_GetSelf, 0x4B2980, (void* self, void* edx));

// Adjustor thunk: the incoming `this` points at a sub-object located 4 bytes into the
// real object, so back the pointer up by 4 before invoking GetSelf.
static void* __fastcall C_008310b8_GetSelfFromSubobject(char* self, void* /*edx*/)
{
    return C_008310b8_GetSelf(self - 4, nullptr);
}

HOOK(0x48EDF0, C_008310b8_GetSelfFromSubobject, "auto");
