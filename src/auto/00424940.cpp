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
CSF_FUNC(unsigned int, __fastcall, DispatchWithArgBlock, 0x004DD060, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());

CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);

static float PathMovement_DecodeFloat(uint32_t bits)
{
    float value;
    std::memcpy(&value, &bits, sizeof(value));
    return value;
}

static uint32_t PathMovement_EncodeFloat(float value)
{
    uint32_t bits;
    std::memcpy(&bits, &value, sizeof(bits));
    return bits;
}

static uint32_t* __fastcall C_0082faf8_AdvancePathMovement(
    C_0082faf8* self, void* /*edx*/, float deltaTime)
{
    const float distanceStep = deltaTime * self->field_0050;
    void* movement = CSF_AT(void*, reinterpret_cast<void*>(self->field_0014), 0x4C);
    void* entry = reinterpret_cast<void*>(self->field_0028);

    const float progress = distanceStep + PathMovement_DecodeFloat(self->field_0060);
    self->field_0060 = PathMovement_EncodeFloat(progress);
    if (progress >= CSF_AT(float, entry, 0x04))
    {
        self->field_0060 =
            PathMovement_EncodeFloat(progress - CSF_AT(float, entry, 0x04));
        S_00426550_PopEntryAndAdvance(
            reinterpret_cast<S_00426550*>(reinterpret_cast<uint8_t*>(self) + 0x24),
            nullptr);
    }

    const float positionZ =
        GetObjectPositionVector(reinterpret_cast<void*>(self->field_0014), nullptr)[2];
    const float positionX =
        GetObjectPositionVector(reinterpret_cast<void*>(self->field_0014), nullptr)[0];

    alignas(8) uint8_t stepBytes[0x10];
    CSF_AT(uint32_t, stepBytes, 0x00) = 0;
    CSF_AT(uint32_t, stepBytes, 0x04) = 0;
    CSF_AT(uint32_t, stepBytes, 0x08) = 0;
    CSF_AT(uint32_t, stepBytes, 0x0C) = 0;

    void* pathTable = reinterpret_cast<uint8_t*>(self) + 0x24;
    if (static_cast<uint8_t>(IsZeroFlagAndCounterAtFinalStep(pathTable, nullptr)))
    {
        void* destination = reinterpret_cast<void*>(
            RecordTable_GetEntryAddress(
                pathTable, nullptr, static_cast<int>(self->field_0040 - 1)));

        CSF_AT(float, stepBytes, 0x00) =
            CSF_AT(float, destination, 0x00) - positionX;
        CSF_AT(float, stepBytes, 0x04) =
            CSF_AT(float, destination, 0x08) - positionZ;

        reinterpret_cast<void(__fastcall*)(void*, void*, void*)>(
            CSF_AT(void**, movement, 0)[0x50 / 4])(movement, nullptr, stepBytes);

        void* owner = reinterpret_cast<void*>(self->field_0014);
        self->field_0020 = 0;
        self->field_0050 = 0.0f;
        self->field_0058 = 0.0f;
        self->field_0074 = 0;

        alignas(8) uint8_t arrivalBytes[0x1C];
        CSF_AT(uint32_t, arrivalBytes, 0x00) = 1;
        CSF_AT(void**, arrivalBytes, 0x04) = CSF_VTBL(0x0082D448);
        CSF_AT(uint32_t, arrivalBytes, 0x08) = CSF_AT(uint32_t, owner, 0x20);
        CSF_AT(uint32_t, arrivalBytes, 0x0C) = CSF_AT(uint32_t, owner, 0x24);
        CSF_AT(uint32_t, arrivalBytes, 0x10) = CSF_AT(uint32_t, destination, 0x00);
        CSF_AT(uint32_t, arrivalBytes, 0x14) = CSF_AT(uint32_t, destination, 0x04);
        CSF_AT(uint32_t, arrivalBytes, 0x18) = CSF_AT(uint32_t, destination, 0x08);

        void* globalObject = reinterpret_cast<void*>(GetValue8B0554());
        DispatchWithArgBlock(
            reinterpret_cast<uint8_t*>(CSF_AT(void*, globalObject, 8152)) + 52,
            nullptr, reinterpret_cast<uint32_t*>(arrivalBytes));

        void* ownerInterface =
            reinterpret_cast<uint8_t*>(self->field_0014) + 0x04;
        reinterpret_cast<void(__fastcall*)(void*, void*)>(
            CSF_AT(void**, ownerInterface, 0)[0x10 / 4])(ownerInterface, nullptr);

        self->field_0018 |= 4u;

        alignas(8) uint8_t recordBytes[0x34];
        CSF_AT(int, recordBytes, 0x0C) = -1;
        CSF_AT(int, recordBytes, 0x10) = -1;
        CSF_AT(int, recordBytes, 0x18) = -1;
        CSF_AT(int, recordBytes, 0x1C) = -1;

        owner = reinterpret_cast<void*>(self->field_0014);
        CSF_AT(void**, recordBytes, 0x00) = CSF_VTBL(0x0082D478);
        CSF_AT(void**, recordBytes, 0x08) = CSF_VTBL(0x0082D448);
        CSF_AT(void**, recordBytes, 0x14) = CSF_VTBL(0x0082D448);
        CSF_AT(uint32_t, recordBytes, 0x04) = 27;
        CSF_AT(uint32_t, recordBytes, 0x20) = 0;
        CSF_AT(uint32_t, recordBytes, 0x24) = 0;
        CSF_AT(uint32_t, recordBytes, 0x2C) = 0;
        CSF_AT(uint32_t, recordBytes, 0x28) = 0;
        CSF_AT(uint8_t, recordBytes, 0x30) = 0;
        CSF_AT(uint32_t, recordBytes, 0x0C) = CSF_AT(uint32_t, owner, 0x20);
        CSF_AT(uint32_t, recordBytes, 0x10) = CSF_AT(uint32_t, owner, 0x24);

        void* dispatcher = reinterpret_cast<void*>(GetGlobalObjectField184());
        DispatchRecordToGlobalObject(dispatcher, nullptr, recordBytes, 0);
    }
    else
    {
        entry = reinterpret_cast<void*>(self->field_0028);
        const uint32_t flags = CSF_AT(uint32_t, entry, 0x00);

        if ((flags & 1) != 0)
        {
            CSF_AT(float, stepBytes, 0x00) =
                CSF_AT(float, entry, 0x10) * PathMovement_DecodeFloat(self->field_0060)
                + CSF_AT(float, entry, 0x08) - positionX;
            CSF_AT(float, stepBytes, 0x04) =
                CSF_AT(float, entry, 0x14) * PathMovement_DecodeFloat(self->field_0060)
                + CSF_AT(float, entry, 0x0C) - positionZ;
        }
        else if ((flags & 6) != 0)
        {
            const float inverseRadius = g_TransitionOne / CSF_AT(float, entry, 0x18);
            const float angularProgress =
                inverseRadius * PathMovement_DecodeFloat(self->field_0060);
            float angle;
            float angleStep;
            if (flags == 2)
            {
                angle = angularProgress + CSF_AT(float, entry, 0x1C);
                angleStep = -(inverseRadius * distanceStep);
            }
            else
            {
                angle = CSF_AT(float, entry, 0x1C) - angularProgress;
                angleStep = inverseRadius * distanceStep;
            }

            CSF_AT(float, stepBytes, 0x00) =
                std::cos(angle) * CSF_AT(float, entry, 0x18)
                + CSF_AT(float, entry, 0x08) - positionX;
            CSF_AT(float, stepBytes, 0x04) =
                std::sin(angle) * CSF_AT(float, entry, 0x18)
                + CSF_AT(float, entry, 0x0C) - positionZ;
            CSF_AT(float, stepBytes, 0x08) = angleStep;
            CSF_AT(uint32_t, stepBytes, 0x0C) = CSF_AT(uint32_t, entry, 0x18);
        }

        reinterpret_cast<void(__fastcall*)(void*, void*, void*)>(
            CSF_AT(void**, movement, 0)[0x50 / 4])(movement, nullptr, stepBytes);
    }

    alignas(8) uint8_t vectorBytes[0x18];
    S_00526d40& displacement = *reinterpret_cast<S_00526d40*>(vectorBytes);
    displacement.field_0000 = CSF_AT(uint32_t, stepBytes, 0x00);
    CSF_AT(float, vectorBytes, 0x04) = 0.0f;
    CSF_AT(float, vectorBytes, 0x08) = CSF_AT(float, stepBytes, 0x04);
    return C_0082faf8_ApplyMovementVector(self, nullptr, &displacement);
}

HOOK(0x00424940, C_0082faf8_AdvancePathMovement, "auto");
