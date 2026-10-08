#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Base initializer of the menu screen (__thiscall, ret 0 -> pops nothing).
CSF_FUNC(int, __fastcall, CMenuMainPC_InitBase, 0x53A660, (void* self, void* edx));

// Calls the base initializer and then resets the menu state field to -1.
static int __fastcall CMenuMainPC_Init(CMenuMainPC* self, void* /*edx*/)
{
    int result = CMenuMainPC_InitBase(self, nullptr);
    self->field_0038 = 0xFFFFFFFFu;
    return result;
}

HOOK(0x543C90, CMenuMainPC_Init, "auto");
