#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, C_0082d410_SetVtable, 0x004DBD40, (C_0082d410* self, void* edx));

// Resolves the embedded sub-object at byte offset `offset` from the task object.
static C_0082d410* SubobjectAtOffset(TFncAtacarDesdeParapeto* self, uint32_t offset)
{
    return reinterpret_cast<C_0082d410*>(reinterpret_cast<uint8_t*>(self) + offset);
}

// Resets the vtable of each of the ten embedded sub-objects (offsets 0x24 down to 0x00),
// in the original order. The final SetVtable on `self` itself is the original's tail jump.
static int __fastcall TFncAtacarDesdeParapeto_DestroySubobjectVtables(TFncAtacarDesdeParapeto* self, void* /*edx*/)
{
    C_0082d410_SetVtable(SubobjectAtOffset(self, 0x24), nullptr);
    C_0082d410_SetVtable(SubobjectAtOffset(self, 0x20), nullptr);
    C_0082d410_SetVtable(SubobjectAtOffset(self, 0x1C), nullptr);
    C_0082d410_SetVtable(SubobjectAtOffset(self, 0x18), nullptr);
    C_0082d410_SetVtable(SubobjectAtOffset(self, 0x14), nullptr);
    C_0082d410_SetVtable(SubobjectAtOffset(self, 0x10), nullptr);
    C_0082d410_SetVtable(SubobjectAtOffset(self, 0x0C), nullptr);
    C_0082d410_SetVtable(SubobjectAtOffset(self, 0x08), nullptr);
    C_0082d410_SetVtable(SubobjectAtOffset(self, 0x04), nullptr);
    C_0082d410_SetVtable(SubobjectAtOffset(self, 0x00), nullptr);
    return 0;
}

HOOK(0x4AE760, TFncAtacarDesdeParapeto_DestroySubobjectVtables, "auto");
