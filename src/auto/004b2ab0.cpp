#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, C_0082d410_SetVtable, 0x004DBD40, (C_0082d410* self, void*));

static void __fastcall TAlerta_DestroyEmbeddedObjects(TAlerta* self, void* /*edx*/)
{
    auto* bytes = reinterpret_cast<uint8_t*>(self);

    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(bytes + 0x84), nullptr);
    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(bytes + 0x80), nullptr);
    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(bytes + 0x7C), nullptr);
    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(bytes + 0x78), nullptr);
    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(bytes + 0x74), nullptr);
    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(bytes + 0x70), nullptr);

    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(bytes + 0x6C), nullptr);
    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(bytes + 0x68), nullptr);
    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(bytes + 0x64), nullptr);
    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(bytes + 0x60), nullptr);
    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(bytes + 0x5C), nullptr);

    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(bytes + 0x4C), nullptr);
    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(bytes + 0x48), nullptr);
    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(bytes + 0x44), nullptr);
    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(bytes + 0x40), nullptr);
    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(bytes + 0x3C), nullptr);
    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(bytes + 0x38), nullptr);

    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(bytes + 0x28), nullptr);
    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(bytes + 0x24), nullptr);
    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(bytes + 0x20), nullptr);
    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(bytes + 0x1C), nullptr);
    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(bytes + 0x18), nullptr);

    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(bytes + 0x14), nullptr);
    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(bytes + 0x0C), nullptr);
    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(bytes + 0x08), nullptr);
    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(bytes + 0x04), nullptr);

    return C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(self), nullptr);
}

HOOK(0x004B2AB0, TAlerta_DestroyEmbeddedObjects, "auto");
