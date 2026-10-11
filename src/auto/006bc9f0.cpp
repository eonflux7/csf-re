#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Returns the dword at position `index` of the table pointed to by the
// object's field at +0x0C (each entry is one dword).
static int __fastcall CGuiBuilder_GetEntryByIndex(CGuiBuilder* self, void* /*edx*/, int index) {
    int* table = reinterpret_cast<int*>(static_cast<uintptr_t>(self->field_000c));
    return table[index];
}

HOOK(0x006BC9F0, CGuiBuilder_GetEntryByIndex, "auto");
