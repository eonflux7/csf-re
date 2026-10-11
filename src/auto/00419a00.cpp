#include <cstdint>
#include <cstring>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Float constant used as the scale factor for the record's second float (fmul at 0x419A0C).
CSF_GLOBAL(float, g_WheelRecordScale, 0x0082DCE8);

// Grow helper: __thiscall with one stack arg (ret 4). Called with the sub-object at +0x34.
CSF_FUNC(void*, __fastcall, GrowRecordBuffer, 0x00418950, (void* self, void* edx, int a2));

// Appends a 32-byte wheel record {int a2, int a3, float a4, float a4*0.4f, 0, 0, 0, 0}
// to the dynamic array at +0x34 (buffer +0x34, capacity +0x38, count +0x3C, grow arg +0x40).
// Returns the new element count.
static int __fastcall C_0082e280_AppendWheelRecord(C_0082e280* self, void* /*edx*/,
                                                   int a2, int a3, float a4) {
    uint32_t record[8] = {};
    record[0] = static_cast<uint32_t>(a2);
    record[1] = static_cast<uint32_t>(a3);
    std::memcpy(&record[2], &a4, sizeof(float));
    float scaledValue = a4 * g_WheelRecordScale;
    std::memcpy(&record[5], &scaledValue, sizeof(float));

    // Grow the array when count >= capacity (unsigned compare, jb).
    if (!(CSF_AT(uint32_t, self, 0x3C) < CSF_AT(uint32_t, self, 0x38))) {
        GrowRecordBuffer(reinterpret_cast<uint8_t*>(self) + 0x34, nullptr,
                         static_cast<int>(CSF_AT(uint32_t, self, 0x40)));
    }

    // Re-read count and buffer after the possible call.
    uint32_t oldCount = CSF_AT(uint32_t, self, 0x3C);
    uint8_t* buffer = reinterpret_cast<uint8_t*>(self->field_0034);
    uint8_t* slot = buffer + static_cast<size_t>(oldCount) * 32;

    uint32_t newCount = oldCount + 1;
    CSF_AT(uint32_t, self, 0x3C) = newCount;
    std::memcpy(slot, record, 0x20);
    return static_cast<int>(newCount);
}

HOOK(0x00419A00, C_0082e280_AppendWheelRecord, "auto");
