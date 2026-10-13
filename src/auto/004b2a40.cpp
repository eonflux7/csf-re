#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, C_0082d410_SetVtable, 0x004DBD40, (C_0082d410*, void*));
CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void*));
static TFncInteractivo* __fastcall TFncInteractivo_Delete(TFncInteractivo* self, void*, char flags) {
    CSF_AT(void*, self, 0x14) = CSF_VTBL(0x0082D410);
    for (int offset = 16; offset >= 0; offset -= 4)
        C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(reinterpret_cast<char*>(self)+offset), nullptr);
    if (flags & 1) CoreMemory_Free(self);
    return self;
}
HOOK(0x004B2A40, TFncInteractivo_Delete, "auto");
