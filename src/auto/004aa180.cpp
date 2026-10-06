#include <cstdint>
#include "globals.h"
#include "hook.h"

// 0x004AA180 (6 bytes) — returns the literal action/tag name "TAsomarse".
// mov eax, 0x837b94 ; ret
static const char* __cdecl GetTAsomarseString()
{
    return "TAsomarse";
}

HOOK(0x4AA180, GetTAsomarseString, "auto");
