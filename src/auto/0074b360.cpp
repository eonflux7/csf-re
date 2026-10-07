#include "globals.h"
#include "hook.h"

// Returns the constant identifier string "NetGhost" (address 0x00868118).
// No stack arguments (ret 0). Used to expose a class/type name, most likely
// as a virtual method returning a tag for the network/ghost entity type.
static const char* __cdecl NetGhost_GetName()
{
    return CSF_STR(0x00868118, "NetGhost");
}

HOOK(0x74B360, NetGhost_GetName, "auto");
