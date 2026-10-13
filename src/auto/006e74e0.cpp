#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

CSF_FUNC(float*, __cdecl, GfxMatrix_ClosestPointOnOrientedBox, 0x006AD550, (float* result, int point, float* box));
CSF_FUNC(char, __fastcall, GrowQueryResultBuffer, 0x006E6F40, (void* self, void* edx, int a2));

static char __fastcall C_00861d48_AppendQueryResult(
    C_00861d48* self, void* /*edx*/, int query, char** results, float distance)
{
    ((void (__fastcall*)(void*, void*))
        CSF_AT(void**, self, 0)[0x58 / 4])(self, nullptr);

    alignas(8) uint8_t hitBytes[0x1C];
    alignas(8) uint8_t boxBytes[0x3C];
    alignas(8) uint8_t closestBytes[0xC];
    std::memset(hitBytes, 0, sizeof(hitBytes));

    void* queryObject = CSF_AT(void*, self, 0x24);
    if (!((bool (__fastcall*)(void*, void*, int, void*, float))
        CSF_AT(void**, queryObject, 0)[0x14 / 4])(
            queryObject, nullptr, query, hitBytes, distance))
    {
        return 0;
    }

    if (distance != g_fZero)
    {
        queryObject = CSF_AT(void*, self, 0x24);
        ((void (__fastcall*)(void*, void*, void*))
            CSF_AT(void**, queryObject, 0)[0x1C / 4])(
                queryObject, nullptr, boxBytes);

        float* closest = GfxMatrix_ClosestPointOnOrientedBox(
            reinterpret_cast<float*>(closestBytes),
            reinterpret_cast<int>(hitBytes),
            reinterpret_cast<float*>(boxBytes));

        CSF_AT(uint32_t, hitBytes, 0x00) = CSF_AT(uint32_t, closest, 0x00);
        CSF_AT(uint32_t, hitBytes, 0x04) = CSF_AT(uint32_t, closest, 0x04);
        CSF_AT(uint32_t, hitBytes, 0x08) = CSF_AT(uint32_t, closest, 0x08);
    }

    uint32_t record[10];
    std::memcpy(record, hitBytes, 0x18);
    record[7] = static_cast<uint32_t>(-1);
    record[8] = static_cast<uint32_t>(-1);

    const bool hasCapacity =
        CSF_AT(uint32_t, results, 0x08) < CSF_AT(uint32_t, results, 0x04);
    CSF_AT(uint8_t, results, 0x10) = 1;
    record[6] = 0;
    record[9] = CSF_AT(uint32_t, hitBytes, 0x18);

    if (!hasCapacity)
        GrowQueryResultBuffer(results, nullptr, CSF_AT(int, results, 0x0C));

    const uint32_t index = CSF_AT(uint32_t, results, 0x08);
    char* data = CSF_AT(char*, results, 0x00);
    CSF_AT(uint32_t, results, 0x08) = index + 1;
    std::memcpy(data + 40 * index, record, 0x28);
    return 1;
}

HOOK(0x006E74E0, C_00861d48_AppendQueryResult, "auto");
