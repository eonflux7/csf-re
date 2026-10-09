#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Adds elapsed time to the weapon FSM's accumulator, forwards the delta to the
// currently active sub-object's update slot (+0x28) when one is selected, then
// returns the result of the FSM's own slot +0x118 handler.
static int __fastcall CCharWeaponFsm_AccumulateTimeAndTick(CCharWeaponFsm* self, void* /*edx*/, float elapsedDelta) {
    uint32_t activeIndex = self->field_0024;
    self->field_006c = elapsedDelta + self->field_006c;

    if (activeIndex != static_cast<uint32_t>(-1)) {
        void** subObjects = reinterpret_cast<void**>(self->field_0018);
        void* activeObject = subObjects[activeIndex];
        // Vtable slot +0x28 => index 10.
        ((void(__fastcall*)(void*, void*, float))CSF_AT(void**, activeObject, 0)[10])(
            activeObject, nullptr, elapsedDelta);
    }

    int elapsedBits;
    std::memcpy(&elapsedBits, &elapsedDelta, sizeof(elapsedBits));
    return self->vDispatchStateHandler(elapsedBits);
}

HOOK(0x4814A0, CCharWeaponFsm_AccumulateTimeAndTick, "auto");
