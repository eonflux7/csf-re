#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static CSF_FUNC(int, __fastcall, IndexedChildForwardValue, 0x004084E0, (void* self, void* edx, int a2, int a3));
static CSF_FUNC(double, __stdcall, Vehicle_CalculateContactResponse, 0x00418830, (float* a1, float* arg4, float* a3, float a4));
static CSF_FUNC(int, __fastcall, Vehicle_GrowContactRecords, 0x00419920, (void* self, void* edx, int a2));
static CSF_FUNC(int, __fastcall, C_0082dd78_DispatchReturnedObject, 0x0041E0C0, (C_0082dd78* self, void*));

static CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
static CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);
static CSF_GLOBAL(float, g_VehicleContactScale, 0x0082E264);

static void __fastcall C_0082e1e8_AccumulateContactResponse(
    C_0082e1e8* self, void* /*edx*/, int a2)
{
    auto* context = reinterpret_cast<uint8_t*>(static_cast<uint32_t>(a2));

    int firstIndex = CSF_AT(int, context, 0x08);
    uint32_t firstEntry = firstIndex == -1
        ? 0
        : self->field_0034 + 32 * static_cast<uint32_t>(firstIndex);

    int secondIndex = CSF_AT(int, context, 0x0C);
    uint32_t secondEntry = secondIndex == -1
        ? 0
        : self->field_0044 + 32 * static_cast<uint32_t>(secondIndex);

    uint32_t entryCount = self->field_003c;
    if (!firstEntry) {
        int fallbackIndex = CSF_AT(int, context, 0x20);
        firstEntry = fallbackIndex == -1
            ? 32 * entryCount + self->field_0034 - 32
            : self->field_0034 + 32 * static_cast<uint32_t>(fallbackIndex);
    }
    if (!secondEntry) {
        int fallbackIndex = CSF_AT(int, context, 0x24);
        secondEntry = fallbackIndex == -1
            ? 32 * entryCount + self->field_0044 - 32
            : self->field_0044 + 32 * static_cast<uint32_t>(fallbackIndex);
    }

    float firstPosition[3];
    float secondPosition[3];
    IndexedChildForwardValue(
        reinterpret_cast<void*>(self->field_0030), nullptr,
        static_cast<int>(reinterpret_cast<uintptr_t>(firstPosition)),
        CSF_AT(int, reinterpret_cast<void*>(firstEntry), 0));
    IndexedChildForwardValue(
        reinterpret_cast<void*>(self->field_0030), nullptr,
        static_cast<int>(reinterpret_cast<uintptr_t>(secondPosition)),
        CSF_AT(int, reinterpret_cast<void*>(secondEntry), 0));

    void* returnedObject = reinterpret_cast<void*>(
        C_0082dd78_DispatchReturnedObject(
            reinterpret_cast<C_0082dd78*>(self), nullptr));
    void* responseParameters =
        ((void*(__fastcall*)(void*, void*))
            CSF_AT(void**, returnedObject, 0)[0x30 / 4])(
                returnedObject, nullptr);

    double response = Vehicle_CalculateContactResponse(
        firstPosition, secondPosition,
        reinterpret_cast<float*>(context + 0x38),
        CSF_AT(float, responseParameters, 0x08));

    double scale;
    if (!(response < g_fZero)) {
        void* owner = reinterpret_cast<void*>(self->field_0004);
        void* parameters = CSF_AT(void*, owner, 0x2C);
        scale = (g_TransitionOne - CSF_AT(float, context, 0x2C))
            * (CSF_AT(float, parameters, 0x1DC) * g_VehicleContactScale);
    } else {
        scale = (self->field_0080 - CSF_AT(float, context, 0x28))
            / self->field_0080;
    }

    self->field_0068 = static_cast<float>(
        scale * response * CSF_AT(float, context, 0) + self->field_0068);

    if (!(self->field_0068 < g_fZero)) {
        self->field_0068 = 0.0f;
        return;
    }

    float record[7];
    std::memcpy(record, firstPosition, sizeof(firstPosition));
    std::memcpy(record + 3, secondPosition, sizeof(secondPosition));
    record[6] = -(CSF_AT(float, context, 0) * self->field_0068);

    if (CSF_AT(uint32_t, context, 0x54) >=
        CSF_AT(uint32_t, context, 0x50)) {
        Vehicle_GrowContactRecords(
            context + 0x4C, nullptr, CSF_AT(int, context, 0x58));
    }

    uint32_t recordIndex = CSF_AT(uint32_t, context, 0x54);
    uint32_t destination = CSF_AT(uint32_t, context, 0x4C)
        + 28 * recordIndex;
    CSF_AT(uint32_t, context, 0x54) = recordIndex + 1;
    std::memcpy(reinterpret_cast<void*>(destination), record, 0x1C);
}

HOOK(0x00419FC0, C_0082e1e8_AccumulateContactResponse, "auto");
