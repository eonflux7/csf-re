#include "hook.h"
#include "types/auto_classes.h"

// Returns the literal name of the "crouch" mode ("TModoAgacharse").
// Original: mov eax, 0x832C90 (points at the game's static string); ret
static const char* __fastcall C_00832bf8_GetCrouchModeName(C_00832bf8* /*self*/, void* /*edx*/)
{
    return "TModoAgacharse";
}

HOOK(0x47BDC0, C_00832bf8_GetCrouchModeName, "auto");
