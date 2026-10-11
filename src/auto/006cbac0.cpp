#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Callees
CSF_FUNC(void*, __fastcall, GetReferencedObject, 0x006683F0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SceneLoader_GetWorldObjectCollection, 0x006B6AF0, (void* self, void* edx));

// Returns the scene object loaded for this loader: the world object collection in
// world mode (field_006b set), or the referenced object otherwise. Falls back to the
// entry selected by field_00b0 when the primary object is missing or not ready.
static int __fastcall C_00860bb8_GetLoadedScene(C_00860bb8* self, void* /*edx*/) {
    int result = 0;

    if (self->field_006b != 0) {
        void* worldObj = reinterpret_cast<void*>(self->field_000c);
        if (worldObj != nullptr && CSF_AT(uint8_t, worldObj, 0x28) != 0) {
            result = SceneLoader_GetWorldObjectCollection(worldObj, nullptr);
            if (result != 0) {
                return result;
            }
        }

        uint32_t index = self->field_00b0;
        if (index != static_cast<uint32_t>(-1)) {
            void* entry = reinterpret_cast<void*>(reinterpret_cast<uint32_t*>(self->field_00a0)[index]);
            void* target = CSF_AT(void*, entry, 0x3C);
            return SceneLoader_GetWorldObjectCollection(target, nullptr);
        }
    } else {
        void* refObj = reinterpret_cast<void*>(self->field_0008);
        if (refObj != nullptr && CSF_AT(uint8_t, refObj, 0x91) != 0) {
            result = static_cast<int>(reinterpret_cast<intptr_t>(GetReferencedObject(refObj, nullptr)));
            if (result != 0) {
                return result;
            }
        }

        uint32_t index = self->field_00b0;
        if (index != static_cast<uint32_t>(-1)) {
            void* entry = reinterpret_cast<void*>(reinterpret_cast<uint32_t*>(self->field_00a0)[index]);
            void* target = CSF_AT(void*, entry, 0x38);
            return static_cast<int>(reinterpret_cast<intptr_t>(GetReferencedObject(target, nullptr)));
        }
    }

    return result;
}

HOOK(0x006CBAC0, C_00860bb8_GetLoadedScene, "auto");
