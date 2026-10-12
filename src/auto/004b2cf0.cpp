#include "hook.h"
#include "globals.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, AiCore_DestroyEmbeddedTask, 0x004AEA30, (void* self, void* edx));
CSF_FUNC(void, __fastcall, C_0082d410_SetVtable, 0x004DBD40, (C_0082d410* self, void*));

static void __fastcall TOrden_DestroyOrderTask(TOrden* self, void* /*edx*/)
{
    CSF_AT(void*, self, 0x94) = CSF_VTBL(0x0082D410);
    AiCore_DestroyEmbeddedTask(&self->field_0064, nullptr);

    auto* bytes = reinterpret_cast<unsigned char*>(self);
    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(bytes + 0x60), nullptr);
    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(bytes + 0x5C), nullptr);
    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(bytes + 0x58), nullptr);
    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(bytes + 0x34), nullptr);
    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(bytes + 0x30), nullptr);
    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(bytes + 0x2C), nullptr);
    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(bytes + 0x28), nullptr);
    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(bytes + 0x24), nullptr);
    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(bytes + 0x20), nullptr);
    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(bytes + 0x1C), nullptr);
    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(bytes + 0x18), nullptr);
    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(bytes + 0x14), nullptr);
    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(bytes + 0x10), nullptr);
    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(bytes + 0x0C), nullptr);
    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(bytes + 0x08), nullptr);
    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(bytes + 0x04), nullptr);
    return C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(self), nullptr);
}

HOOK(0x004B2CF0, TOrden_DestroyOrderTask, "auto");
