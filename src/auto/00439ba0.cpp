#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Original: 0x00438880, __thiscall-style callee with 4 stack args (ret 16).
CSF_FUNC(void, __fastcall, sub_438880, 0x00438880, (void* self, void* edx, int a3, float* a4, int a5, int a6));

// Vtable slot 9 (byte offset 0x24) of the object stored at this->field_00d0.
static void S_00439ba0_RefreshSlotObject(S_00439ba0* self) {
    void* slotObject = reinterpret_cast<void*>(self->field_00d0);
    void** vtbl = CSF_AT(void**, slotObject, 0);
    using RefreshFn = void(__fastcall*)(void* self, void* edx);
    reinterpret_cast<RefreshFn>(vtbl[0x24 / 4])(slotObject, nullptr);
}

static int __fastcall S_00439ba0_UpdateFourSlots(S_00439ba0* self, void* /*edx*/) {
    S_00439ba0_RefreshSlotObject(self);

    uint8_t* entry = reinterpret_cast<uint8_t*>(self) + 0x50;
    for (int remaining = 4; remaining != 0; --remaining) {
        float slotData[8];
        std::memcpy(slotData, entry, sizeof(slotData));

        uint32_t flagWord = 0;
        std::memcpy(&flagWord, &slotData[7], sizeof(flagWord));
        if (static_cast<uint8_t>(flagWord) != 0) {
            sub_438880(self, nullptr,
                       static_cast<int>(self->field_00d0),
                       slotData,
                       reinterpret_cast<int>(&slotData[5]),
                       reinterpret_cast<int>(&self->field_00d4));
        }

        entry += 0x20;
    }
    return 0;
}

HOOK(0x00439BA0, S_00439ba0_UpdateFourSlots, "auto");
