#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// sub_4DBFE0: __thiscall-style callee (ecx = this, edx = slot value, one stack arg).
CSF_FUNC(void, __fastcall, sub_4DBFE0, 0x004DBFE0, (void* self, void* edx, int a2));

// TAiState ctor helper: the object is passed in ecx with no `this` use in the caller;
// the caller pops its single stdcall argument (ret 4).
static void __stdcall PrepareStaticAiState(void* a1)
{
    // a1 + 0x0C holds a pointer to a table-like struct; its dword at +0x10 is the flag.
    void* table = CSF_AT(void*, a1, 0x0C);
    if (table == nullptr)
        return;

    uint32_t flagValue = CSF_AT(uint32_t, table, 0x10);
    if (flagValue == 0)
        return;

    sub_4DBFE0(a1, reinterpret_cast<void*>(static_cast<uintptr_t>(flagValue)), 0);
}

HOOK(0x004DC3C0, PrepareStaticAiState, "auto");
