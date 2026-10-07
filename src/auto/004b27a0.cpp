#include "globals.h"
#include "hook.h"

// Returns the class/type name string of the "TPrcFuegoCobertura" processing object.
// Original: mov eax, 0x83A648 ; ret  (returns pointer to the constant string)
static const char* __cdecl FuegoCobertura_GetTypeName()
{
    return CSF_STR(0x83A648, "TPrcFuegoCobertura");
}

HOOK(0x4B27A0, FuegoCobertura_GetTypeName, "auto");
