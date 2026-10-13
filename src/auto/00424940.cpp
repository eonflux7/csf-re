#include <cstdint>
#include <cstring>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, C_0082faf8_ApplyMovementVector, 0x004244E0, (void* self, void* edx, void* arg0));
CSF_FUNC(int, __fastcall, RecordTable_GetEntryAddress, 0x00425B40, (void* self, void* , int index));
CSF_FUNC(int, __fastcall, IsZeroFlagAndCounterAtFinalStep, 0x00425B70, (void* self, void*));
CSF_FUNC(void, __fastcall, S_00426550_PopEntryAndAdvance, 0x00426D20, (S_00426550* self, void*));
CSF_FUNC(int, __cdecl, GetGlobalObjectField184, 0x00475470, ());
CSF_FUNC(uint32_t, __fastcall, DispatchRecordToGlobalObject, 0x004758B0, (void* self, void* edx, void* record, char flag));
CSF_FUNC(float*, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(uint32_t, __fastcall, S_004dca10_DispatchWithArgBlock, 0x004DD060, (S_004dca10* self, void* , uint32_t* args));
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());

CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);

static float PathMovement_FloatFromBits(uint32_t bits)
{
    float value;
    std::memcpy(&value, &bits, sizeof(value));
    return value;
}

static uint32_t PathMovement_FloatToBits(float value)
{
    uint32_t bits;
    std::memcpy(&bits, &value, sizeof(bits));
    return bits;
}

