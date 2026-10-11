#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

CSF_FUNC(int, __fastcall, GrowSceneObjectRectStorage, 0x00445A80,
         (void* self, void* edx, int a2));
CSF_FUNC(int, __stdcall, ResolveZeroSceneObjectRect, 0x00446910,
         (int a1, float arg4));

static uint32_t* __fastcall ResolveSceneObjectRect(
    void* self, void* /*edx*/, float* rect)
{
    if (rect[5] == g_fZero)
    {
        return reinterpret_cast<uint32_t*>(
            ResolveZeroSceneObjectRect(
                reinterpret_cast<int>(rect), 1.0f));
    }

    const uint32_t count = CSF_AT(uint32_t, self, 0x34);
    uint32_t index = 0;

    if (count != 0)
    {
        uint8_t* record = CSF_AT(uint8_t*, self, 0x2C);
        while (index < count)
        {
            if (CSF_AT(int32_t, record, 0) == -1)
            {
                if (index != UINT32_MAX)
                {
                    std::memcpy(record, rect, 0x20);
                    return reinterpret_cast<uint32_t*>(record);
                }
                break;
            }

            ++index;
            record += 0x20;
        }
    }

    if (CSF_AT(uint32_t, self, 0x34) >=
        CSF_AT(uint32_t, self, 0x30))
    {
        GrowSceneObjectRectStorage(
            static_cast<uint8_t*>(self) + 0x2C,
            nullptr,
            CSF_AT(int, self, 0x38));
    }

    const uint32_t appendIndex = CSF_AT(uint32_t, self, 0x34);
    uint8_t* destination =
        CSF_AT(uint8_t*, self, 0x2C) + 0x20 * appendIndex;
    const uint32_t newCount = appendIndex + 1;
    CSF_AT(uint32_t, self, 0x34) = newCount;
    std::memcpy(destination, rect, 0x20);

    return reinterpret_cast<uint32_t*>(
        static_cast<uintptr_t>(newCount));
}

HOOK(0x00447130, ResolveSceneObjectRect, "auto");
