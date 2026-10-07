#include <cstdint>
#include <cstddef>
#include "hook.h"

// Target: 0x004E9FF0 (7 bytes)
//
// char *__thiscall sub_4E9FF0(char *this)
// {
//   return this - 8124;
// }
//
// Disassembly:
//   004e9ff0  lea eax, [ecx - 0x1fbc]
//   004e9ff6  ret
//
// Plain pointer-adjustment thunk: given a pointer to an embedded sub-object
// located at offset 0x1FBC inside a larger object, it returns a pointer to the
// containing (base) object. Classic "__thiscall" adjustor converting an inner
// member pointer back to its owner.

// Returns the address of the object that owns the sub-object currently
// pointed to by `self` (i.e. `self - 0x1FBC`).
static char* __fastcall EmbeddedSubobjectToBase(char* self, void* /*edx*/)
{
    return self - 0x1FBC;
}

HOOK(0x4E9FF0, EmbeddedSubobjectToBase, "auto");
