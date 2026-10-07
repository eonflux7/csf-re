#include <cstdint>
#include "types/auto_classes.h"
#include "hook.h"

// 0x00728640 (__thiscall, this = C_00865870*)
// Queries virtual slot 6 (offset 0x18) with no arguments; if it returns non-zero,
// clears field_0044 (written as a 32-bit 0 store) and flag field_0048, then
// dispatches to virtual slot 31 (offset 0x7c) with argument 0 and returns its result.
static int __fastcall C_00865870_CheckAndResetState(C_00865870* self, void* /*edx*/)
{
    int canReset = reinterpret_cast<int(__fastcall*)(void*, void*)>(
                       self->vtbl[6])(self, nullptr);
    if (canReset == 0)
        return 0;

    // Reload the vtable after the first call (the object may have been mutated).
    void** vtbl = self->vtbl;
    self->field_0044 = 0.0f;   // mov dword ptr [esi+0x44], 0
    self->field_0048 = 1;      // mov byte ptr [esi+0x48], 1
    return reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
               vtbl[31])(self, nullptr, 0);
}

HOOK(0x00728640, C_00865870_CheckAndResetState, "auto");
