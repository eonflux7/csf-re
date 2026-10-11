#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Virtual slot +0xA4 is vtbl[41]. The typed wrapper vSlotA4 is declared void,
// but the original uses its int return value, so call the slot directly.
static int __fastcall PairLink_PrepareForRebind(C_0083e9d0* self, void* /*edx*/)
{
    int result = ((int(__fastcall*)(void*, void*))CSF_AT(void**, self, 0)[0xA4 / 4])(self, nullptr);
    ++self->field_001c;
    return result;
}

HOOK(0x4E55A0, PairLink_PrepareForRebind, "auto");
