#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_GLOBAL(float, g_ZeroVectorX, 0x008BB420);
CSF_GLOBAL(float, g_ZeroVectorY, 0x008BB424);
CSF_GLOBAL(float, g_ZeroVectorZ, 0x008BB428);

CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(int, __fastcall, RunSpatialHitQuery, 0x0075C7C0, (void* self, void* edx, float* a2, int a3, unsigned int* a4));
CSF_FUNC(int, __fastcall, GetHitRecordByIndex, 0x0075CF00, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, GetHitEntryPosition, 0x0075D000, (void* self, void* edx, uint32_t* a2));

// Pointer stored at offset 4 of the output record (re-read on every use, as the original does).
static uint8_t* OutputBlockOf(int outputRecord)
{
    return CSF_AT(uint8_t*, reinterpret_cast<uint8_t*>(outputRecord), 4);
}

static int __stdcall ResolveEntityHitPosition(int outputRecord, [[maybe_unused]] int unusedArg, uint32_t* entityRef)
{
    // Zero the output position (block+4..+0x0C) and reset both hit ids (block+0x10, block+0x14).
    float* zeroPosition = reinterpret_cast<float*>(OutputBlockOf(outputRecord) + 4);
    zeroPosition[0] = g_ZeroVectorX;
    zeroPosition[1] = g_ZeroVectorY;
    zeroPosition[2] = g_ZeroVectorZ;
    CSF_AT(int, OutputBlockOf(outputRecord), 16) = -1;
    CSF_AT(int, OutputBlockOf(outputRecord), 20) = -1;

    // Hit manager lives at offset 8152 of the value returned by GetValue8B0554().
    C_0083e0d8* hitManager = CSF_AT(C_0083e0d8*, reinterpret_cast<uint8_t*>(GetValue8B0554()), 8152);

    // Source position: *(*entityRef + 4) + 4, copied into a query-local vector.
    float* entityPosition = reinterpret_cast<float*>(CSF_AT(uint8_t*, reinterpret_cast<uint8_t*>(*entityRef), 4) + 4);
    float queryPosition[3];
    queryPosition[0] = entityPosition[0];
    queryPosition[1] = entityPosition[1];
    queryPosition[2] = entityPosition[2];

    // Second entity reference supplies the query value: *(*(entityRef[1] + 4) + 4).
    uint8_t* secondaryRef = reinterpret_cast<uint8_t*>(entityRef[1]);

    // Query argument block, laid out as the callee expects: kind, flag byte, value.
    struct RangeQueryArgs
    {
        int queryKind;      // +0
        uint8_t queryFlag;  // +4
        uint8_t pad[3];     // +5..+7
        int queryValue;     // +8
    };
    RangeQueryArgs query{};
    query.queryKind = 13;
    query.queryFlag = 1;
    query.queryValue = CSF_AT(int, CSF_AT(uint8_t*, secondaryRef, 4), 4);

    // Hit result pair: index and sub-index.
    struct HitIndexPair
    {
        int index;     // a2
        int subIndex;  // v17
    };
    HitIndexPair hit{-1, -1};

    int result = RunSpatialHitQuery(hitManager, nullptr, queryPosition,
                                    reinterpret_cast<int>(&query),
                                    reinterpret_cast<unsigned int*>(&hit));

    if (static_cast<uint8_t>(result))
    {
        int hitRecordAddr = GetHitRecordByIndex(hitManager, nullptr, hit.index);

        // Hit entry: record->[+0x38] + 44 * subIndex (read after the lookup call).
        uint8_t* hitEntry = CSF_AT(uint8_t*, reinterpret_cast<uint8_t*>(hitRecordAddr), 56)
                          + 44 * hit.subIndex;
        int* hitRecordKey = reinterpret_cast<int*>(hitRecordAddr);

        // Output block is read here, after the record lookup and before the position lookup.
        uint8_t* outputBlockForHit = OutputBlockOf(outputRecord);
        float* hitPosition = reinterpret_cast<float*>(
            GetHitEntryPosition(hitManager, nullptr, reinterpret_cast<uint32_t*>(&hit)));
        CSF_AT(float, outputBlockForHit, 4) = hitPosition[0];
        CSF_AT(float, outputBlockForHit, 8) = hitPosition[1];
        CSF_AT(float, outputBlockForHit, 12) = hitPosition[2];

        result = *hitRecordKey;
        CSF_AT(int, OutputBlockOf(outputRecord), 16) = result;
        CSF_AT(int, OutputBlockOf(outputRecord), 20) = *reinterpret_cast<int*>(hitEntry);
    }
    return result;
}

HOOK(0x00463400, ResolveEntityHitPosition, "auto");
