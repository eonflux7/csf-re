#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, S_00692b80_FindIndexOfValue, 0x00462A80, (S_00692b80* self, void* , int value));
CSF_FUNC(int, __fastcall, SceneFallbackLookup, 0x004686A0, (S_004686a0* self, void* , int a2));
CSF_FUNC(int, __fastcall, MissionScript_GetExpressionRecordCount, 0x00471690, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(int, __fastcall, C_0086c288_FindIndexOfValue, 0x0075CE20, (C_0086c288* self, void* , int value));
CSF_FUNC(int, __fastcall, C_0083e0d8_GetRecordAddress, 0x0075CF00, (C_0083e0d8* self, void* , int recordIndex));
CSF_FUNC(int, __fastcall, InputBindings_FindEntryIndexByKey, 0x007686E0, (void* self, void* , int key));
CSF_FUNC(int, __fastcall, AiAttackTasks_FindSceneEntryIndex, 0x00770610, (void* self, void* edx, unsigned int a2));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

// The packet also classifies the direct SceneFallbackLookup call as a vtable reference.
[[maybe_unused]] static const auto sceneLookupVtable = CSF_VTBL(0x004686A0);

static char __fastcall MissionScript_EvaluateEmbeddedExpression(void* self, void* /*edx*/)
{
    void* expression = CSF_AT(void*, self, 0x4);
    if (!expression)
        return 0;
    if (CSF_AT(int, self, 0x8) != -1)
        return 1;

    switch (CSF_AT(std::uint8_t, expression, 0x0))
    {
    case 4:
    case 0xE:
    case 0x12:
    case 0x25:
    case 0x27:
    case 0x28:
        return 1;

    case 5:
    {
        void* context = reinterpret_cast<void*>(GetValue8B0554());
        auto* records = CSF_AT(C_0083e0d8*, context, 0x1FD8);
        int index = C_0086c288_FindIndexOfValue(
            reinterpret_cast<C_0086c288*>(records), nullptr,
            CSF_AT(int, CSF_AT(void*, self, 0x4), 0x10));
        if (index == -1)
            return 0;

        int value = CSF_AT(int, CSF_AT(void*, self, 0x4), 0x14);
        auto* record = reinterpret_cast<S_00692b80*>(
            C_0083e0d8_GetRecordAddress(records, nullptr, index));
        return S_00692b80_FindIndexOfValue(record, nullptr, value) != -1;
    }

    case 6:
    case 7:
        return SceneFallbackLookup(
            CSF_AT(S_004686a0*, g_pCurrentScene, 0xBC), nullptr,
            CSF_AT(int, expression, 0x4)) != 0;

    case 0xA:
    {
        void* context = reinterpret_cast<void*>(GetValue8B0554());
        unsigned int value =
            CSF_AT(unsigned int, CSF_AT(void*, self, 0x4), 0x4);
        return AiAttackTasks_FindSceneEntryIndex(
            CSF_AT(void*, context, 0x1FDC), nullptr, value) != -1;
    }

    case 0xC:
    {
        void* context = reinterpret_cast<void*>(GetValue8B0554());
        int value = CSF_AT(int, CSF_AT(void*, self, 0x4), 0x4);
        return C_0086c288_FindIndexOfValue(
            CSF_AT(C_0086c288*, context, 0x1FD8), nullptr, value) != -1;
    }

    case 0xD:
    {
        void* context = reinterpret_cast<void*>(GetValue8B0554());
        return InputBindings_FindEntryIndexByKey(
            CSF_AT(void*, context, 0x1F90), nullptr,
            CSF_AT(int, expression, 0x4)) != -1;
    }

    case 0x11:
    {
        auto* records = reinterpret_cast<std::uint8_t*>(GetValue8B0554()) + 20;
        std::uint32_t count = static_cast<std::uint32_t>(
            MissionScript_GetExpressionRecordCount(records, nullptr));
        bool found = false;
        std::uint32_t index = 0;
        std::uint32_t offset = 0;
        while (index < count && !found)
        {
            auto* entries = CSF_AT(std::uint8_t*, records, 0x4);
            found = CSF_AT(int, entries + offset, 0x1C) ==
                    CSF_AT(int, CSF_AT(void*, self, 0x4), 0x4);
            ++index;
            offset += 52;
        }
        return found;
    }

    case 0x18:
        return CSF_AT(int, expression, 0x4) != -1;

    case 0x20:
    {
        void* context = reinterpret_cast<void*>(GetValue8B0554());
        void* lookup = CSF_AT(void*, context, 0x1FC8);
        int value = CSF_AT(int, CSF_AT(void*, self, 0x4), 0x4);
        return reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, lookup, 0x0)[4])(lookup, nullptr, value) != -1;
    }

    case 0x24:
    {
        void* context = reinterpret_cast<void*>(GetValue8B0554());
        void* currentExpression = CSF_AT(void*, self, 0x4);
        void* lookup = CSF_AT(void*, context, 0x1FC4);
        int value = CSF_AT(int, currentExpression, 0x4);
        return reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, lookup, 0x0)[2])(lookup, nullptr, value) != -1;
    }

    case 0x26:
    {
        void* context = reinterpret_cast<void*>(GetValue8B0554());
        void* currentExpression = CSF_AT(void*, self, 0x4);
        void* lookup = CSF_AT(void*, context, 0x1FD0);
        int value = CSF_AT(int, currentExpression, 0x4);
        return reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, lookup, 0x0)[3])(lookup, nullptr, value) != -1;
    }

    case 0x29:
    {
        void* context = reinterpret_cast<void*>(GetValue8B0554());
        void* lookup = CSF_AT(void*, context, 0x1FCC);
        int value = CSF_AT(int, CSF_AT(void*, self, 0x4), 0x4);
        return reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, lookup, 0x0)[4])(lookup, nullptr, value) != -1;
    }

    case 0x2F:
    {
        void* context = reinterpret_cast<void*>(GetValue8B0554());
        void* lookup = CSF_AT(void*, context, 0x1FC0);
        int value = CSF_AT(int, CSF_AT(void*, self, 0x4), 0x4);
        return reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, lookup, 0x0)[3])(lookup, nullptr, value) != -1;
    }

    default:
        return 1;
    }
}

HOOK(0x00471F00, MissionScript_EvaluateEmbeddedExpression, "auto");
