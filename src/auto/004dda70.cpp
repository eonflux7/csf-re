#include <cstdint>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t, __fastcall, CGuiControl_GetField012C, 0x0044F630, (CGuiControl* self, void*));
CSF_FUNC(int, __cdecl, C_0083e0d8_CopyPointLinks, 0x004DC730, (int a1, int a2, int a3, int a4, int a5));
CSF_FUNC(unsigned int, __fastcall, C_0083e0d8_InsertPointLinks, 0x004DD270, (void* self, void* edx, char* a1, unsigned int a3, uint32_t* arg8));
CSF_FUNC(unsigned int, __fastcall, C_0083e0d8_ReservePointLinks, 0x004DD510, (void* self, void* edx, unsigned int a2));
CSF_FUNC(int, __fastcall, SceneFindSubObject, 0x004DE7A0, (void* self, void* , unsigned int a2));
CSF_FUNC(double, __fastcall, S_004e0c20_DistanceToPoint, 0x004E0F60, (S_004e0c20* self, void* , float* a2));
CSF_FUNC(unsigned int, __fastcall, C_0086c288_ResetTimerState, 0x0075BF30, (C_0086c288* self, void*));
CSF_FUNC(int, __fastcall, C_0083e0d8_CountPointsOfKind, 0x0075C520, (C_0083e0d8* self, void* , int kind));
CSF_FUNC(int, __fastcall, C_0083e0d8_GetRecordAddress, 0x0075CF00, (C_0083e0d8* self, void* , int recordIndex));

CSF_GLOBAL(float, g_Const200_0f, 0x0082DDEC);
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static int __fastcall C_0083e0d8_RebuildPointSubObjectLinks(
    C_0083e0d8* self, void* /*edx*/)
{
    C_0086c288_ResetTimerState(self, nullptr);
    const uint32_t sceneData = CGuiControl_GetField012C(
        reinterpret_cast<CGuiControl*>(g_pCurrentScene), nullptr);
    if (!sceneData)
        return static_cast<int>(sceneData);

    const int pointCount = C_0083e0d8_CountPointsOfKind(self, nullptr, 3);
    auto* links = reinterpret_cast<uint8_t*>(self) + 0x64;
    C_0083e0d8_ReservePointLinks(
        links, nullptr, static_cast<unsigned int>(pointCount));

    const uint32_t recordsBegin = self->field_0008;
    const uint32_t recordCount = recordsBegin
        ? static_cast<uint32_t>(
            static_cast<int32_t>(self->field_000c - recordsBegin) / 72)
        : 0;

    alignas(8) uint8_t linkBytes[0x30];
    auto* link = reinterpret_cast<uint32_t*>(linkBytes);

    for (uint32_t recordIndex = 0; recordIndex < recordCount; ++recordIndex)
    {
        auto* record = reinterpret_cast<C_0083e0d8*>(
            C_0083e0d8_GetRecordAddress(
                self, nullptr, static_cast<int>(recordIndex)));
        if (record->field_0020 != 3)
            continue;

        const uint32_t pointsBegin = record->field_0038;
        const uint32_t count = pointsBegin
            ? static_cast<uint32_t>(
                static_cast<int32_t>(record->field_003c - pointsBegin) / 44)
            : 0;
        if (!count)
            continue;

        link[3] = 0;
        link[0] = recordIndex;
        uint32_t pointOffset = 0;

        for (uint32_t pointIndex = 0; pointIndex < count; ++pointIndex)
        {
            auto* point = reinterpret_cast<float*>(
                record->field_0038 + pointOffset + 4);
            auto* subObject = reinterpret_cast<S_004e0c20*>(
                SceneFindSubObject(
                    reinterpret_cast<void*>(sceneData), nullptr,
                    reinterpret_cast<unsigned int>(point)));

            if (subObject &&
                std::fabs(S_004e0c20_DistanceToPoint(
                    subObject, nullptr, point)) > g_Const200_0f)
            {
                subObject = nullptr;
            }

            const uint32_t begin = CSF_AT(uint32_t, links, 0x4);
            link[1] = pointIndex;
            link[2] = reinterpret_cast<uint32_t>(subObject);

            const uint32_t size = begin
                ? static_cast<uint32_t>(
                    static_cast<int32_t>(
                        CSF_AT(uint32_t, links, 0x8) - begin) >> 4)
                : 0;

            if (begin &&
                size < static_cast<uint32_t>(
                    static_cast<int32_t>(
                        CSF_AT(uint32_t, links, 0xC) - begin) >> 4))
            {
                const uint32_t end = CSF_AT(uint32_t, links, 0x8);
                C_0083e0d8_CopyPointLinks(
                    static_cast<int>(end), 1,
                    reinterpret_cast<int>(link),
                    reinterpret_cast<int>(links),
                    reinterpret_cast<int>(self));
                CSF_AT(uint32_t, links, 0x8) = end + 16;
            }
            else
            {
                C_0083e0d8_InsertPointLinks(
                    links, nullptr,
                    reinterpret_cast<char*>(CSF_AT(uint32_t, links, 0x8)),
                    1, link);
            }

            pointOffset += 44;
        }
    }

    return static_cast<int>(recordCount);
}

HOOK(0x004DDA70, C_0083e0d8_RebuildPointSubObjectLinks, "auto");
