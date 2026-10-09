#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(float, g_flt_0082E25C, 0x0082E25C);

CSF_FUNC(int, __cdecl, GetGlobalValue_4FBDE0, 0x004FBDE0, ());
CSF_FUNC(void*, __fastcall, GetGlobalSubObject_4FC0A0, 0x004FC0A0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SubmitEventRecord, 0x00443000, (void* self, void* edx, int a2));

struct TimedVectorRecord {
    int32_t reserved00;
    int32_t objectId;
    int32_t reserved08;
    uint8_t flags;
    int32_t timeMs;
    uint32_t packedVector;
    float x;
    float y;
    float z;
};
static_assert(sizeof(TimedVectorRecord) == 0x24);

static int __stdcall SubmitTimedVectorRecord(int /*a1*/, int /*a2*/, uint32_t* a3)
{
    TimedVectorRecord record;
    record.objectId = 0;
    record.reserved00 = 0;
    record.reserved08 = 0;

    uint8_t* timeHolder = reinterpret_cast<uint8_t*>(a3[0]);
    uint8_t* idBlock = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, timeHolder, 0x4));
    record.objectId = CSF_AT(int32_t, idBlock, 0x4);

    uint8_t* vectorHolder = reinterpret_cast<uint8_t*>(a3[2]);
    uint8_t* vectorData =
        reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, vectorHolder, 0x4)) + 0x4;
    record.x = CSF_AT(float, vectorData, 0x0);
    float vectorY = CSF_AT(float, vectorData, 0x4);
    float vectorZ = CSF_AT(float, vectorData, 0x8);
    record.y = vectorY;

    uint8_t* timeBlock = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, timeHolder, 0x4));
    float seconds = CSF_AT(float, timeBlock, 0x4);
    record.z = vectorZ;
    record.timeMs = static_cast<int32_t>(
        static_cast<int64_t>(seconds * g_flt_0082E25C));

    uint32_t packed = static_cast<uint32_t>(static_cast<int64_t>(record.x)) << 8;
    packed |= static_cast<uint32_t>(static_cast<int64_t>(record.y));
    packed <<= 8;
    packed |= static_cast<uint32_t>(static_cast<int64_t>(record.z));
    record.packedVector = packed;

    uint8_t* flagsHolder = reinterpret_cast<uint8_t*>(a3[1]);
    uint8_t* flagsBlock = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, flagsHolder, 0x4));
    record.flags = CSF_AT(uint8_t, flagsBlock, 0x1);

    void* globalValue = reinterpret_cast<void*>(GetGlobalValue_4FBDE0());
    void* globalSubObject = GetGlobalSubObject_4FC0A0(globalValue, nullptr);
    return SubmitEventRecord(globalSubObject, nullptr, reinterpret_cast<int>(&record));
}

HOOK(0x0045B630, SubmitTimedVectorRecord, "auto");
