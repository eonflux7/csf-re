#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, GetReferencedObject, 0x006683F0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SceneLoader_GetWorldObjectCollection, 0x006B6AF0, (void* self, void* edx));

// Returns true when `target` is the referenced object or world object collection
// held by this scene record, either directly or by any of its entries in the list at +0xA0.
static bool __fastcall GfxScene_MatchesReferencedObject(void* self, void* /*edx*/, int target)
{
    void* refObject = CSF_AT(void*, self, 0x08);
    if (refObject) {
        if (target == reinterpret_cast<int>(GetReferencedObject(refObject, nullptr))) {
            return true;
        }
    }

    void* worldCollection = CSF_AT(void*, self, 0x0C);
    if (worldCollection) {
        if (target == SceneLoader_GetWorldObjectCollection(worldCollection, nullptr)) {
            return true;
        }
    }

    uint32_t index = 0;
    if (CSF_AT(uint32_t, self, 0xA8) == 0) {
        return false;
    }

    while (true) {
        void* item = CSF_AT(void**, self, 0xA0)[index];

        void* itemRefObject = CSF_AT(void*, item, 0x38);
        if (itemRefObject) {
            if (target == reinterpret_cast<int>(GetReferencedObject(itemRefObject, nullptr))) {
                return true;
            }
        }

        void* itemWorldCollection = CSF_AT(void*, item, 0x3C);
        if (itemWorldCollection) {
            if (target == SceneLoader_GetWorldObjectCollection(itemWorldCollection, nullptr)) {
                return true;
            }
        }

        ++index;
        if (index >= CSF_AT(uint32_t, self, 0xA8)) {
            return false;
        }
    }
}

HOOK(0x006CBE60, GfxScene_MatchesReferencedObject, "auto");