static uint32_t* __fastcall C_0082faf8_AdvancePathMovement(
    C_0082faf8* self, void* /*edx*/, float elapsedTime)
{
    alignas(8) uint8_t completionRecord[0x34] = {};
    alignas(8) uint8_t completionArgs[0x1C] = {};
    alignas(8) uint8_t displacement[0x10] = {};
    alignas(8) uint8_t movementVector[0x18] = {};

    const float distance = elapsedTime * self->field_0050;
    void* movementController = CSF_AT(void*,
        reinterpret_cast<void*>(self->field_0014), 0x4C);
    void* segment = reinterpret_cast<void*>(self->field_0028);

    const float progress =
        distance + PathMovement_FloatFromBits(self->field_0060);
    self->field_0060 = PathMovement_FloatToBits(progress);
    if (progress >= CSF_AT(float, segment, 0x04))
    {
        self->field_0060 =
            PathMovement_FloatToBits(progress - CSF_AT(float, segment, 0x04));
        S_00426550_PopEntryAndAdvance(
            reinterpret_cast<S_00426550*>(
                reinterpret_cast<uint8_t*>(self) + 0x24),
            nullptr);
    }

    const float currentZ = GetObjectPositionVector(
        reinterpret_cast<void*>(self->field_0014), nullptr)[2];
    const float currentX = GetObjectPositionVector(
        reinterpret_cast<void*>(self->field_0014), nullptr)[0];

    void* path = reinterpret_cast<uint8_t*>(self) + 0x24;
    if (static_cast<uint8_t>(
            IsZeroFlagAndCounterAtFinalStep(path, nullptr)) != 0)
    {
        void* endpoint = reinterpret_cast<void*>(
            RecordTable_GetEntryAddress(
                path, nullptr, static_cast<int>(self->field_0040 - 1)));

        CSF_AT(float, displacement, 0x00) =
            CSF_AT(float, endpoint, 0x00) - currentX;
        CSF_AT(float, displacement, 0x04) =
            CSF_AT(float, endpoint, 0x08) - currentZ;

        // The remaining stack arguments are unspecified in the original.
        reinterpret_cast<void(__fastcall*)(void*, void*, void*, int, int, int)>(
            CSF_AT(void**, movementController, 0)[0x50 / 4])(
                movementController, nullptr, displacement, 0, 0, 0);

        void* owner = reinterpret_cast<void*>(self->field_0014);
        self->field_0020 = 0;
        self->field_0050 = 0.0f;
        self->field_0058 = 0.0f;
        self->field_0074 = 0;

        CSF_AT(uint32_t, completionArgs, 0x00) = 1;
        CSF_AT(void**, completionArgs, 0x04) = CSF_VTBL(0x0082D448);
        CSF_AT(uint32_t, completionArgs, 0x08) = CSF_AT(uint32_t, owner, 0x20);
        CSF_AT(uint32_t, completionArgs, 0x0C) = CSF_AT(uint32_t, owner, 0x24);
        CSF_AT(uint32_t, completionArgs, 0x10) = CSF_AT(uint32_t, endpoint, 0x00);
        CSF_AT(uint32_t, completionArgs, 0x14) = CSF_AT(uint32_t, endpoint, 0x04);
        CSF_AT(uint32_t, completionArgs, 0x18) = CSF_AT(uint32_t, endpoint, 0x08);

        void* globalObject = reinterpret_cast<void*>(GetValue8B0554());
        S_004dca10_DispatchWithArgBlock(
            reinterpret_cast<S_004dca10*>(
                CSF_AT(uint8_t*, globalObject, 0x1FD8) + 0x34),
            nullptr, reinterpret_cast<uint32_t*>(completionArgs));

        void* ownerBase =
            reinterpret_cast<uint8_t*>(self->field_0014) + 0x04;
        reinterpret_cast<void(__fastcall*)(void*, void*)>(
            CSF_AT(void**, ownerBase, 0)[0x10 / 4])(ownerBase, nullptr);

        self->field_0018 |= 4u;

        CSF_AT(int, completionRecord, 0x0C) = -1;
        CSF_AT(int, completionRecord, 0x10) = -1;
        CSF_AT(int, completionRecord, 0x18) = -1;
        CSF_AT(int, completionRecord, 0x1C) = -1;

        void* ownerRecord =
            reinterpret_cast<uint8_t*>(self->field_0014) + 0x1C;
        CSF_AT(void**, completionRecord, 0x00) = CSF_VTBL(0x0082D478);
        CSF_AT(void**, completionRecord, 0x08) = CSF_VTBL(0x0082D448);
        CSF_AT(void**, completionRecord, 0x14) = CSF_VTBL(0x0082D448);
        CSF_AT(uint32_t, completionRecord, 0x04) = 27;
        CSF_AT(uint32_t, completionRecord, 0x20) = 0;
        CSF_AT(uint32_t, completionRecord, 0x24) = 0;
        CSF_AT(uint32_t, completionRecord, 0x2C) = 0;
        CSF_AT(uint32_t, completionRecord, 0x28) = 0;
        CSF_AT(uint8_t, completionRecord, 0x30) = 0;
        CSF_AT(uint32_t, completionRecord, 0x0C) =
            CSF_AT(uint32_t, ownerRecord, 0x04);
        CSF_AT(uint32_t, completionRecord, 0x10) =
            CSF_AT(uint32_t, ownerRecord, 0x08);

        void* dispatcher = reinterpret_cast<void*>(GetGlobalObjectField184());
        DispatchRecordToGlobalObject(
            dispatcher, nullptr, completionRecord, 0);
    }
    else
    {
        segment = reinterpret_cast<void*>(self->field_0028);
        const uint32_t flags = CSF_AT(uint32_t, segment, 0x00);
        if ((flags & 1) != 0)
        {
            CSF_AT(float, displacement, 0x00) =
                CSF_AT(float, segment, 0x10) *
                    PathMovement_FloatFromBits(self->field_0060) +
                CSF_AT(float, segment, 0x08) - currentX;
            CSF_AT(float, displacement, 0x04) =
                CSF_AT(float, segment, 0x14) *
                    PathMovement_FloatFromBits(self->field_0060) +
                CSF_AT(float, segment, 0x0C) - currentZ;
        }
        else if ((flags & 6) != 0)
        {
            const float inverseRadius =
                g_TransitionOne / CSF_AT(float, segment, 0x18);
            const float angularProgress =
                inverseRadius * PathMovement_FloatFromBits(self->field_0060);
            float angle;
            float angularDisplacement;
            if (flags == 2)
            {
                angle = angularProgress + CSF_AT(float, segment, 0x1C);
                angularDisplacement = -(inverseRadius * distance);
            }
            else
            {
                angle = CSF_AT(float, segment, 0x1C) - angularProgress;
                angularDisplacement = inverseRadius * distance;
            }

            CSF_AT(float, displacement, 0x00) =
                std::cos(angle) * CSF_AT(float, segment, 0x18) +
                CSF_AT(float, segment, 0x08) - currentX;
            CSF_AT(float, displacement, 0x04) =
                std::sin(angle) * CSF_AT(float, segment, 0x18) +
                CSF_AT(float, segment, 0x0C) - currentZ;
            CSF_AT(float, displacement, 0x08) = angularDisplacement;
            CSF_AT(uint32_t, displacement, 0x0C) =
                CSF_AT(uint32_t, segment, 0x18);
        }

        reinterpret_cast<void(__fastcall*)(void*, void*, void*, int, int, int)>(
            CSF_AT(void**, movementController, 0)[0x50 / 4])(
                movementController, nullptr, displacement, 0, 0, 0);
    }

    CSF_AT(uint32_t, movementVector, 0x00) =
        CSF_AT(uint32_t, displacement, 0x00);
    CSF_AT(float, movementVector, 0x04) = 0.0f;
    CSF_AT(uint32_t, movementVector, 0x08) =
        CSF_AT(uint32_t, displacement, 0x04);
    return C_0082faf8_ApplyMovementVector(self, nullptr, movementVector);
}

HOOK(0x00424940, C_0082faf8_AdvancePathMovement, "auto");
