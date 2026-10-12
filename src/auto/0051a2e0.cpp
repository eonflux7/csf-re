#include "hook.h"
#include "globals.h"

static CSF_FUNC(int, __cdecl, ForwardTableOperation, 0x0051A1C0,
                (int a1, int a2, int a3, int a4));

static int __cdecl BddTables_ForwardOperation(int a1, int a2, int a3)
{
    // The original forwards an uninitialized stack slot as its fourth argument.
    return ForwardTableOperation(a1, a2, a3, 0);
}

HOOK(0x0051A2E0, BddTables_ForwardOperation, "auto");
