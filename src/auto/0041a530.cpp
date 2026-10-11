#include <cstdint>
#include <cstring>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __cdecl, ClampFloatToBounds, 0x00406B60, (float* value, float* minimum, float* maximum));
CSF_FUNC(int, __fastcall, C_0082e1e8_UpdateRotationInputs, 0x00419160, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, Vehicle_GrowRotationCommands, 0x00419920, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));

CSF_GLOBAL(float, g_HalfDurationScale, 0x0082D444);
CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);
CSF_GLOBAL(float, g_ProgressScale, 0x0082E03C);
CSF_GLOBAL(float, g_VehicleRotationInputScale, 0x0082E258);
CSF_GLOBAL(float, g_VehicleRotationDeadZone, 0x0082E268);

static float Vehicle_FloatFromBits(std::uint32_t bits)
{
    float value;
    std::memcpy(&value, &bits, sizeof(value));
    return value;
}

static void __fastcall C_0082e1e8_UpdateRotationCommands(
    C_0082e1e8* self, void* /*edx*/, int a2)
{
    auto* update = reinterpret_cast<std::uint8_t*>(a2);
    CSF_AT(float, update, 0x44) = 0.0f;
    CSF_AT(float, update, 0x48) = 0.0f;
    C_0082e1e8_UpdateRotationInputs(self, nullptr, a2);

    void* vehicle = reinterpret_cast<void*>(self->field_0004);
    void* parameters = CSF_AT(void*, vehicle, 0x2C);
    const float doubledResponse =
        CSF_AT(float, parameters, 0x1E0) + CSF_AT(float, parameters, 0x1E0);
    const float halfResponse =
        CSF_AT(float, parameters, 0x1E4) * g_HalfDurationScale;
    void* motion = CSF_AT(void*, vehicle, 0x30);
    const float input =
        CSF_AT(float, motion, 0x58) * g_VehicleRotationInputScale;

    if (input > g_fZero)
        CSF_AT(float, update, 0x44) -=
            (g_TransitionOne - CSF_AT(float, update, 0x28)) *
            input * doubledResponse;
    if (input < g_fZero)
        CSF_AT(float, update, 0x44) -=
            (g_TransitionOne - CSF_AT(float, update, 0x2C)) *
            input * doubledResponse;

    if (Vehicle_FloatFromBits(self->field_0028) > g_fZero)
    {
        const float speed = CSF_AT(float, motion, 0x50);
        CSF_AT(float, update, 0x48) =
            speed / Vehicle_FloatFromBits(self->field_002c) *
            speed * halfResponse + CSF_AT(float, update, 0x48);
    }
    if (Vehicle_FloatFromBits(self->field_0028) < g_fZero)
    {
        const float speed = CSF_AT(float, motion, 0x50);
        CSF_AT(float, update, 0x48) -=
            speed / Vehicle_FloatFromBits(self->field_002c) *
            speed * halfResponse;
    }

    float maximum = 0.30000001f;
    float minimum = -0.30000001f;
    ClampFloatToBounds(&CSF_AT(float, update, 0x44), &minimum, &maximum);
    minimum = 0.30000001f;
    maximum = -0.30000001f;
    ClampFloatToBounds(&CSF_AT(float, update, 0x48), &maximum, &minimum);

    self->field_0074 =
        CSF_AT(float, update, 0) * CSF_AT(float, update, 0x44) +
        self->field_0074;
    self->field_0078 =
        CSF_AT(float, update, 0x48) * CSF_AT(float, update, 0) +
        self->field_0078;

    const float firstDamping = g_TransitionOne -
        (self->field_0074 < g_fZero
            ? CSF_AT(float, update, 0x28)
            : CSF_AT(float, update, 0x2C));
    const float secondDamping = g_TransitionOne -
        (self->field_0078 > g_fZero
            ? CSF_AT(float, update, 0x34)
            : CSF_AT(float, update, 0x30));

    vehicle = reinterpret_cast<void*>(self->field_0004);
    parameters = CSF_AT(void*, vehicle, 0x2C);
    const float secondInverseResponse =
        g_TransitionOne / CSF_AT(float, parameters, 0x1E4);
    self->field_0074 -=
        g_TransitionOne / CSF_AT(float, parameters, 0x1E0) *
        self->field_0074 * CSF_AT(float, update, 0) * firstDamping;
    self->field_0078 -=
        secondInverseResponse * self->field_0078 *
        CSF_AT(float, update, 0) * secondDamping;

    if (CSF_AT(float, update, 0x2C) < g_ProgressScale &&
        self->field_0074 < g_fZero)
        self->field_0074 = 0.0f;
    if (CSF_AT(float, update, 0x28) < g_ProgressScale &&
        self->field_0074 > g_fZero)
        self->field_0074 = 0.0f;
    if (CSF_AT(float, update, 0x30) < g_ProgressScale &&
        self->field_0078 < g_fZero)
        self->field_0078 = 0.0f;
    if (CSF_AT(float, update, 0x34) < g_ProgressScale &&
        self->field_0078 > g_fZero)
        self->field_0078 = 0.0f;

    if (std::fabs(CSF_AT(float, update, 0x44)) < g_VehicleRotationDeadZone &&
        std::fabs(self->field_0074) < g_ProgressScale)
        self->field_0074 = 0.0f;
    if (std::fabs(CSF_AT(float, update, 0x48)) < g_VehicleRotationDeadZone &&
        std::fabs(self->field_0078) < g_ProgressScale)
        self->field_0078 = 0.0f;

    void* transformOwner =
        reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, vehicle, 0)[40 / 4])(vehicle, nullptr);

    // Each matrix output consists of a pointer followed by an ownership byte.
    struct MatrixOutput
    {
        std::uint32_t* matrix;
        std::uint8_t owned;
        std::uint8_t padding[3];
    };

    float command[7];
    if (self->field_0074 != g_fZero)
    {
        std::memcpy(&command[0], &CSF_AT(std::uint32_t, update, 0x38), 4);
        std::memcpy(&command[1], &CSF_AT(std::uint32_t, update, 0x3C), 4);
        std::memcpy(&command[2], &CSF_AT(std::uint32_t, update, 0x40), 4);

        void* transform =
            reinterpret_cast<void* (__fastcall*)(void*, void*)>(
                CSF_AT(void**, transformOwner, 0)[24 / 4])(
                    transformOwner, nullptr);
        MatrixOutput output{};
        void* matrixResult =
            reinterpret_cast<void* (__fastcall*)(void*, void*, MatrixOutput*)>(
                CSF_AT(void**, transform, 0)[140 / 4])(
                    transform, nullptr, &output);
        float* matrix = CSF_AT(float*, matrixResult, 0);

        command[3] = CSF_AT(float, update, 0x38) + matrix[0];
        command[4] = CSF_AT(float, update, 0x3C) + matrix[1];
        command[5] = CSF_AT(float, update, 0x40) + matrix[2];
        if (output.owned)
            RwMatrixDestroy(output.matrix);

        const std::uint32_t count = CSF_AT(std::uint32_t, update, 0x54);
        const std::uint32_t capacity = CSF_AT(std::uint32_t, update, 0x50);
        command[6] = self->field_0074 * CSF_AT(float, update, 0);
        if (count >= capacity)
            Vehicle_GrowRotationCommands(
                update + 0x4C, nullptr, CSF_AT(int, update, 0x58));

        auto* destination =
            CSF_AT(std::uint8_t*, update, 0x4C) +
            28 * CSF_AT(std::uint32_t, update, 0x54)++;
        std::memcpy(destination, command, 0x1C);
    }

    if (self->field_0078 != g_fZero)
    {
        std::memcpy(&command[0], &CSF_AT(std::uint32_t, update, 0x38), 4);
        std::memcpy(&command[1], &CSF_AT(std::uint32_t, update, 0x3C), 4);
        std::memcpy(&command[2], &CSF_AT(std::uint32_t, update, 0x40), 4);

        void* transform =
            reinterpret_cast<void* (__fastcall*)(void*, void*)>(
                CSF_AT(void**, transformOwner, 0)[24 / 4])(
                    transformOwner, nullptr);
        MatrixOutput output{};
        void* matrixResult =
            reinterpret_cast<void* (__fastcall*)(void*, void*, MatrixOutput*)>(
                CSF_AT(void**, transform, 0)[140 / 4])(
                    transform, nullptr, &output);
        float* matrix = CSF_AT(float*, matrixResult, 0);

        command[3] = matrix[8] + CSF_AT(float, update, 0x38);
        command[4] = matrix[9] + CSF_AT(float, update, 0x3C);
        command[5] = matrix[10] + CSF_AT(float, update, 0x40);
        if (output.owned)
            RwMatrixDestroy(output.matrix);

        const std::uint32_t count = CSF_AT(std::uint32_t, update, 0x54);
        command[6] = self->field_0078 * CSF_AT(float, update, 0);
        const std::uint32_t capacity = CSF_AT(std::uint32_t, update, 0x50);
        if (count >= capacity)
            Vehicle_GrowRotationCommands(
                update + 0x4C, nullptr, CSF_AT(int, update, 0x58));

        auto* destination =
            CSF_AT(std::uint8_t*, update, 0x4C) +
            28 * CSF_AT(std::uint32_t, update, 0x54)++;
        std::memcpy(destination, command, 0x1C);
    }
}

HOOK(0x0041A530, C_0082e1e8_UpdateRotationCommands, "auto");
