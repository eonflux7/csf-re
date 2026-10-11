#include <cstdint>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(double, __fastcall, C_008342e8_ComputeResolvedObjectScalar, 0x0048F220, (C_008342e8* self, void*));
CSF_FUNC(void, __fastcall, CharacterModel_UpdateModel, 0x004F6990, (void* self, void* edx, float a2));
CSF_FUNC(int, __cdecl, GameConfig_IsStateCodeAllowed, 0x00755810, (std::uint32_t stateCode));

CSF_GLOBAL(float, g_flt_0082DC84, 0x0082DC84);
CSF_GLOBAL(float, g_RadiansToDegrees, 0x0083F180);
CSF_GLOBAL(float, g_DegreesToRadians, 0x0083F184);
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static void __fastcall CharacterModel_UpdateTrackedTransform(
    void* self, void* /*edx*/, float deltaTime)
{
    alignas(8) std::uint8_t vectorBytes[0x18];
    float* vectors = reinterpret_cast<float*>(vectorBytes);
    auto* bytes = static_cast<std::uint8_t*>(self);

    if (CSF_AT(int, self, 0xD8) >= 0)
    {
        auto* target = reinterpret_cast<C_008342e8*>(
            CHandleTable_Resolve(g_pCurrentScene, nullptr,
                reinterpret_cast<int>(bytes + 0xD0)));

        if (target)
        {
            const double heading =
                C_008342e8_ComputeResolvedObjectScalar(target, nullptr);
            vectors[1] = CSF_AT(float, self, 0xF8);
            void* model = CSF_AT(void*, self, 0x4);

            const double angle = CSF_AT(float, self, 0x108) +
                (heading * g_DegreesToRadians -
                 CSF_AT(float, self, 0x104) -
                 CSF_AT(float, self, 0x108)) *
                CSF_AT(float, self, 0x10C);
            CSF_AT(float, self, 0x108) = static_cast<float>(angle);

            const double sine = std::sin(angle);
            const double cosine = std::cos(angle);
            vectors[0] = static_cast<float>(
                cosine * CSF_AT(float, self, 0xF4) +
                sine * CSF_AT(float, self, 0xFC));
            vectors[2] = static_cast<float>(
                cosine * CSF_AT(float, self, 0xFC) -
                sine * CSF_AT(float, self, 0xF4));

            void* transform =
                ((void* (__fastcall*)(void*, void*))
                    CSF_AT(void**, model, 0)[0x10 / 4])(model, nullptr);

            const float* position = reinterpret_cast<const float*>(
                GetObjectPositionVector(target, nullptr));
            vectors[3] = vectors[0] + position[0];
            vectors[4] = vectors[1] + position[1];
            vectors[5] = vectors[2] + position[2];

            if (CSF_AT(std::uint8_t, self, 0xCC))
            {
                CSF_AT(float, self, 0xE8) =
                    (vectors[3] - CSF_AT(float, self, 0xE8)) *
                    CSF_AT(float, self, 0x10C) +
                    CSF_AT(float, self, 0xE8);
                CSF_AT(float, self, 0xEC) =
                    (vectors[4] - CSF_AT(float, self, 0xEC)) *
                    CSF_AT(float, self, 0x10C) +
                    CSF_AT(float, self, 0xEC);
                CSF_AT(float, self, 0xF0) =
                    (vectors[5] - CSF_AT(float, self, 0xF0)) *
                    CSF_AT(float, self, 0x10C) +
                    CSF_AT(float, self, 0xF0);
            }
            else
            {
                CSF_AT(float, self, 0xE8) = vectors[3];
                CSF_AT(float, self, 0xEC) = vectors[4];
                CSF_AT(float, self, 0xF0) = vectors[5];
            }

            ((void (__fastcall*)(void*, void*, void*))
                CSF_AT(void**, transform, 0)[0x14 / 4])(
                    transform, nullptr, bytes + 0xE8);

            const float rotation =
                (CSF_AT(float, self, 0x100) +
                 CSF_AT(float, self, 0x108)) * g_RadiansToDegrees;
            ((void (__fastcall*)(void*, void*, float))
                CSF_AT(void**, transform, 0)[0x34 / 4])(
                    transform, nullptr, rotation);

            const float dx = CSF_AT(float, self, 0xE8) - vectors[3];
            const float dy = CSF_AT(float, self, 0xEC) - vectors[4];
            const float dz = CSF_AT(float, self, 0xF0) - vectors[5];
            CSF_AT(std::uint8_t, self, 0xCD) =
                std::sqrt(dz * dz + dx * dx + dy * dy) < g_flt_0082DC84;
        }
    }

    if (CSF_AT(int, self, 0xE4) >= 0)
    {
        auto* target = reinterpret_cast<C_008342e8*>(
            CHandleTable_Resolve(g_pCurrentScene, nullptr,
                reinterpret_cast<int>(bytes + 0xDC)));

        if (target)
        {
            const float* position;
            if (static_cast<std::uint8_t>(
                    GameConfig_IsStateCodeAllowed(target->field_0050)))
            {
                position =
                    ((float* (__fastcall*)(void*, void*, float*))
                        CSF_AT(void**, target, 0)[0xA4 / 4])(
                            target, nullptr, vectors);
            }
            else
            {
                position = reinterpret_cast<const float*>(
                    GetObjectPositionVector(target, nullptr));
            }

            vectors[3] = position[0];
            vectors[4] = position[1];
            vectors[5] = position[2];
            void* model = CSF_AT(void*, self, 0x4);

            CSF_AT(float, self, 0x110) =
                (vectors[3] - CSF_AT(float, self, 0x110)) *
                CSF_AT(float, self, 0x10C) +
                CSF_AT(float, self, 0x110);
            CSF_AT(float, self, 0x114) =
                (vectors[4] - CSF_AT(float, self, 0x114)) *
                CSF_AT(float, self, 0x10C) +
                CSF_AT(float, self, 0x114);
            CSF_AT(float, self, 0x118) =
                (vectors[5] - CSF_AT(float, self, 0x118)) *
                CSF_AT(float, self, 0x10C) +
                CSF_AT(float, self, 0x118);

            void* transform =
                ((void* (__fastcall*)(void*, void*))
                    CSF_AT(void**, model, 0)[0x10 / 4])(model, nullptr);
            ((void (__fastcall*)(void*, void*, void*))
                CSF_AT(void**, transform, 0)[0x54 / 4])(
                    transform, nullptr, bytes + 0x110);
        }
    }

    CharacterModel_UpdateModel(self, nullptr, deltaTime);
}

HOOK(0x004F6F30, CharacterModel_UpdateTrackedTransform, "auto");
