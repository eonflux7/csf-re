#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, C_0082dd78_SubmitWheelRecord, 0x0040E1C0,
         (void* self, void* edx, float* arg0));
CSF_FUNC(unsigned int, __fastcall, C_0082dd78_UpdateWheelState, 0x0040E510,
         (void* self, void* edx, float arg0));
CSF_FUNC(void*, __fastcall, C_0082dd78_DispatchReturnedObject, 0x0041E0C0,
         (void* self, void* edx));
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10,
         (std::uint32_t* matrix));

CSF_GLOBAL(float, g_WheelRecordScale, 0x0082DCE8);

static char __fastcall C_0082dd78_RecordWheelFrame(
    C_0082dd78* self, void* /*edx*/, float frameValue)
{
    const char enabled = static_cast<char>(self->field_0039);
    if (!enabled)
        return enabled;

    void* owner = reinterpret_cast<void*>(self->field_0004);
    void* intermediate =
        reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, owner, 0)[0x28 / 4])(owner, nullptr);
    void* wheel =
        reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, intermediate, 0)[0x18 / 4])(
                intermediate, nullptr);

    reinterpret_cast<float (__fastcall*)(void*, void*)>(
        CSF_AT(void**, wheel, 0)[0x28 / 4])(wheel, nullptr);
    reinterpret_cast<float* (__fastcall*)(void*, void*)>(
        CSF_AT(void**, wheel, 0)[0x10 / 4])(wheel, nullptr);

    alignas(8) std::uint8_t recordBytes[0x40]{};
    CSF_AT(std::uint32_t, recordBytes, 0x24) = 10;
    CSF_AT(float, recordBytes, 0x00) = frameValue;

    void* dispatched = C_0082dd78_DispatchReturnedObject(self, nullptr);

    alignas(8) std::uint8_t matrixResultBytes[0xC]{};
    void* matrixResult =
        reinterpret_cast<void* (__fastcall*)(void*, void*, void*)>(
            CSF_AT(void**, wheel, 0)[0x8C / 4])(
                wheel, nullptr, matrixResultBytes);
    auto* direction = reinterpret_cast<std::uint8_t*>(
        CSF_AT(void*, matrixResult, 0)) + 0x10;

    void* scaleData =
        reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, dispatched, 0)[0x30 / 4])(
                dispatched, nullptr);
    const float scale = CSF_AT(float, scaleData, 0x04) * g_WheelRecordScale;

    CSF_AT(float, recordBytes, 0x34) = scale * CSF_AT(float, direction, 0x00);
    CSF_AT(float, recordBytes, 0x38) = scale * CSF_AT(float, direction, 0x04);
    CSF_AT(float, recordBytes, 0x3C) = scale * CSF_AT(float, direction, 0x08);

    float* position =
        reinterpret_cast<float* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, wheel, 0)[0x10 / 4])(wheel, nullptr);

    CSF_AT(float, recordBytes, 0x28) =
        CSF_AT(float, recordBytes, 0x34) + position[0];
    CSF_AT(float, recordBytes, 0x04) = CSF_AT(float, recordBytes, 0x28);
    CSF_AT(float, recordBytes, 0x2C) =
        CSF_AT(float, recordBytes, 0x38) + position[1];
    CSF_AT(float, recordBytes, 0x08) = CSF_AT(float, recordBytes, 0x2C);
    CSF_AT(float, recordBytes, 0x30) =
        CSF_AT(float, recordBytes, 0x3C) + position[2];
    CSF_AT(float, recordBytes, 0x0C) = CSF_AT(float, recordBytes, 0x30);

    if (CSF_AT(std::uint8_t, matrixResultBytes, 0x04))
        RwMatrixDestroy(CSF_AT(std::uint32_t*, matrixResultBytes, 0x00));

    C_0082dd78_SubmitWheelRecord(
        self, nullptr, reinterpret_cast<float*>(recordBytes));
    return static_cast<char>(
        C_0082dd78_UpdateWheelState(self, nullptr, frameValue));
}

HOOK(0x0040E7D0, C_0082dd78_RecordWheelFrame, "auto");
