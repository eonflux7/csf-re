#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void*));
static C_0085e65c* __fastcall C_0085e65c_Delete(C_0085e65c* self, void*, char flags) {
    self->vtbl = CSF_VTBL(0x0085E65C);
    if (flags & 1) CoreMemory_Free(self);
    return self;
}
HOOK(0x006923E0, C_0085e65c_Delete, "auto");
