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

// Returns the first check of the vehicle state flag bit 2 (true when clear).
CSF_FUNC(bool, __fastcall, IsStateFlagBit2Clear, 0x0041D030, (void* self, void*));
// Clears/processes the entry key 28 state; returns int (only the low byte is used).
CSF_FUNC(int, __fastcall, Vehicle_ProcessEntryKey28WhenState1, 0x0041D010, (void* self, void*));
// Steers the vehicle wheels toward a point; callee pops its 4-byte argument.
CSF_FUNC(std::uint8_t, __fastcall, Vehicle_SetWheelSteeringTowardPoint, 0x0041D040, (void* self, void*, float* target));
// Returns the position vector of an object (forwarder).
CSF_FUNC(std::uint32_t, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
// Searches for a nearby object of the given kind (argument 30).
CSF_FUNC(uint32_t*, __fastcall, sub_4D0750, 0x004D0750, (void* self, void* edx, int a2));
// Returns true when the object is a valid steering target for this task.
CSF_FUNC(bool, __fastcall, sub_4DA830, 0x004DA830, (void* self, void* edx, void* a2));
// Final attack/approach step on the target sub-object.
CSF_FUNC(uint32_t*, __fastcall, sub_4DAA40, 0x004DAA40, (void* self, void* edx, int a2));
// Returns the stored value of the attack sub-object; nonzero when negative.
CSF_FUNC(int, __fastcall, AiAttack_IsStoredValueNegative, 0x0047AEB0, (float* self, void*));

static std::uint8_t __fastcall C_0083dd58_TrackNearestTargetAndSteer(C_0083dd58* self, void* /*edx*/, int /*a2*/)
{
    C_0082dd78* v3 = reinterpret_cast<C_0082dd78*>(
        CSF_AT(std::uint32_t, reinterpret_cast<std::uint8_t*>(self->field_0014), 0x4c));
    std::uint8_t result = IsStateFlagBit2Clear(v3, nullptr);
    if (result)
    {
        C_008342e8* v5 = nullptr;
        if ((CSF_AT(std::uint8_t, v3, 0x7c) & 4) == 0)
            v5 = reinterpret_cast<C_008342e8*>(sub_4D0750(self, nullptr, 30));

        result = self->field_0090;
        if (result)
        {
            result = static_cast<std::uint8_t>(Vehicle_ProcessEntryKey28WhenState1(v3, nullptr));
            self->field_0090 = 0;
        }

        if (v5)
        {
            if (sub_4DA830(self, nullptr, v5) && !self->field_0090)
            {
                std::uint32_t objectPosition = GetObjectPositionVector(v5, nullptr);
                Vehicle_SetWheelSteeringTowardPoint(
                    v3, nullptr,
                    reinterpret_cast<float*>(static_cast<std::uintptr_t>(objectPosition)));
                self->field_0090 = 1;
            }

            result = static_cast<std::uint8_t>(AiAttack_IsStoredValueNegative(
                reinterpret_cast<float*>(reinterpret_cast<std::uint8_t*>(self) + 0x60), nullptr));
            if (!result)
            {
                std::uintptr_t targetFieldAddress = reinterpret_cast<std::uintptr_t>(v5) + 0x1c;
                uint32_t* approachResult = sub_4DAA40(self, nullptr, static_cast<int>(targetFieldAddress));
                return static_cast<std::uint8_t>(reinterpret_cast<std::uintptr_t>(approachResult));
            }
        }
    }
    return result;
}

HOOK(0x004DAB80, C_0083dd58_TrackNearestTargetAndSteer, "auto");
