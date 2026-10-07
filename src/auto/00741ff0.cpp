#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Returns the fixed class/type name string associated with C_00867a58.
static const char* __fastcall C_00867a58_GetName(C_00867a58* self, void* /*edx*/)
{
    (void)self;
    return CSF_STR(0x00867960, "NetCogible");
}

HOOK(0x741FF0, C_00867a58_GetName, "auto");
