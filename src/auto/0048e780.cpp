#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static CSF_FUNC(int, __fastcall, GetAccountSelectionIndex, 0x004A2FC0, (void* self, void* edx));

static int __fastcall SceneObject_GetSelectionIndex(void* self, void* /*edx*/)
{
    void* accounts = CSF_AT(void*, self, 0x34);
    if (accounts)
        return GetAccountSelectionIndex(accounts, nullptr);
    return -1;
}

HOOK(0x0048E780, SceneObject_GetSelectionIndex, "auto");
