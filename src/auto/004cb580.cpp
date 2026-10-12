#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, AiSearchOrders_ResolveRecordPoint, 0x004A9DE0, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(uint32_t*, __fastcall, AiDistracted_GrowPointList, 0x004ADDB0, (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, AiSearchOrders_BuildFallbackPoints, 0x004CB490, (void* self, void* edx, int a2, float a3, float a4, int* a5));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(int, __fastcall, C_0083e0d8_GetRecordAddress, 0x0075CF00, (C_0083e0d8* self, void* , int recordIndex));

// The packet also requests a vtable binding for this direct call target.
[[maybe_unused]] static const auto kFallbackVtable = CSF_VTBL(0x004CB490);

static char __fastcall S_004cb490_BuildSearchPoints(
    S_004cb490* self, void* /*edx*/, float a2, float a3, int* points)
{
    points[2] = 0;
    uint32_t recordIndex = self->field_06a8;

    if (recordIndex == UINT32_MAX)
    {
        void* link = reinterpret_cast<uint8_t*>(self) + 8;
        int linkedValue = GetLinkedObjectValue_4DB5C0(link, nullptr);
        void* linkedObject = reinterpret_cast<void*>(
            linkedValue ? static_cast<uint32_t>(linkedValue) - 4 : 0);

        void* owner = CSF_AT(void*, linkedObject, 0x2C);
        void* state = CSF_AT(void*, owner, 0x34);
        if (CSF_AT(uint32_t, state, 0x304) != 1)
            return AiSearchOrders_BuildFallbackPoints(self, nullptr, 3, a2, a3, points);

        linkedValue = GetLinkedObjectValue_4DB5C0(link, nullptr);
        linkedObject = reinterpret_cast<void*>(
            linkedValue ? static_cast<uint32_t>(linkedValue) - 4 : 0);

        if (CSF_AT(uint32_t, linkedObject, 0x6C8) == 0 ||
            CSF_AT(uint32_t, linkedObject, 0x6C0) == UINT32_MAX ||
            CSF_AT(uint32_t, linkedObject, 0x6C4) == UINT32_MAX)
        {
            return AiSearchOrders_BuildFallbackPoints(self, nullptr, 3, a2, a3, points);
        }

        recordIndex = CSF_AT(uint32_t, self, 0x6C0);
        if (recordIndex == UINT32_MAX)
            return AiSearchOrders_BuildFallbackPoints(self, nullptr, 3, a2, a3, points);
    }

    void* manager = reinterpret_cast<void*>(GetValue8B0554());
    C_0083e0d8* records = CSF_AT(C_0083e0d8*, manager, 0x1FD8);
    void* record = reinterpret_cast<void*>(
        C_0083e0d8_GetRecordAddress(records, nullptr, static_cast<int>(recordIndex)));

    if (CSF_AT(uint32_t, record, 0x20) == 3)
    {
        alignas(8) uint8_t pointBytes[0x14];
        alignas(8) uint8_t selectionBytes[0x8];

        uint32_t begin = CSF_AT(uint32_t, record, 0x38);
        CSF_AT(int32_t, selectionBytes, 4) = -1;
        CSF_AT(uint32_t, selectionBytes, 0) = recordIndex;

        uint32_t pointCount = 0;
        if (begin != 0)
        {
            uint32_t end = CSF_AT(uint32_t, record, 0x3C);
            int32_t byteDifference = static_cast<int32_t>(end - begin);
            // Signed division by 44, retaining the original reciprocal literal.
            int64_t product = static_cast<int64_t>(byteDifference) * 0x2E8BA2E9;
            int32_t scaledCount = static_cast<int32_t>(product >> 32) >> 3;
            pointCount = static_cast<uint32_t>(scaledCount) +
                         (static_cast<uint32_t>(scaledCount) >> 31);
        }

        uint32_t capacity = static_cast<uint32_t>(points[1]);
        if (pointCount > capacity)
        {
            AiDistracted_GrowPointList(
                points, nullptr, static_cast<int>(pointCount - capacity));
        }

        for (uint32_t i = 0; i < pointCount; ++i)
        {
            CSF_AT(uint32_t, selectionBytes, 4) = i;
            AiSearchOrders_ResolveRecordPoint(
                pointBytes, nullptr, reinterpret_cast<uint32_t*>(selectionBytes));

            if (static_cast<uint32_t>(points[2]) >= static_cast<uint32_t>(points[1]))
                AiDistracted_GrowPointList(points, nullptr, points[3]);

            uint32_t count = static_cast<uint32_t>(points[2]);
            uint8_t* data = reinterpret_cast<uint8_t*>(points[0]);
            points[2] = static_cast<int32_t>(count + 1);
            uint8_t* destination = data + 20 * count;

            CSF_AT(uint32_t, destination, 0) = CSF_AT(uint32_t, pointBytes, 0);
            CSF_AT(uint32_t, destination, 4) = CSF_AT(uint32_t, pointBytes, 4);
            CSF_AT(uint32_t, destination, 8) = CSF_AT(uint32_t, pointBytes, 8);
            CSF_AT(uint32_t, destination, 0xC) = CSF_AT(uint32_t, pointBytes, 0xC);
            CSF_AT(uint32_t, destination, 0x10) = CSF_AT(uint32_t, pointBytes, 0x10);
        }
    }

    return 1;
}

HOOK(0x004CB580, S_004cb490_BuildSearchPoints, "auto");
