#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, MenuProgressInput_ResetSlotsAndUpdate, 0x0045BD10, (void* self, void*));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __fastcall, BddLoader_ApplyScratchToGlobalState, 0x0050E290, (void* self, void* , int a2));
CSF_FUNC(unsigned int, __fastcall, CMenuMissionInfo_SetNormalizedColor, 0x0053D260, (float* self, void* , uint32_t a2, uint32_t a3, uint32_t a4, uint32_t a5));

CSF_GLOBAL(uint8_t, g_byte_82E65C, 0x0082E65C);
CSF_GLOBAL(const char*, g_pszMenu_P_Help, 0x00899AA4);

static char __fastcall CMenuMissionInfo_UpdatePanels(
    CMenuMissionInfo* self, void* /*edx*/)
{
    const uint32_t state = self->field_00ac;
    if (state != 0)
    {
        if (state != 1)
            return static_cast<char>(state - 1);

        void* globalState = reinterpret_cast<void*>(GetGlobalState_50D190());
        void* panel = CSF_AT(void*, globalState, 0x14);
        void** panelVtable = CSF_AT(void**, panel, 0);

        int help = reinterpret_cast<int(__fastcall*)(void*, void*, const char*, int)>(
            panelVtable[0x5C / 4])(panel, nullptr, g_pszMenu_P_Help, 1);
        bool active = reinterpret_cast<bool(__fastcall*)(void*, void*, int)>(
            panelVtable[0x74 / 4])(panel, nullptr, help);

        if (!active)
            self->field_00ac = 0;
        return static_cast<char>(active);
    }

    alignas(8) uint8_t scratchBytes[0xE4]{};
    MenuProgressInput_ResetSlotsAndUpdate(scratchBytes, nullptr);

    if (self->field_00a7)
    {
        self->field_00a7 = 0;
        CSF_AT(const char*, scratchBytes, 0) = CSF_STR(0x00842CF0, "TAC0");
        CMenuMissionInfo_SetNormalizedColor(
            reinterpret_cast<float*>(scratchBytes + 0x08), nullptr,
            160, 142, 108, 255);

        CSF_AT(const char*, scratchBytes, 0x4C) = CSF_STR(0x00842CE8, "TAC2");
        CMenuMissionInfo_SetNormalizedColor(
            reinterpret_cast<float*>(scratchBytes + 0x54), nullptr,
            160, 142, 108, 255);
        CMenuMissionInfo_SetNormalizedColor(
            reinterpret_cast<float*>(scratchBytes + 0x64), nullptr,
            226, 167, 93, 255);
    }
    else
    {
        if (!self->field_00a8)
            return 0;

        self->field_00a8 = 0;
        CSF_AT(const char*, scratchBytes, 0) = CSF_STR(0x00842CF0, "TAC0");
        CMenuMissionInfo_SetNormalizedColor(
            reinterpret_cast<float*>(scratchBytes + 0x08), nullptr,
            160, 142, 108, 255);

        CSF_AT(const char*, scratchBytes, 0x4C) = CSF_STR(0x00842CE0, "TAC1");
        CMenuMissionInfo_SetNormalizedColor(
            reinterpret_cast<float*>(scratchBytes + 0x54), nullptr,
            160, 142, 108, 255);
        CMenuMissionInfo_SetNormalizedColor(
            reinterpret_cast<float*>(scratchBytes + 0x64), nullptr,
            226, 167, 93, 255);
    }

    CSF_AT(uint8_t*, scratchBytes, 0x98) = &g_byte_82E65C;
    void* globalState = reinterpret_cast<void*>(GetGlobalState_50D190());
    const int result = BddLoader_ApplyScratchToGlobalState(
        globalState, nullptr, reinterpret_cast<int>(scratchBytes));
    self->field_00ac = 1;
    return static_cast<char>(result);
}

HOOK(0x0053D910, CMenuMissionInfo_UpdatePanels, "auto");
