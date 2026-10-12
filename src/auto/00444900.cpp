#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, Scene_LookupEntryByKey, 0x00468750, (S_00468750* self, void* /*edx*/, int key));

// Returns true when the scene has no entry for the key, or when the entry's
// field at +0x18 is zero. The scene pointer is stored at +4 of this object.
static bool __fastcall S_00468750_IsEntryMissingOrEmpty(S_00468750* self, void* /*edx*/, int key)
{
    S_00468750* scene = CSF_AT(S_00468750*, self, 4);
    int entry = Scene_LookupEntryByKey(scene, nullptr, key);
    if (entry == 0)
        return true;
    int entryFlags = CSF_AT(int, reinterpret_cast<void*>(entry), 0x18);
    return entryFlags == 0;
}

HOOK(0x00444900, S_00468750_IsEntryMissingOrEmpty, "auto");
