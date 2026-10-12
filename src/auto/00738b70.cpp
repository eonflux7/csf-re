#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Sets the object at +0x34 (replacing the previous one), refreshes the cached
// fields from the new object's vtable, and notifies the controller at +0x04
// through slots 0x84 (attach) and 0x88 (detach) on the previous object.
static int __fastcall C_008673b0_SetTargetObject(C_008673b0* self, void* /*edx*/, int a2)
{
    int result = a2;
    uint32_t previousObject = self->field_0034;
    self->field_0034 = static_cast<uint32_t>(a2);

    if (a2 != 0) {
        void* controller = reinterpret_cast<void*>(self->field_0004);
        // Slot 0x84 / 4 = 33: controller->OnObjectAttached(a2)
        ((void(__fastcall*)(void*, void*, uint32_t))(CSF_AT(void**, controller, 0)[33]))(
            controller, nullptr, static_cast<uint32_t>(a2));

        // Re-read the newly stored object after the call.
        void* newObject = reinterpret_cast<void*>(self->field_0034);
        uint32_t newVtable = CSF_AT(uint32_t, newObject, 0);
        self->field_002c = CSF_AT(uint32_t, reinterpret_cast<void*>(newVtable), 12);
        uint32_t cachedValue = CSF_AT(uint32_t, reinterpret_cast<void*>(newVtable), 16);

        self->field_0024 = 1.0f;
        self->field_0028 = 1.0f;
        self->field_0030 = cachedValue;
        self->field_001c = 0.0f;
        self->field_0020 = 0.0f;

        result = static_cast<int>(reinterpret_cast<uintptr_t>(
            self->vSlot30(reinterpret_cast<uint32_t*>(&self->field_002c))));
    }

    if (previousObject != 0) {
        void* controller = reinterpret_cast<void*>(self->field_0004);
        // Slot 0x88 / 4 = 34: controller->OnObjectDetached(previousObject)
        return ((int(__fastcall*)(void*, void*, uint32_t))(CSF_AT(void**, controller, 0)[34]))(
            controller, nullptr, previousObject);
    }
    return result;
}

HOOK(0x738B70, C_008673b0_SetTargetObject, "auto");
