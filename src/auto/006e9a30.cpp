#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Callee bindings (bind-as lines from the packet).
CSF_FUNC(int, __fastcall, sub_6E8A50, 0x006E8A50, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, sub_6E9880, 0x006E9880, (void* self, void* edx, int a2));

// Stores a 3-float position for joint `jointIndex`. If the joint has an attached
// object with a local offset at +0x24, first sends (position - offset) to that
// object's virtual slot 10. Then copies the position into the joint's slot at +0x10.
static uint32_t* __fastcall C_00861e98_StoreJointPosition(C_00861e98* self, void* /*edx*/,
                                                          int jointIndex, float* position) {
    // Joint table pointer at +0x4C; element jointIndex holds the joint object.
    uint32_t* jointTable = reinterpret_cast<uint32_t*>(CSF_AT(uint32_t, self, 0x4C));
    uint32_t jointObjAddr = jointTable[jointIndex];

    if (jointObjAddr != 0) {
        // Local offset of the joint object (float[3] at +0x24 pointer).
        float* jointOffset = reinterpret_cast<float*>(
            CSF_AT(uint32_t, reinterpret_cast<void*>(jointObjAddr), 0x24));
        if (jointOffset != nullptr) {
            void* attached = reinterpret_cast<void*>(sub_6E8A50(self, nullptr, jointIndex));
            float delta[3];
            delta[0] = position[0] - jointOffset[4];
            delta[1] = position[1] - jointOffset[5];
            delta[2] = position[2] - jointOffset[6];
            // Virtual slot 0x28 / 4 = 10 on the attached object.
            reinterpret_cast<void(__fastcall*)(void*, void*, float*)>(
                CSF_AT(void**, attached, 0)[10])(attached, nullptr, delta);
        }
    }

    int slotBase = sub_6E9880(self, nullptr, jointIndex);
    uint32_t* result = reinterpret_cast<uint32_t*>(static_cast<uintptr_t>(slotBase) + 16);

    // Copy the three dwords of the position (re-read from the argument).
    const uint32_t* src = reinterpret_cast<const uint32_t*>(position);
    result[0] = src[0];
    result[1] = src[1];
    result[2] = src[2];
    return result;
}

HOOK(0x006E9A30, C_00861e98_StoreJointPosition, "auto");
