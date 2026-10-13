#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(uint32_t*, __fastcall, AiCore_ConstructStateData, 0x004A9D80, (uint32_t* self, void*));
CSF_FUNC(int, __cdecl, CoreMemory_AllocUntagged, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));

static char* __fastcall AiDistracted_GrowPointList(
    void* self, void* /*edx*/, int additionalCapacity)
{
    const uint32_t newCapacity =
        CSF_AT(uint32_t, self, 0x04) +
        static_cast<uint32_t>(additionalCapacity);

    char* newPoints = reinterpret_cast<char*>(
        CoreMemory_AllocUntagged(20 * newCapacity));

    if (newPoints && static_cast<int32_t>(newCapacity - 1) >= 0)
    {
        char* point = newPoints;
        uint32_t remaining = newCapacity;
        do
        {
            AiCore_ConstructStateData(
                reinterpret_cast<uint32_t*>(point), nullptr);
            point += 20;
            --remaining;
        } while (remaining);
    }

    const uint32_t pointCount = CSF_AT(uint32_t, self, 0x08);
    if (pointCount != 0)
    {
        std::memcpy(newPoints, CSF_AT(void*, self, 0x00),
                    20 * pointCount);
    }

    const uint32_t updatedCapacity =
        CSF_AT(uint32_t, self, 0x04) +
        static_cast<uint32_t>(additionalCapacity);
    void* oldPoints = CSF_AT(void*, self, 0x00);
    CSF_AT(uint32_t, self, 0x04) = updatedCapacity;

    if (oldPoints)
    {
        char* result = reinterpret_cast<char*>(CoreMemory_Free(oldPoints));
        CSF_AT(void*, self, 0x00) = nullptr;
        CSF_AT(void*, self, 0x00) = newPoints;
        return result;
    }

    CSF_AT(void*, self, 0x00) = newPoints;
    return newPoints;
}

HOOK(0x004ADDB0, AiDistracted_GrowPointList, "auto");
