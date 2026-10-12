#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __fastcall, VehicleWheel_GrowArray, 0x0040CDB0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, Vehicle_SerializeWheelValue, 0x0040F840, (void* self, void* edx, int a2));

static int __stdcall Vehicle_SerializeBaseValue(int* array, void* serializer)
{
    const bool writing = CSF_AT(std::uint8_t, serializer, 0x14) != 0;
    const std::uint8_t savedMode = CSF_AT(std::uint8_t, serializer, 0x15);

    alignas(8) std::uint8_t countBytes[0x4] = {};
    auto& count = *reinterpret_cast<std::uint32_t*>(countBytes);
    if (writing)
        count = CSF_AT(std::uint32_t, array, 0x8);

    bool failed = false;
    if (CSF_AT(std::uint8_t, serializer, 0) == 0)
    {
        void* backend = CSF_AT(void*, serializer, 0x4);
        failed = !reinterpret_cast<bool(__fastcall*)(
            void*, void*, std::uint32_t*, void*)>(
                CSF_AT(void**, backend, 0)[0x78 / 4])(
                    backend, nullptr, &count, serializer);
    }
    CSF_AT(std::uint8_t, serializer, 0) = failed;

    if (!failed)
    {
        if (writing || savedMode != 0)
        {
            for (std::uint32_t i = 0, offset = 0; i < count; ++i, offset += 24)
            {
                const bool canSerialize = CSF_AT(std::uint8_t, serializer, 0) == 0;
                CSF_AT(std::uint32_t, serializer, 0x1C) = 0;
                if (canSerialize)
                {
                    CSF_AT(std::uint8_t, serializer, 0x15) = savedMode;
                    CSF_AT(std::uint8_t, serializer, 0x16) = 0;
                    void* entry = static_cast<std::uint8_t*>(
                        CSF_AT(void*, array, 0)) + offset;
                    Vehicle_SerializeWheelValue(
                        entry, nullptr, reinterpret_cast<int>(serializer));
                }
            }
        }
        else
        {
            const std::uint32_t capacity = CSF_AT(std::uint32_t, array, 0x4);
            CSF_AT(std::uint32_t, array, 0x8) = 0;
            if (count > capacity)
                VehicleWheel_GrowArray(array, nullptr, static_cast<int>(count - capacity));

            for (std::uint32_t i = 0, offset = 0; i < count; ++i, offset += 24)
            {
                if (CSF_AT(std::uint32_t, array, 0x8) >=
                    CSF_AT(std::uint32_t, array, 0x4))
                {
                    VehicleWheel_GrowArray(
                        array, nullptr, CSF_AT(int, array, 0xC));
                }

                const std::uint32_t index = CSF_AT(std::uint32_t, array, 0x8);
                auto* entries = static_cast<std::uint8_t*>(CSF_AT(void*, array, 0));
                CSF_AT(std::uint32_t, array, 0x8) = index + 1;
                void* entry = entries + 24 * index;
                CSF_AT(std::uint32_t, entry, 0) = 0;
                CSF_AT(std::uint32_t, entry, 0x4) = 0;
                CSF_AT(std::uint32_t, entry, 0x8) = 0;
                CSF_AT(std::uint32_t, entry, 0xC) = 0;
                CSF_AT(int, entry, 0x10) = -1;
                CSF_AT(int, entry, 0x14) = -1;

                const bool canSerialize = CSF_AT(std::uint8_t, serializer, 0) == 0;
                CSF_AT(std::uint32_t, serializer, 0x1C) = 0;
                if (canSerialize)
                {
                    CSF_AT(std::uint8_t, serializer, 0x15) = 0;
                    CSF_AT(std::uint8_t, serializer, 0x16) = 0;
                    entry = static_cast<std::uint8_t*>(CSF_AT(void*, array, 0)) + offset;
                    Vehicle_SerializeWheelValue(
                        entry, nullptr, reinterpret_cast<int>(serializer));
                }
            }
        }

        if (CSF_AT(std::uint8_t, serializer, 0) == 0)
        {
            void* backend = CSF_AT(void*, serializer, 0x4);
            if (reinterpret_cast<bool(__fastcall*)(void*, void*, void*)>(
                    CSF_AT(void**, backend, 0)[0x7C / 4])(
                        backend, nullptr, serializer))
            {
                CSF_AT(std::uint8_t, serializer, 0) = 0;
                return 0;
            }
        }
    }

    CSF_AT(std::uint8_t, serializer, 0) = 1;
    return 1;
}

HOOK(0x0040D5E0, Vehicle_SerializeBaseValue, "auto");
