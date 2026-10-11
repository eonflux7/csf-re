#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, Vehicle_GrowActionList, 0x0040CDB0,
         (void* self, void* edx, int a2));
CSF_FUNC(void*, __fastcall, Vehicle_GrowConfigurationList, 0x004189D0,
         (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, Vehicle_InitializeBase, 0x00418A50,
         (void* self, void* edx, char a2));
CSF_FUNC(int, __fastcall, Vehicle_AddFirstConfiguration, 0x00419A00,
         (void* self, void* edx, int a2, int a3, float a4));
CSF_FUNC(int, __fastcall, Vehicle_AddSecondConfiguration, 0x00419A80,
         (void* self, void* edx, int a2, int a3, float a4));

static uint32_t* __fastcall C_0082e350_Initialize(
    C_0082e350* self, void* /*edx*/, char initializationFlag)
{
    Vehicle_InitializeBase(self, nullptr, initializationFlag);
    *reinterpret_cast<void***>(self) = CSF_VTBL(0x0082E350);

    Vehicle_AddFirstConfiguration(self, nullptr, 0, 0, 10.0f);
    Vehicle_AddSecondConfiguration(self, nullptr, 0, 0, 10.0f);
    Vehicle_AddFirstConfiguration(self, nullptr, 1, 1090519040, 0.0f);
    Vehicle_AddSecondConfiguration(self, nullptr, 7, 1090519040, 0.0f);

    int configurationIndex = 8;
    int remaining = 5;
    do
    {
        Vehicle_AddFirstConfiguration(
            self, nullptr, configurationIndex - 6, 1090519040, 20.0f);
        Vehicle_AddSecondConfiguration(
            self, nullptr, configurationIndex, 1090519040, 20.0f);
        ++configurationIndex;
        --remaining;
    } while (remaining);

    if (self->field_005c >= self->field_0058)
    {
        Vehicle_GrowConfigurationList(
            reinterpret_cast<uint8_t*>(self) + 0x54, nullptr,
            static_cast<int>(CSF_AT(uint32_t, self, 0x60)));
    }

    uint32_t configurationCount = self->field_005c;
    auto* configuration = reinterpret_cast<uint32_t*>(
        self->field_0054 + 24 * configurationCount);
    self->field_005c = configurationCount + 1;
    configuration[0] = 13;
    configuration[1] = 15;
    configuration[2] = 14;
    configuration[3] = 16;
    configuration[4] = static_cast<uint32_t>(-1032847360);
    configuration[5] = 1092616192;

    void* actionList = reinterpret_cast<uint8_t*>(self) + 0x10;
    uint32_t actionCapacity = self->field_0014;
    if (actionCapacity < 2)
    {
        Vehicle_GrowActionList(
            actionList, nullptr, static_cast<int>(2 - actionCapacity));
    }

    if (CSF_AT(uint32_t, self, 0x18) >= self->field_0014)
    {
        Vehicle_GrowActionList(
            actionList, nullptr,
            static_cast<int>(CSF_AT(uint32_t, self, 0x1C)));
    }

    uint32_t actionCount = CSF_AT(uint32_t, self, 0x18);
    auto* action = reinterpret_cast<uint32_t*>(
        self->field_0010 + 24 * actionCount);
    CSF_AT(uint32_t, self, 0x18) = actionCount + 1;
    action[0] = 0;
    action[1] = 1100;
    action[2] = 0;
    action[3] = 0;
    action[4] = static_cast<uint32_t>(-1);
    action[5] = static_cast<uint32_t>(-1);

    if (CSF_AT(uint32_t, self, 0x18) >= self->field_0014)
    {
        Vehicle_GrowActionList(
            actionList, nullptr,
            static_cast<int>(CSF_AT(uint32_t, self, 0x1C)));
    }

    actionCount = CSF_AT(uint32_t, self, 0x18);
    action = reinterpret_cast<uint32_t*>(
        self->field_0010 + 24 * actionCount);
    CSF_AT(uint32_t, self, 0x18) = actionCount + 1;
    action[0] = 0;
    action[1] = 1110;
    action[2] = 0;
    action[3] = 0;
    action[4] = static_cast<uint32_t>(-1);
    action[5] = static_cast<uint32_t>(-1);

    self->field_008c = 0.0f;
    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x0041C270, C_0082e350_Initialize, "auto");
