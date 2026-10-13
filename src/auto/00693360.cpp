#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Tail-jumps to FindProperty(this, keyword). The callee pops the one stack
// argument (ret 4), so this wrapper is __fastcall with one stack argument too.
CSF_FUNC(std::uint8_t*, __fastcall, FindProperty, 0x006932B0, (std::uint8_t* self, void* edx, char* keyword));

static S_00692b80* __fastcall LookupNamedElement(S_00692b80* self, void* /*edx*/, char* String1)
{
    return reinterpret_cast<S_00692b80*>(FindProperty(reinterpret_cast<std::uint8_t*>(self), nullptr, String1));
}

HOOK(0x00693360, LookupNamedElement, "auto");
