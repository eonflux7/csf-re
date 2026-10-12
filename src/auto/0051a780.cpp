#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int*, __cdecl, BddTables_ForwardTableOperation, 0x0051A220,
         (int a1, int a2, int* a3, int a4, int a5));

static int* __fastcall BddTables_ForwardTableOperationWithSelf(
    void* self, void* /*edx*/, int a1, int a2, int* a3)
{
    return BddTables_ForwardTableOperation(
        a1, a2, a3,
        static_cast<int>(reinterpret_cast<std::uintptr_t>(self)),
        static_cast<int>(reinterpret_cast<std::uintptr_t>(a3)));
}

HOOK(0x0051A780, BddTables_ForwardTableOperationWithSelf, "auto");
