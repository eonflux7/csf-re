#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_GLOBAL(float, g_DegreesToRadians, 0x0082D754);

static void __fastcall TargetPoint_SetAngles(
    void* self, void* /*edx*/, int index, const std::uint32_t* angles)
{
    auto* point = CSF_AT(std::uint8_t*, self, 0x0C)
        + 44 * static_cast<std::uint32_t>(index);

    if (!CSF_AT(std::uint8_t, point, 0x00)
        || CSF_AT(std::uint32_t, point, 0x04) != 1)
    {
        CSF_AT(std::uint32_t, point, 0x18) = 0;
        CSF_AT(std::uint32_t, point, 0x28) = 0;
    }

    CSF_AT(std::uint8_t, point, 0x00) = 1;
    CSF_AT(std::uint32_t, point, 0x04) = 1;
    CSF_AT(std::uint32_t, point, 0x14) = angles[0];

    const std::uint32_t secondAngle = angles[1];
    const std::uint32_t initialFirstAngle =
        CSF_AT(std::uint32_t, point, 0x0C);
    CSF_AT(std::uint32_t, point, 0x24) = secondAngle;

    if (initialFirstAngle == 2139095039)
    {
        void* owner = CSF_AT(void*, self, 0x08);
        void* entity = reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, owner, 0)[10])(owner, nullptr);
        void* orientation = reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, entity, 0)[6])(entity, nullptr);
        const double angle = reinterpret_cast<double (__fastcall*)(void*, void*)>(
            CSF_AT(void**, orientation, 0)[9])(orientation, nullptr);
        CSF_AT(float, point, 0x0C) =
            static_cast<float>(angle * g_DegreesToRadians);
    }

    if (CSF_AT(std::uint32_t, point, 0x10) == 2139095039)
        CSF_AT(std::uint32_t, point, 0x10) =
            CSF_AT(std::uint32_t, point, 0x0C);

    if (CSF_AT(std::uint32_t, point, 0x1C) == 2139095039)
    {
        void* owner = CSF_AT(void*, self, 0x08);
        void* entity = reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, owner, 0)[10])(owner, nullptr);
        void* orientation = reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, entity, 0)[6])(entity, nullptr);
        const double angle = reinterpret_cast<double (__fastcall*)(void*, void*)>(
            CSF_AT(void**, orientation, 0)[10])(orientation, nullptr);
        CSF_AT(float, point, 0x1C) =
            static_cast<float>(angle * g_DegreesToRadians);
    }

    if (CSF_AT(std::uint32_t, point, 0x20) == 2139095039)
        CSF_AT(std::uint32_t, point, 0x20) =
            CSF_AT(std::uint32_t, point, 0x1C);
}

HOOK(0x00406C90, TargetPoint_SetAngles, "auto");
