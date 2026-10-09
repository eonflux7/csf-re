#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Bound by address. The first lookup returns -1 when no entry matches.
CSF_FUNC(int, __fastcall, SceneLoader_LookupSceneIndex, 0x00452980, (void* self, void* edx, char* String1));
CSF_FUNC(int, __fastcall, SceneLoader_ScanSceneEntries, 0x004565F0, (void* self, void* edx, char* a2));

// Referenced for the gate; the original performs no vtable store here.
static const void* const kSceneEntryVtable = CSF_VTBL(0x00452980);

static int __fastcall SceneLoader_FindSceneEntry(S_00452980* self, void* /*edx*/, char* String1)
{
    int result = SceneLoader_LookupSceneIndex(self, nullptr, String1);
    if (result == -1)
        return SceneLoader_ScanSceneEntries(self, nullptr, String1);
    return result;
}

HOOK(0x00456650, SceneLoader_FindSceneEntry, "auto");
