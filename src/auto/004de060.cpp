#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// C_00867260::GetField0084: __fastcall (ecx = this), returns a pointer, pops nothing.
CSF_FUNC(uint8_t*, __fastcall, C_00867260_GetField0084, 0x0048E450, (C_00867260* self, void* edx));

// Per-entry worker on the S_004ddf20 side: __thiscall(ecx = this), one stack arg, ret 4.
CSF_FUNC(int, __fastcall, sub_4DE010, 0x004DE010, (void* self, void* edx, int a2));

// Returns 1 when the entry is enabled and this owner has a bound field; otherwise 0.
static char __fastcall S_004ddf20_ApplyFlaggedEntry(S_004ddf20* self, void* /*edx*/, C_00867260* a2)
{
    // Byte flag at a2 + 0x174 gates the whole operation.
    if (CSF_AT(uint8_t, a2, 0x174) == 0)
        return 0;

    if (self->field_001c != 0)
    {
        uint8_t* entry = C_00867260_GetField0084(a2, nullptr);
        sub_4DE010(self, nullptr, static_cast<int>(reinterpret_cast<uintptr_t>(entry)));
    }
    return 1;
}

HOOK(0x004DE060, S_004ddf20_ApplyFlaggedEntry, "auto");
