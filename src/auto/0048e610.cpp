#include "hook.h"
#include "globals.h"
#include "types/auto_classes.h"

static CSF_FUNC(int, __fastcall, SceneLoader_ConfigureFocusEntry, 0x0048E460, (void* self, void* edx, int* a2));

static int __fastcall C_00833908_ConfigureFocusEntry(
    C_00833908* self, void* /*edx*/, int* entry)
{
    return SceneLoader_ConfigureFocusEntry(self, nullptr, entry);
}

HOOK(0x0048E610, C_00833908_ConfigureFocusEntry, "auto");
