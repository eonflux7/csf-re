#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, MpGameModes_SetIntegerRuleOption, 0x007440E0,
         (void* self, void* edx, int a2, int a3));
CSF_FUNC(int, __fastcall, MpGameModes_SetRuleOption, 0x00744150,
         (void* self, void* edx, int a2, int a3));
CSF_FUNC(int, __fastcall, MpGameModes_SetExtendedOption, 0x007443D0,
         (void* self, void* edx, int a2, int a3));
CSF_FUNC(int, __fastcall, MpGameModes_SetHeaderOption, 0x00744550,
         (void* self, void* edx, int a2, int a3));

static void* S_0054ecf0_GetModeOptionGroup(
    S_0054ecf0* self, std::uint32_t groupOffset)
{
    const std::uint32_t modeSubobject = self->field_0004;
    const std::uint32_t modeBase = modeSubobject ? modeSubobject - 216 : 0;
    return reinterpret_cast<void*>(modeBase + groupOffset);
}

static int __fastcall S_0054ecf0_ApplyGameModeOptions(
    S_0054ecf0* self, void* /*edx*/, std::int32_t* options)
{
    const std::uint32_t modeSubobject = self->field_0004;
    if (modeSubobject == 0 || modeSubobject == 216)
        return static_cast<int>(modeSubobject);

    MpGameModes_SetHeaderOption(
        S_0054ecf0_GetModeOptionGroup(self, 0x44), nullptr, 0, options[0]);
    MpGameModes_SetHeaderOption(
        S_0054ecf0_GetModeOptionGroup(self, 0x44), nullptr, 1, options[1]);
    MpGameModes_SetHeaderOption(
        S_0054ecf0_GetModeOptionGroup(self, 0x44), nullptr, 2, options[2]);

    MpGameModes_SetExtendedOption(
        S_0054ecf0_GetModeOptionGroup(self, 0xA8), nullptr, 3, options[3]);
    MpGameModes_SetExtendedOption(
        S_0054ecf0_GetModeOptionGroup(self, 0xA8), nullptr, 4, options[4]);
    MpGameModes_SetExtendedOption(
        S_0054ecf0_GetModeOptionGroup(self, 0xA8), nullptr, 5, options[5]);
    MpGameModes_SetExtendedOption(
        S_0054ecf0_GetModeOptionGroup(self, 0xA8), nullptr, 6, options[6]);
    MpGameModes_SetExtendedOption(
        S_0054ecf0_GetModeOptionGroup(self, 0xA8), nullptr, 7, options[7]);
    MpGameModes_SetExtendedOption(
        S_0054ecf0_GetModeOptionGroup(self, 0xA8), nullptr, 8, options[8]);
    MpGameModes_SetExtendedOption(
        S_0054ecf0_GetModeOptionGroup(self, 0xA8), nullptr, 9, options[9]);
    MpGameModes_SetExtendedOption(
        S_0054ecf0_GetModeOptionGroup(self, 0xA8), nullptr, 10, options[10]);
    MpGameModes_SetExtendedOption(
        S_0054ecf0_GetModeOptionGroup(self, 0xA8), nullptr, 11, options[11]);
    MpGameModes_SetExtendedOption(
        S_0054ecf0_GetModeOptionGroup(self, 0xA8), nullptr, 12, options[12]);

    MpGameModes_SetRuleOption(
        S_0054ecf0_GetModeOptionGroup(self, 0x58), nullptr, 13, options[13]);
    MpGameModes_SetRuleOption(
        S_0054ecf0_GetModeOptionGroup(self, 0x58), nullptr, 14, options[14]);
    MpGameModes_SetRuleOption(
        S_0054ecf0_GetModeOptionGroup(self, 0x58), nullptr, 15, options[15]);
    MpGameModes_SetRuleOption(
        S_0054ecf0_GetModeOptionGroup(self, 0x58), nullptr, 16, options[16]);
    MpGameModes_SetRuleOption(
        S_0054ecf0_GetModeOptionGroup(self, 0x58), nullptr, 17, options[17]);
    MpGameModes_SetRuleOption(
        S_0054ecf0_GetModeOptionGroup(self, 0x58), nullptr, 18, options[18]);
    MpGameModes_SetRuleOption(
        S_0054ecf0_GetModeOptionGroup(self, 0x58), nullptr, 19, options[19]);

    MpGameModes_SetIntegerRuleOption(
        S_0054ecf0_GetModeOptionGroup(self, 0x58), nullptr, 20, options[20]);
    MpGameModes_SetIntegerRuleOption(
        S_0054ecf0_GetModeOptionGroup(self, 0x58), nullptr, 21, options[21]);

    MpGameModes_SetRuleOption(
        S_0054ecf0_GetModeOptionGroup(self, 0x58), nullptr, 22, options[22]);
    MpGameModes_SetRuleOption(
        S_0054ecf0_GetModeOptionGroup(self, 0x58), nullptr, 23, options[23]);
    MpGameModes_SetRuleOption(
        S_0054ecf0_GetModeOptionGroup(self, 0x58), nullptr, 24, options[24]);
    MpGameModes_SetRuleOption(
        S_0054ecf0_GetModeOptionGroup(self, 0x58), nullptr, 25, options[25]);
    MpGameModes_SetRuleOption(
        S_0054ecf0_GetModeOptionGroup(self, 0x58), nullptr, 26, options[26]);
    MpGameModes_SetRuleOption(
        S_0054ecf0_GetModeOptionGroup(self, 0x58), nullptr, 27, options[27]);
    MpGameModes_SetRuleOption(
        S_0054ecf0_GetModeOptionGroup(self, 0x58), nullptr, 28, options[28]);

    MpGameModes_SetIntegerRuleOption(
        S_0054ecf0_GetModeOptionGroup(self, 0x58), nullptr, 29, options[29]);
    return MpGameModes_SetIntegerRuleOption(
        S_0054ecf0_GetModeOptionGroup(self, 0x58), nullptr, 30, options[30]);
}

HOOK(0x0054F280, S_0054ecf0_ApplyGameModeOptions, "auto");
