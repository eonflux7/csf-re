#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Returns the fixed class/type name string associated with NetCogible.
static const char* __fastcall NetCogible_GetName(NetCogible* self, void* /*edx*/)
{
    (void)self;
    return CSF_STR(0x00867960, "NetCogible");
}

HOOK(0x741FF0, NetCogible_GetName, "auto");
