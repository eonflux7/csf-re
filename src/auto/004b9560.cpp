#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, C_0082d410_SetVtable, 0x004DBD40, (C_0082d410* self, void*));

static void __fastcall TCombate_DestroyCombatTask(TCombate* self, void* /*edx*/)
{
    auto* base = reinterpret_cast<uint8_t*>(self);
    auto* group = base + 0x78;

    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(group + 0x14), nullptr);
    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(group + 0x10), nullptr);
    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(group + 0xC), nullptr);
    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(group + 8), nullptr);
    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(group + 4), nullptr);
    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(group), nullptr);

    group = base + 0x54;
    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(group + 0x14), nullptr);
    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(group + 0xC), nullptr);
    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(group + 8), nullptr);
    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(group + 4), nullptr);
    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(group), nullptr);

    group = base + 0x48;
    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(group + 8), nullptr);
    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(group + 4), nullptr);
    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(group), nullptr);

    group = base + 0x30;
    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(group + 8), nullptr);
    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(group + 4), nullptr);
    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(group), nullptr);

    group = base + 0x24;
    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(group + 8), nullptr);
    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(group + 4), nullptr);
    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(group), nullptr);

    group = base + 4;
    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(group + 0x1C), nullptr);
    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(group + 0x18), nullptr);
    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(group + 0x14), nullptr);

    auto* nestedGroup = group + 4;
    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(nestedGroup + 8), nullptr);
    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(nestedGroup + 4), nullptr);
    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(nestedGroup), nullptr);
    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(group), nullptr);

    return C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(self), nullptr);
}

HOOK(0x004B9560, TCombate_DestroyCombatTask, "auto");
