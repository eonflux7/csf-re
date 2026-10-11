#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, Vehicle_GrowPrimaryRecords, 0x0040CDB0, (void* self, void* edx, int a2));
CSF_FUNC(void*, __fastcall, Vehicle_GrowSecondaryRecords, 0x004189D0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, Vehicle_InitializeBase, 0x00418A50, (void* self, void* edx, char a2));
CSF_FUNC(int, __fastcall, Vehicle_AddLeftEntry, 0x00419A00, (void* self, void* edx, int a2, int a3, float a4));
CSF_FUNC(int, __fastcall, Vehicle_AddRightEntry, 0x00419A80, (void* self, void* edx, int a2, int a3, float a4));

static int* __fastcall C_0082e3b8_Initialize(C_0082e3b8* self, void* /*edx*/, char initializationMode)
{
    Vehicle_InitializeBase(self, nullptr, initializationMode);
    *reinterpret_cast<void***>(self) = CSF_VTBL(0x0082E3B8);
    self->field_0090 = 0;
    self->field_0094 = 0;
    self->field_0098 = 0;

    Vehicle_AddLeftEntry(self, nullptr, 0, 0, 10.0f);
    Vehicle_AddRightEntry(self, nullptr, 9, 0, 10.0f);
    Vehicle_AddLeftEntry(self, nullptr, 1, 1090519040, 0.0f);
    Vehicle_AddRightEntry(self, nullptr, 10, 1090519040, 0.0f);

    int rightIndex = 11;
    int remaining = 6;
    do {
        Vehicle_AddLeftEntry(self, nullptr, rightIndex - 9, 1090519040, 20.0f);
        Vehicle_AddRightEntry(self, nullptr, rightIndex, 1090519040, 20.0f);
        ++rightIndex;
        --remaining;
    } while (remaining);

    Vehicle_AddLeftEntry(self, nullptr, 8, 1090519040, 10.0f);
    Vehicle_AddRightEntry(self, nullptr, 17, 1090519040, 10.0f);

    if (self->field_005c >= self->field_0058) {
        Vehicle_GrowSecondaryRecords(
            reinterpret_cast<std::uint8_t*>(self) + 0x54, nullptr,
            CSF_AT(int, self, 0x60));
    }

    const std::uint32_t secondaryIndex = self->field_005c;
    const std::uint32_t secondaryStorage = self->field_0054;
    self->field_005c = secondaryIndex + 1;
    void* secondaryRecord =
        reinterpret_cast<void*>(secondaryStorage + 24 * secondaryIndex);

    CSF_AT(std::uint32_t, secondaryRecord, 0x00) = 18;
    CSF_AT(std::uint32_t, secondaryRecord, 0x04) = 20;
    CSF_AT(std::uint32_t, secondaryRecord, 0x08) = 19;
    CSF_AT(std::uint32_t, secondaryRecord, 0x0C) = 21;
    CSF_AT(std::uint32_t, secondaryRecord, 0x10) = 1106247680;
    CSF_AT(std::uint32_t, secondaryRecord, 0x14) = 1092616192;

    if (!self->field_0014) {
        Vehicle_GrowPrimaryRecords(
            reinterpret_cast<std::uint8_t*>(self) + 0x10, nullptr, 1);
    }
    if (CSF_AT(std::uint32_t, self, 0x18) >= self->field_0014) {
        Vehicle_GrowPrimaryRecords(
            reinterpret_cast<std::uint8_t*>(self) + 0x10, nullptr,
            CSF_AT(int, self, 0x1C));
    }

    const std::uint32_t primaryIndex = CSF_AT(std::uint32_t, self, 0x18);
    const std::uint32_t primaryStorage = self->field_0010;
    CSF_AT(std::uint32_t, self, 0x18) = primaryIndex + 1;
    void* primaryRecord =
        reinterpret_cast<void*>(primaryStorage + 24 * primaryIndex);

    CSF_AT(std::uint32_t, primaryRecord, 0x00) = 1;
    CSF_AT(std::uint32_t, primaryRecord, 0x04) = 1100;
    CSF_AT(std::uint32_t, primaryRecord, 0x08) = 1200;
    CSF_AT(std::uint32_t, primaryRecord, 0x0C) = 0;
    CSF_AT(std::uint32_t, primaryRecord, 0x10) = 23;
    CSF_AT(std::uint32_t, primaryRecord, 0x14) = 24;

    self->field_00a4 = 0.0f;
    self->field_008c = 0;
    self->field_009c = 0;
    self->field_00a0 = 0;
    self->field_00ac = -1;
    return reinterpret_cast<int*>(self);
}

HOOK(0x0041CCE0, C_0082e3b8_Initialize, "auto");
