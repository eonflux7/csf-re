// Reimplementation of sub_4214E0 (0x004214E0)
// Returns the pointer to the localized tag string "TNavegando" that lives in the
// game's read-only data section (0x0082F910). The original is a 6-byte stub which
// simply loads that address into EAX and returns it.

#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Returns the game's own "TNavegando" string constant.
// (Returns the original address rather than a DLL-local copy so that pointer
// identity with the rest of the game stays intact.)
static const char* GetNavegandoString()
{
    return reinterpret_cast<const char*>(0x0082F910);
}

HOOK(0x4214E0, GetNavegandoString, "auto");
