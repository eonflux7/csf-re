#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, C_0082dc88_SubmitPositionRecord, 0x0040D1F0,
         (void* self, void* edx, float* arg0));
CSF_FUNC(void*, __fastcall, C_0082dd78_DispatchReturnedObject, 0x0041E0C0,
         (void* self, void* edx));
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10,
         (std::uint32_t* matrix));

CSF_GLOBAL(float, g_WheelRecordScale, 0x0082DCE8);

static char __fastcall C_0082dc88_RecordScaledPosition(
    C_0082dc88* self, void* /*edx*/, float time)
{
    const char enabled = static_cast<char>(self->field_0039);
    if (!enabled)
        return enabled;

    void* owner = reinterpret_cast<void*>(self->field_0004);
    void* source =
        reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, owner, 0)[0x28 / 4])(owner, nullptr);
    void* transform =
        reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, source, 0)[0x18 / 4])(source, nullptr);

    reinterpret_cast<float (__fastcall*)(void*, void*)>(
        CSF_AT(void**, transform, 0)[0x28 / 4])(transform, nullptr);
    reinterpret_cast<float* (__fastcall*)(void*, void*)>(
        CSF_AT(void**, transform, 0)[0x10 / 4])(transform, nullptr);

    alignas(8) std::uint8_t recordBytes[0x40]{};
    float* record = reinterpret_cast<float*>(recordBytes);
    std::memset(record + 6, 0, 12);
    CSF_AT(std::uint32_t, recordBytes, 0x24) = 10;
    record[0] = time;

    void* dispatched = C_0082dd78_DispatchReturnedObject(self, nullptr);

    alignas(8) std::uint8_t matrixResultBytes[0xC];
    void* matrixResult =
        reinterpret_cast<void* (__fastcall*)(void*, void*, void*)>(
            CSF_AT(void**, transform, 0)[0x8C / 4])(
                transform, nullptr, matrixResultBytes);
    const float* direction = reinterpret_cast<const float*>(
        reinterpret_cast<std::uint8_t*>(
            CSF_AT(void*, matrixResult, 0)) + 0x10);

    void* dimensions =
        reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, dispatched, 0)[0x30 / 4])(dispatched, nullptr);
    const float scale = CSF_AT(float, dimensions, 0x4) * g_WheelRecordScale;

    record[13] = scale * direction[0];
    record[14] = scale * direction[1];
    record[15] = scale * direction[2];

    const float* position =
        reinterpret_cast<float* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, transform, 0)[0x10 / 4])(transform, nullptr);
    record[10] = record[13] + position[0];
    record[1] = record[10];
    record[11] = record[14] + position[1];
    record[2] = record[11];
    record[12] = record[15] + position[2];
    record[3] = record[12];

    if (CSF_AT(std::uint8_t, matrixResultBytes, 0x4))
        RwMatrixDestroy(CSF_AT(std::uint32_t*, matrixResultBytes, 0));

    return C_0082dc88_SubmitPositionRecord(self, nullptr, record);
}

HOOK(0x0040D480, C_0082dc88_RecordScaledPosition, "auto");
