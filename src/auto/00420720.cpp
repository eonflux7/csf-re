#include "globals.h"
#include "hook.h"

// Returns the pointer to the constant animation/state name "TMoviendose"
// (Spanish: "moving"), stored in read-only data at 0x0082F190.
static const char* __cdecl GetMoviendoseName()
{
    return CSF_STR(0x0082F190, "TMoviendose");
}

HOOK(0x00420720, GetMoviendoseName, "auto");
