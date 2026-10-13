#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, C_0041c550_IsField8CNull, 0x0041C550, (void* self, void*));
CSF_FUNC(int, __fastcall, Wheels_ProcessEntryWhenState1, 0x0041C650, (void* self, void*));
CSF_FUNC(bool, __fastcall, Vehicle_AimAtPosition, 0x0041C960, (void* self, void* , float* position, char interpolate));
CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __fastcall, AiAttack_IsStoredValueNegative, 0x0047AEB0, (float* self, void*));
CSF_FUNC(float*, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GetControlEntryByIndex, 0x0049ED10, (void* self, void* edx, int a2));
CSF_FUNC(std::uint32_t*, __fastcall, C_0083dd58_FindCompatibleCandidate, 0x004D0750, (C_0083dd58* self, void* , int selectionContext));
CSF_FUNC(bool, __fastcall, C_0083dd58_CanAimAtCandidate, 0x004D5630, (void* self, void* edx, void* a2));
CSF_FUNC(uint32_t*, __fastcall, C_0083dd58_UpdateTargetData, 0x004D5780, (void* self, void* edx, int a3));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static char __fastcall C_0083dd58_UpdateCandidateAim(
    C_0083dd58* self, void* /*edx*/, int /*unused*/)
{
    void* vehicle = CSF_AT(
        void*, reinterpret_cast<void*>(self->field_0014), 0x4C);

    if (static_cast<std::uint8_t>(
            C_0041c550_IsField8CNull(vehicle, nullptr)))
    {
        void* candidate = nullptr;

        if (CSF_AT(std::uint8_t, self, 0xE4))
        {
            void* currentVehicle = CSF_AT(
                void*, reinterpret_cast<void*>(self->field_0014), 0x4C);

            if (reinterpret_cast<std::uint8_t (__fastcall*)(
                    void*, void*)>(
                        CSF_AT(void**, self, 0)[0xD8 / 4])(self, nullptr))
            {
                if ((CSF_AT(std::uint8_t, currentVehicle, 0x7C) & 4) == 0)
                {
                    candidate = C_0083dd58_FindCompatibleCandidate(
                        self, nullptr, 13);
                }
            }
        }

        if (CSF_AT(std::uint8_t, self, 0xE5))
        {
            Wheels_ProcessEntryWhenState1(vehicle, nullptr);
            CSF_AT(std::uint8_t, self, 0xE5) = 0;
        }

        if (candidate)
        {
            if (C_0083dd58_CanAimAtCandidate(self, nullptr, candidate)
                && !CSF_AT(std::uint8_t, self, 0xE5))
            {
                float* position = GetObjectPositionVector(candidate, nullptr);
                Vehicle_AimAtPosition(vehicle, nullptr, position, 0);
                CSF_AT(std::uint8_t, self, 0xE5) = 1;
            }

            if (!static_cast<std::uint8_t>(
                    AiAttack_IsStoredValueNegative(
                        reinterpret_cast<float*>(
                            reinterpret_cast<std::uint8_t*>(self) + 0x60),
                        nullptr)))
            {
                C_0083dd58_UpdateTargetData(
                    self, nullptr,
                    reinterpret_cast<int>(
                        reinterpret_cast<std::uint8_t*>(candidate) + 0x1C));
                CSF_AT(std::uint8_t, self, 0xE6) = 0;
            }
        }
        else if (!CSF_AT(std::uint8_t, self, 0xE6)
                 && reinterpret_cast<std::uint8_t (__fastcall*)(
                     void*, void*)>(
                         CSF_AT(void**, self, 0)[0xD8 / 4])(self, nullptr))
        {
            void* scene = g_pCurrentScene;
            int handle = GetControlEntryByIndex(
                reinterpret_cast<void*>(self->field_0014), nullptr, 0);
            void* entry = reinterpret_cast<void*>(
                CHandleTable_Resolve(scene, nullptr, handle));

            reinterpret_cast<void (__fastcall*)(
                void*, void*, int, int, int, int)>(
                    CSF_AT(void**, entry, 0)[0x8C / 4])(
                        entry, nullptr, 0, 0, -1, 0);

            CSF_AT(std::uint8_t, self, 0xE6) = 1;
        }
    }

    std::uint8_t result = CSF_AT(std::uint8_t, self, 0xE4);
    if (result)
    {
        result = reinterpret_cast<std::uint8_t (__fastcall*)(
            void*, void*)>(
                CSF_AT(void**, self, 0)[0xD8 / 4])(self, nullptr);
    }

    if (!result)
        self->field_0074 = 1;

    return static_cast<char>(result);
}

HOOK(0x004D5C60, C_0083dd58_UpdateCandidateAim, "auto");
