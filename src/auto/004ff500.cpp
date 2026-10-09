#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __stdcall, SceneLoader_CreateQuadDebugGeometry, 0x004FDCA0, (int a1));
CSF_FUNC(uint32_t*, __fastcall, SceneLoader_GrowQuadArray, 0x004FF020, (void* self, void* edx, int a2));

static void __fastcall SceneLoader_AddOrUpdateVerticalQuad(
    void* self, void* /*edx*/, int category, float* first,
    float* second, float height, int attribute, int identifier)
{
    if (CSF_AT(uint32_t, self, 0x44) != 0)
    {
        auto* records = CSF_AT(uint8_t*, self, 0x3C);
        uint32_t index = 0;
        auto* record = records;

        do
        {
            if (CSF_AT(int, record, 0x04) == category &&
                CSF_AT(float, record, 0x08) == first[0] &&
                CSF_AT(float, record, 0x0C) == first[1] &&
                CSF_AT(float, record, 0x10) == first[2] &&
                CSF_AT(float, record, 0x14) == second[0] &&
                CSF_AT(float, record, 0x18) == second[1] &&
                CSF_AT(float, record, 0x1C) == second[2])
            {
                CSF_AT(int, records + 0x44 * index, 0x3C) = identifier;
                return;
            }

            ++index;
            record += 0x44;
        } while (index < CSF_AT(uint32_t, self, 0x44));
    }

    alignas(4) uint8_t record[0x50] = {};

    const float secondTopY = height + second[1];
    CSF_AT(int, record, 0x04) = category;
    CSF_AT(float, record, 0x08) = first[0];

    const float firstTopY = height + first[1];
    CSF_AT(float, record, 0x0C) = first[1];
    CSF_AT(float, record, 0x10) = first[2];
    CSF_AT(float, record, 0x14) = second[0];
    CSF_AT(float, record, 0x18) = second[1];
    CSF_AT(float, record, 0x1C) = second[2];

    CSF_AT(float, record, 0x20) = second[0];
    CSF_AT(float, record, 0x24) = secondTopY;
    CSF_AT(float, record, 0x28) = second[2];
    CSF_AT(float, record, 0x2C) = first[0];
    CSF_AT(float, record, 0x30) = firstTopY;
    CSF_AT(float, record, 0x34) = first[2];

    CSF_AT(int, record, 0x38) = attribute;
    CSF_AT(int, record, 0x3C) = identifier;
    CSF_AT(uint32_t, record, 0x40) = 0;
    CSF_AT(uint8_t, record, 0x00) = 1;

    CSF_AT(float, record, 0x44) = CSF_AT(float, record, 0x2C);
    CSF_AT(float, record, 0x48) = firstTopY;
    CSF_AT(float, record, 0x4C) = CSF_AT(float, record, 0x34);

    if (CSF_AT(uint8_t, self, 0x08) != 0)
        SceneLoader_CreateQuadDebugGeometry(reinterpret_cast<int>(record));

    if (CSF_AT(uint32_t, self, 0x44) >= CSF_AT(uint32_t, self, 0x40))
    {
        SceneLoader_GrowQuadArray(
            static_cast<uint8_t*>(self) + 0x3C, nullptr,
            CSF_AT(int, self, 0x48));
    }

    const uint32_t count = CSF_AT(uint32_t, self, 0x44);
    auto* destination = CSF_AT(uint8_t*, self, 0x3C) + 0x44 * count;
    CSF_AT(uint32_t, self, 0x44) = count + 1;
    std::memcpy(destination, record, 0x44);
}

HOOK(0x004FF500, SceneLoader_AddOrUpdateVerticalQuad, "auto");
