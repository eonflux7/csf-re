#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(char, __fastcall, ScriptCommands_DispatchCommand, 0x00465310,
         (void* self, void* edx, int result, int16_t* entry, C_0085e7b8* arguments));
CSF_FUNC(void*, __cdecl, GetGlobalBlock8AFDA0, 0x0046ED80, ());
CSF_FUNC(int, __stdcall, GetDwordField4, 0x0046ED90, (int a1));
CSF_FUNC(int, __fastcall, MissionScript_GetEntryArgument, 0x0046F540,
         (void* self, void* edx, int a2, int a3));
CSF_FUNC(void*, __fastcall, ProcessEntryPair, 0x00472B60, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, GetGlobalValue_768E70, 0x00768E70, ());
CSF_FUNC(int, __fastcall, MissionScript_GetCommandDefinition, 0x00769360,
         (void* self, void* edx, unsigned int a2));

static char __fastcall MissionScript_EvaluateEntry(
    void* self, void* /*edx*/, int16_t* entry, C_008313f8* result)
{
    if (entry[1] == -1)
    {
        void* source = CSF_AT(void*, entry, 0x4);
        uint32_t index = CSF_AT(uint32_t, source, 0x8);

        if (index != static_cast<uint32_t>(-1))
        {
            void* table = CSF_AT(void*, self, 0x20);
            if ((index & 0x800000) != 0)
            {
                void* sceneData = CSF_AT(void*, g_pCurrentScene, 0xBC);
                table = CSF_AT(void*, sceneData, 0x38);
                index &= ~0x800000u;
            }
            source = static_cast<uint8_t*>(CSF_AT(void*, table, 0x8)) + 12 * index;
        }

        ProcessEntryPair(result, nullptr, reinterpret_cast<int>(source));
        result->field_0008 =
            CSF_AT(uint32_t, CSF_AT(void*, entry, 0x4), 0x8);
        return 1;
    }

    const unsigned int command = static_cast<unsigned int>(*entry);
    void* registry = reinterpret_cast<void*>(GetGlobalValue_768E70());
    void* definition = reinterpret_cast<void*>(
        MissionScript_GetCommandDefinition(registry, nullptr, command));

    if (CSF_AT(int, definition, 0x0) == 4)
        return 1;

    const uint32_t argumentCount = CSF_AT(uint32_t, definition, 0x18);
    uint32_t arguments[0x20 / sizeof(uint32_t)] = {};
    arguments[5] = argumentCount;

    for (uint32_t i = 0; i < argumentCount; ++i)
    {
        int argumentEntry = MissionScript_GetEntryArgument(
            CSF_AT(void*, self, 0x10), nullptr,
            reinterpret_cast<int>(entry), static_cast<int>(i));

        GetGlobalBlock8AFDA0();
        int argumentResult = GetDwordField4(argumentEntry);

        MissionScript_EvaluateEntry(
            self, nullptr, reinterpret_cast<int16_t*>(argumentEntry),
            reinterpret_cast<C_008313f8*>(argumentResult));
        arguments[i] = static_cast<uint32_t>(argumentResult);
    }

    return ScriptCommands_DispatchCommand(
        self, nullptr, reinterpret_cast<int>(result), entry,
        reinterpret_cast<C_0085e7b8*>(arguments));
}

HOOK(0x00467BC0, MissionScript_EvaluateEntry, "auto");
