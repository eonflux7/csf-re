#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringRange_429B60, 0x00429B60, (void* self, void* edx, char* text, unsigned int length));
CSF_FUNC(std::uint32_t, __fastcall, SceneLoader_DestroyMenuBuffer, 0x004FF3C0, (std::uint32_t* self, void*));
CSF_FUNC(int, __fastcall, MenuBuffer_Init, 0x00501870, (uint8_t* self, void*));
CSF_FUNC(int, __cdecl, GetGlobalValue_8B056C, 0x00506BD0, ());
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __fastcall, GameBddLoader_ShowSaveGameMessage, 0x0050F860, (void* self, void* , int text, int messageAction, int configuration));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(void*, __cdecl, InitStringFromLiteral, 0x00692720, (void* returnSlot, const char* text, int count));

static void* __fastcall CMenuScenarioSelect_ShowDifficultySelection(
    CMenuScenarioSelect* self, void* /*edx*/)
{
    // Keep the configuration and its string members contiguous as on the
    // original stack; callees may access beyond the individually named parts.
    alignas(8) uint8_t frameBytes[0xD0];
    uint8_t* configuration = frameBytes;
    uint8_t* titleString = frameBytes + 0xB4;

    CSF_AT(std::uint32_t, titleString, 0x18) = 7;
    CSF_AT(std::uint32_t, titleString, 0x14) = 0;
    CSF_AT(std::uint16_t, titleString, 0x04) = 0;
    InitStringFromLiteral(titleString, CSF_STR(0x00841630, "DIF0"), 1);

    MenuBuffer_Init(configuration, nullptr);
    CSF_AT(std::uint32_t, configuration, 0x00) = 1;
    CSF_AT(std::uint32_t, configuration, 0x04) = 1;

    void* state = reinterpret_cast<void*>(GetGlobalState_50D190());
    switch (CSF_AT(std::uint32_t, state, 0x84))
    {
    case 0:
        CSF_AT(int, configuration, 0x08) = 64;
        break;
    case 1:
        CSF_AT(int, configuration, 0x08) = 128;
        break;
    case 2:
        CSF_AT(int, configuration, 0x08) = 256;
        break;
    case 3:
        CSF_AT(int, configuration, 0x08) = 512;
        break;
    default:
        // The original leaves this stack value uninitialized.
        CSF_AT(int, configuration, 0x08) = 0;
        break;
    }

    AssignStringRange_429B60(
        frameBytes + 0x1C, nullptr,
        const_cast<char*>(CSF_STR(0x00841628, "DIF2")), 4);
    AssignStringRange_429B60(
        frameBytes + 0x38, nullptr,
        const_cast<char*>(CSF_STR(0x00841620, "DIF1")), 4);
    AssignStringRange_429B60(
        frameBytes + 0x54, nullptr,
        const_cast<char*>(CSF_STR(0x00841618, "DIF3")), 4);
    AssignStringRange_429B60(
        frameBytes + 0x70, nullptr,
        const_cast<char*>(CSF_STR(0x00841610, "DIF4")), 4);

    void* globalValue = reinterpret_cast<void*>(GetGlobalValue_8B056C());
    void* manager = CSF_AT(void*, globalValue, 0x08);
    void* context =
        ((void* (__fastcall*)(void*, void*))
            CSF_AT(void**, manager, 0)[0x24 / 4])(manager, nullptr);

    void* title = CSF_AT(std::uint32_t, titleString, 0x18) < 8
        ? static_cast<void*>(titleString + 0x04)
        : CSF_AT(void*, titleString, 0x04);
    int messageAction = CSF_AT(std::uint8_t, context, 0x44) != 0
        ? 3008 : 2496;

    void* loader = reinterpret_cast<void*>(GetGlobalState_50D190());
    GameBddLoader_ShowSaveGameMessage(
        loader, nullptr, reinterpret_cast<int>(title),
        messageAction, reinterpret_cast<int>(configuration));

    self->field_0038 = 2;
    void* result = reinterpret_cast<void*>(SceneLoader_DestroyMenuBuffer(
        reinterpret_cast<std::uint32_t*>(configuration), nullptr));

    if (CSF_AT(std::uint32_t, titleString, 0x18) >= 8)
    {
        result = CSF_AT(void*, titleString, 0x04);
        if (result)
            return reinterpret_cast<void*>(Mem_FreeDebug(result));
    }
    return result;
}

HOOK(0x0052E050, CMenuScenarioSelect_ShowDifficultySelection, "auto");
