#include <cstdint>
#include "globals.h"
#include "hook.h"

// 0x004AA010 (6 bytes)
// Original:
//   mov eax, 0x836924   ; address of the constant string "TPausa"
//   ret                 ; __cdecl, pops no stack bytes
//
// Returns a pointer to the constant string "TPausa" that lives in the game's
// read-only data at 0x836924.  We return the very same address the original
// did so that any pointer-identity checks behave identically.
static const char* GetPausaTag()
{
    return reinterpret_cast<const char*>(CSF_ADDR_DATA(0x836924));
}

HOOK(0x4AA010, GetPausaTag, "auto");
