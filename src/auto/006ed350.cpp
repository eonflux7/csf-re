#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, GfxCollision_InsertRecord, 0x006EC7C0,
         (void* self, void* edx, int a2, void* a3));
CSF_FUNC(char, __fastcall, GfxCollision_GrowRecords, 0x006EC910,
         (void* self, void* edx, int a2));
CSF_FUNC(void*, __cdecl, _memcpy, 0x007AEE60,
         (void* destination, const void* source, std::size_t byteCount));

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_CollisionInsertionEpsilon, 0x00861FA8);

static char __fastcall GfxCollision_AddIntersection(
    void* self, void* /*edx*/, float* record, char terminate)
{
    auto* records = static_cast<std::uint8_t*>(self) + 0x4;
    float directionX = 0.0f;
    float directionY = 0.0f;
    float directionZ = 0.0f;

    if (CSF_AT(int, self, 0x34) == 1)
    {
        directionX = CSF_AT(float, self, 0x28) - CSF_AT(float, self, 0x1C);
        directionY = CSF_AT(float, self, 0x2C) - CSF_AT(float, self, 0x20);
        directionZ = CSF_AT(float, self, 0x30) - CSF_AT(float, self, 0x24);
    }

    const int originalCount = CSF_AT(int, self, 0xC);
    int index = 0;
    float distanceDifference = 0.0f;

    if (originalCount > 0)
    {
        auto* candidate = CSF_AT(std::uint8_t*, self, 0x4);
        while (index < originalCount)
        {
            distanceDifference = CSF_AT(float, candidate, 0) - record[0];
            if (distanceDifference > g_fZero)
                break;
            ++index;
            candidate += 0x68;
        }
    }

    if (index < originalCount)
    {
        bool insertBefore = !(distanceDifference < g_CollisionInsertionEpsilon);
        if (!insertBefore)
        {
            if (CSF_AT(int, self, 0x34) == 3)
            {
                auto* candidate = CSF_AT(std::uint8_t*, records, 0) + 104 * index;
                directionX = CSF_AT(float, candidate, 0x4) - CSF_AT(float, self, 0x1C);
                directionY = CSF_AT(float, candidate, 0x8) - CSF_AT(float, self, 0x20);
                directionZ = CSF_AT(float, candidate, 0xC) - CSF_AT(float, self, 0x24);
            }

            auto* candidate = CSF_AT(std::uint8_t*, records, 0) + 104 * index;
            const float projection =
                directionY * CSF_AT(float, candidate, 0x14) +
                directionX * CSF_AT(float, candidate, 0x10) +
                directionZ * CSF_AT(float, candidate, 0x18);
            insertBefore = !(projection < g_CollisionInsertionEpsilon);
        }

        if (insertBefore)
        {
            GfxCollision_InsertRecord(records, nullptr, index, record);
        }
        else if (index >= originalCount - 1)
        {
            GfxCollision_InsertRecord(records, nullptr, index + 1, record);
        }
        else
        {
            if (CSF_AT(std::uint32_t, records, 0x8) >=
                CSF_AT(std::uint32_t, records, 0x4))
            {
                GfxCollision_GrowRecords(records, nullptr, CSF_AT(int, records, 0xC));
            }

            const std::uint32_t count = CSF_AT(std::uint32_t, records, 0x8);
            auto* destination = CSF_AT(std::uint8_t*, records, 0) + 104 * count;
            CSF_AT(std::uint32_t, records, 0x8) = count + 1;
            for (std::size_t offset = 0; offset < 0x68; offset += 4)
                CSF_AT(std::uint32_t, destination, offset) =
                    CSF_AT(std::uint32_t, record, offset);
        }

        if (terminate)
        {
            int removeIndex = CSF_AT(int, self, 0xC) - 1;
            int offset = 104 * removeIndex;
            while (removeIndex > index)
            {
                const int newCount = CSF_AT(int, records, 0x8) - 1;
                CSF_AT(int, records, 0x8) = newCount;
                if (removeIndex != newCount)
                {
                    auto* destination = CSF_AT(std::uint8_t*, records, 0) + offset;
                    _memcpy(destination, destination + 104,
                            static_cast<std::size_t>(104 * (newCount - removeIndex)));
                }
                --removeIndex;
                offset -= 104;
            }
            CSF_AT(char, self, 0x19) = 1;
        }

        CSF_AT(char, self, 0x18) = 0;
        CSF_AT(int, self, 0x14) = 0;
        return static_cast<char>(reinterpret_cast<std::uintptr_t>(self));
    }

    const char alreadyTerminated = CSF_AT(char, self, 0x19);
    if (alreadyTerminated)
    {
        CSF_AT(char, self, 0x18) = 0;
        CSF_AT(int, self, 0x14) = 0;
        return alreadyTerminated;
    }

    if (CSF_AT(std::uint32_t, self, 0xC) >=
        CSF_AT(std::uint32_t, self, 0x8))
    {
        GfxCollision_GrowRecords(records, nullptr, CSF_AT(int, self, 0x10));
    }

    const std::uint32_t count = CSF_AT(std::uint32_t, records, 0x8);
    auto* destination = CSF_AT(std::uint8_t*, records, 0) + 104 * count;
    CSF_AT(std::uint32_t, records, 0x8) = count + 1;
    for (std::size_t offset = 0; offset < 0x68; offset += 4)
        CSF_AT(std::uint32_t, destination, offset) =
            CSF_AT(std::uint32_t, record, offset);

    CSF_AT(char, self, 0x19) = terminate;
    CSF_AT(char, self, 0x18) = 0;
    CSF_AT(int, self, 0x14) = 0;
    return static_cast<char>(reinterpret_cast<std::uintptr_t>(self));
}

HOOK(0x006ED350, GfxCollision_AddIntersection, "auto");
