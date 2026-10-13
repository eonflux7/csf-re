#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(float*, __cdecl, GfxMatrix_ClosestPointOnOrientedBox, 0x006AD550,
         (float* result, int point, float* box));
CSF_FUNC(int, __fastcall, InsertHitRecord, 0x006E6E00,
         (void* self, void* edx, int a2, void* a3));
CSF_FUNC(char, __fastcall, GrowHitRecordBuffer, 0x006E6F40,
         (void* self, void* edx, int a2));

CSF_GLOBAL(float, g_HalfDurationScale, 0x0082D444);
CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

static char __fastcall C_008632c0_CollectSortedIntersections(
    C_008632c0* self, void* /*edx*/, int query, int outputAddress, float radius)
{
    alignas(8) uint8_t boxBytes[0x3C];
    alignas(8) uint8_t closestPointBytes[0xC];
    alignas(8) uint8_t aggregateHitBytes[0x1C];
    alignas(8) uint8_t boundBytes[0x4];
    alignas(8) uint8_t recordBytes[0x2C];
    alignas(8) uint8_t hitBytes[0x2C];

    char foundAny = 0;

    reinterpret_cast<void(__fastcall*)(void*, void*)>(
        CSF_AT(void**, self, 0)[0x58 / 4])(self, nullptr);

    void* bounds = reinterpret_cast<void*>(CSF_AT(uint32_t, self, 0x24));
    const float expandedRadius = radius + g_HalfDurationScale;
    CSF_AT(int, boundBytes, 0) = 2139095039;
    if (!reinterpret_cast<bool(__fastcall*)(void*, void*, int, void*, float)>(
            CSF_AT(void**, bounds, 0)[0x10 / 4])(
            bounds, nullptr, query, boundBytes, expandedRadius))
        return foundAny;

    void* output = reinterpret_cast<void*>(outputAddress);
    CSF_AT(uint8_t, output, 0x10) = 1;

    reinterpret_cast<void(__fastcall*)(void*, void*)>(
        CSF_AT(void**, self, 0)[0x5C / 4])(self, nullptr);

    void* aggregate = reinterpret_cast<void*>(self->field_004c);
    const bool hasAggregate = aggregate != nullptr;
    if (hasAggregate)
    {
        std::memset(aggregateHitBytes, 0, 24);
        if (!reinterpret_cast<bool(__fastcall*)(void*, void*, int, void*, float)>(
                CSF_AT(void**, aggregate, 0)[0x14 / 4])(
                aggregate, nullptr, query, aggregateHitBytes, 0.0f))
            return foundAny;
    }

    uint32_t childIndex = 0;
    uint32_t childOffset = 0;
    if (CSF_AT(uint32_t, self, 0x30) != 0)
    {
        do
        {
            void* entries = reinterpret_cast<void*>(CSF_AT(uint32_t, self, 0x28));
            const int entryId = CSF_AT(int, entries, childOffset + 4);
            int recordId = -1;
            bool eligible = true;
            if (entryId >= 0)
            {
                recordId = entryId;
                C_00833908* manager =
                    reinterpret_cast<C_00833908*>(self->field_0008);
                void* related = reinterpret_cast<void*(__fastcall*)(
                    void*, void*, int)>(manager->vtbl[0x80 / 4])(
                        manager, nullptr, entryId);
                eligible = reinterpret_cast<bool(__fastcall*)(void*, void*)>(
                    CSF_AT(void**, related, 0)[0x1C / 4])(related, nullptr);
            }

            if (eligible)
            {
                entries = reinterpret_cast<void*>(CSF_AT(uint32_t, self, 0x28));
                std::memset(hitBytes, 0, 24);
                void* shape = CSF_AT(void*, entries, childOffset);
                if (reinterpret_cast<bool(__fastcall*)(
                        void*, void*, int, void*, float)>(
                        CSF_AT(void**, shape, 0)[0x14 / 4])(
                        shape, nullptr, query, hitBytes,
                        hasAggregate ? 0.0f : radius))
                {
                    bool insertBefore = false;
                    int insertionIndex = -1;
                    uint32_t scanIndex = 0;
                    uint32_t scanOffset = 0;

                    if (!hasAggregate)
                        foundAny = 0;

                    const uint32_t count = CSF_AT(uint32_t, output, 8);
                    if (hasAggregate ? childIndex < count : count != 0)
                    {
                        do
                        {
                            if (insertBefore)
                                break;

                            void* records = CSF_AT(void*, output, 0);
                            if (CSF_AT(float, hitBytes, 0x18) <
                                CSF_AT(float, records, scanOffset + 36))
                            {
                                insertBefore = true;
                                insertionIndex = static_cast<int>(scanIndex);
                            }
                            ++scanIndex;
                            scanOffset += 40;

                            // The aggregate path deliberately tests the child
                            // index, not the advancing scan index.
                        } while (hasAggregate
                                     ? childIndex < CSF_AT(uint32_t, output, 8)
                                     : scanIndex < CSF_AT(uint32_t, output, 8));
                    }

                    if (!hasAggregate && radius != g_fZero)
                    {
                        entries = reinterpret_cast<void*>(
                            CSF_AT(uint32_t, self, 0x28));
                        shape = CSF_AT(void*, entries, childOffset);
                        reinterpret_cast<void(__fastcall*)(void*, void*, void*)>(
                            CSF_AT(void**, shape, 0)[0x1C / 4])(
                            shape, nullptr, boxBytes);

                        float* closest = GfxMatrix_ClosestPointOnOrientedBox(
                            reinterpret_cast<float*>(closestPointBytes),
                            reinterpret_cast<int>(hitBytes),
                            reinterpret_cast<float*>(boxBytes));
                        std::memcpy(hitBytes, closest, 12);
                    }

                    std::memcpy(recordBytes, hitBytes, 24);
                    CSF_AT(uint32_t, recordBytes, 36) =
                        CSF_AT(uint32_t, hitBytes, 0x18);

                    entries = reinterpret_cast<void*>(
                        CSF_AT(uint32_t, self, 0x28));
                    CSF_AT(uint32_t, recordBytes, 24) =
                        CSF_AT(uint32_t, entries, childOffset + 8);
                    CSF_AT(uint32_t, recordBytes, 28) =
                        CSF_AT(uint32_t, entries, childOffset + 12);
                    CSF_AT(int, recordBytes, 32) = recordId;

                    if (insertBefore)
                    {
                        InsertHitRecord(output, nullptr, insertionIndex, recordBytes);
                    }
                    else
                    {
                        if (CSF_AT(uint32_t, output, 8) >=
                            CSF_AT(uint32_t, output, 4))
                        {
                            GrowHitRecordBuffer(
                                output, nullptr, CSF_AT(int, output, 0xC));
                        }

                        const uint32_t appendIndex =
                            CSF_AT(uint32_t, output, 8);
                        uint8_t* records = CSF_AT(uint8_t*, output, 0);
                        CSF_AT(uint32_t, output, 8) = appendIndex + 1;
                        std::memcpy(records + 40 * appendIndex, recordBytes, 0x28);
                    }

                    if (!hasAggregate)
                        foundAny = 1;
                }
            }

            childOffset += 16;
            ++childIndex;
        } while (childIndex < CSF_AT(uint32_t, self, 0x30));
    }

    if (!hasAggregate)
        return foundAny;

    if (CSF_AT(uint32_t, output, 8) == 0)
        return 0;

    void* firstRecord = CSF_AT(void*, output, 0);
    std::memcpy(firstRecord, aggregateHitBytes, 12);
    uint8_t* firstNormal = CSF_AT(uint8_t*, output, 0) + 12;
    std::memcpy(firstNormal, aggregateHitBytes + 12, 12);
    return 1;
}

HOOK(0x00706A50, C_008632c0_CollectSortedIntersections, "auto");
