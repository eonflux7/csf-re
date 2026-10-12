#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(float*, __fastcall, AiStatic_AddVectors, 0x00402000,
         (void* self, void* edx, float* a2, float* a3));
CSF_FUNC(bool, __fastcall, AiStatic_IsPositionReachable, 0x004B0200,
         (void* self, void* edx, int a2, int a3));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0,
         (void* self, void*));
CSF_FUNC(char, __fastcall, AiStatic_TestPositionConstraint, 0x004DC660,
         (void* self, void* edx, int a2, int a3));
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());

CSF_GLOBAL(float, g_AiStaticOffsetDistance, 0x0083E0D4);

// The packet also classifies this direct-call address as a vtable reference.
static const auto kReachabilityReference = CSF_VTBL(0x004B0200);

static char __fastcall S_004b0200_FindReachableOffset(
    S_004b0200* self, void* /*edx*/, int actor, int positionAddress)
{
    alignas(8) uint8_t resultBytes[0xC];
    alignas(8) uint8_t candidateBytes[0x14];
    float* resultVector = reinterpret_cast<float*>(resultBytes);
    float* candidate = reinterpret_cast<float*>(candidateBytes);
    float* position = reinterpret_cast<float*>(positionAddress);
    CSF_AT(char, candidateBytes, 0x13) = 0;

    int linkedValue = GetLinkedObjectValue_4DB5C0(
        reinterpret_cast<void*>(actor), nullptr);
    void* linkedObject = reinterpret_cast<void*>(
        linkedValue ? linkedValue - 4 : 0);
    uint32_t actorData = CSF_AT(uint32_t, linkedObject, 0x2C);

    void* context = reinterpret_cast<void*>(GetValue8B0554());
    void* constraintOwner = reinterpret_cast<void*>(
        CSF_AT(uint32_t, context, 0x1FD8));
    if (AiStatic_TestPositionConstraint(
            static_cast<uint8_t*>(constraintOwner) + 0x54, nullptr,
            static_cast<int>(actorData + 0x1C),
            reinterpret_cast<int>(&self->field_0004)))
    {
        return CSF_AT(char, candidateBytes, 0x13);
    }

    CSF_AT(float, candidateBytes, 0xC) = g_AiStaticOffsetDistance;
    candidate[1] = position[1];
    candidate[0] = position[0] + CSF_AT(float, candidateBytes, 0xC);
    candidate[2] = position[2];

    if (AiStatic_IsPositionReachable(
            self, nullptr, actor, reinterpret_cast<int>(candidate)))
    {
        candidate[0] = position[0] + CSF_AT(float, candidateBytes, 0xC);
        candidate[1] = position[1];
        candidate[2] = position[2];
        goto CopyCandidate;
    }

    {
        float negativeDistance = -CSF_AT(float, candidateBytes, 0xC);
        void* origin = reinterpret_cast<uint8_t*>(self) + 4;
        float originX;
        std::memcpy(&originX, &self->field_0004, sizeof(originX));
        candidate[0] = negativeDistance + originX;
        candidate[1] = self->field_0008;
        candidate[2] = CSF_AT(float, self, 0xC);

        if (AiStatic_IsPositionReachable(
                self, nullptr, actor, reinterpret_cast<int>(candidate)))
        {
            candidate[0] = negativeDistance + position[0];
            candidate[1] = position[1];
            candidate[2] = position[2];
            goto CopyCandidate;
        }

        std::memcpy(&candidate[0], &self->field_0004, sizeof(float));
        candidate[2] =
            CSF_AT(float, candidateBytes, 0xC) + CSF_AT(float, self, 0xC);
        candidate[1] = self->field_0008;

        if (AiStatic_IsPositionReachable(
                self, nullptr, actor, reinterpret_cast<int>(candidate)))
        {
            candidate[0] = position[0];
            candidate[1] = position[1];
            candidate[2] =
                CSF_AT(float, candidateBytes, 0xC) + position[2];
            goto CopyCandidate;
        }

        std::memcpy(&candidate[0], &self->field_0004, sizeof(float));
        candidate[2] = negativeDistance + CSF_AT(float, self, 0xC);
        candidate[1] = self->field_0008;

        bool reachable = AiStatic_IsPositionReachable(
            self, nullptr, actor, reinterpret_cast<int>(candidate));
        candidate[1] = 0.0;
        if (reachable)
        {
            candidate[0] = 0.0;
            candidate[2] = negativeDistance;
            goto AddOffsetToPosition;
        }

        candidate[0] = CSF_AT(float, candidateBytes, 0xC);
        candidate[2] = CSF_AT(float, candidateBytes, 0xC);
        float* testedPosition = AiStatic_AddVectors(
            origin, nullptr, resultVector, candidate);
        reachable = AiStatic_IsPositionReachable(
            self, nullptr, actor, reinterpret_cast<int>(testedPosition));
        candidate[1] = 0.0;
        if (reachable)
        {
            candidate[0] = CSF_AT(float, candidateBytes, 0xC);
            candidate[2] = CSF_AT(float, candidateBytes, 0xC);
            goto AddOffsetToPosition;
        }

        candidate[0] = CSF_AT(float, candidateBytes, 0xC);
        candidate[2] = negativeDistance;
        testedPosition = AiStatic_AddVectors(
            origin, nullptr, resultVector, candidate);
        reachable = AiStatic_IsPositionReachable(
            self, nullptr, actor, reinterpret_cast<int>(testedPosition));
        candidate[1] = 0.0;
        if (reachable)
        {
            candidate[0] = CSF_AT(float, candidateBytes, 0xC);
            candidate[2] = negativeDistance;
            goto AddOffsetToPosition;
        }

        candidate[0] = negativeDistance;
        candidate[2] = CSF_AT(float, candidateBytes, 0xC);
        testedPosition = AiStatic_AddVectors(
            origin, nullptr, resultVector, candidate);
        reachable = AiStatic_IsPositionReachable(
            self, nullptr, actor, reinterpret_cast<int>(testedPosition));
        candidate[1] = 0.0;
        if (reachable)
        {
            candidate[0] = negativeDistance;
            candidate[2] = CSF_AT(float, candidateBytes, 0xC);
            goto AddOffsetToPosition;
        }

        candidate[0] = negativeDistance;
        candidate[2] = negativeDistance;
        testedPosition = AiStatic_AddVectors(
            origin, nullptr, resultVector, candidate);
        if (AiStatic_IsPositionReachable(
                self, nullptr, actor, reinterpret_cast<int>(testedPosition)))
        {
            candidate[1] = 0.0;
            candidate[0] = negativeDistance;
            candidate[2] = negativeDistance;
            goto AddOffsetToPosition;
        }
    }

    return CSF_AT(char, candidateBytes, 0x13);

CopyCandidate:
    position[0] = candidate[0];
    position[1] = candidate[1];
    position[2] = candidate[2];
    CSF_AT(char, candidateBytes, 0x13) = 1;
    return CSF_AT(char, candidateBytes, 0x13);

AddOffsetToPosition:
    CSF_AT(char, candidateBytes, 0x13) = 1;
    float* adjustedPosition = AiStatic_AddVectors(
        position, nullptr, resultVector, candidate);
    position[0] = adjustedPosition[0];
    position[1] = adjustedPosition[1];
    position[2] = adjustedPosition[2];
    return CSF_AT(char, candidateBytes, 0x13);
}

HOOK(0x004B0300, S_004b0200_FindReachableOffset, "auto");
