#include <cstdint>
#include "globals.h"
#include "hook.h"

// 0x0078BB40: __fastcall helper (ecx = this, edx unused, one stack arg), ret 4.
CSF_FUNC(int, __fastcall, BddTables_ResolveEntry, 0x0078BB40, (void* self, void* edx, int a2));

// 0x0051B4B0: bare `jmp 0x78bb40`. The original never writes ecx, so the callee receives
// whatever ecx the caller passed in. Declaring this as __fastcall exposes ecx and edx
// unchanged, while the single stack argument and `ret 4` match the original's frame.
static int __fastcall ForwardToTableResolve(void* self, void* edx, int a2)
{
    return BddTables_ResolveEntry(self, edx, a2);
}

HOOK(0x0051B4B0, ForwardToTableResolve, "auto");
