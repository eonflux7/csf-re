#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, BuildRulesBlock, 0x0054F680, (void* self, void* edx, uint32_t* a1));
CSF_FUNC(int, __cdecl, GetGlobal8B0688, 0x00550CC0, ());
CSF_FUNC(int, __cdecl, GetValue_8B08F8, 0x0055A9C0, ());
CSF_FUNC(int, __fastcall, CopyRulesBlockAndApply, 0x0055A9F0, (int* self, void* , const void* sourceBlock));
CSF_FUNC(int, __fastcall, MpRules_CopyBlockAndApply, 0x0055AA20, (int* self, void* , const void* sourceBlock));
CSF_FUNC(void*, __cdecl, CoreFiles_SetFileOption, 0x00698600, (char locked));
CSF_FUNC(int, __cdecl, GetGlobalDword_73C020, 0x0073C020, ());
CSF_FUNC(int, __cdecl, InvokeMethodOnFetchedObject, 0x0073C520, ());
CSF_FUNC(int, __stdcall, StoreArgsIntoFetchedSlotAndProcess, 0x0073CC40, (int firstValue, int secondValue));
CSF_FUNC(int, __fastcall, GetSelectionValue, 0x00744580, (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, C_749b20_TakeByteFlag, 0x00749B20, (void* self, void*));

CSF_GLOBAL(std::uint8_t, g_OnlineMatchFlag, 0x008B068C);

static char __fastcall S_0054ecf0_ProcessPendingRules(S_0054ecf0* self, void* /*edx*/)
{
    const uint32_t initialObject = self->field_0004;
    if (!initialObject || initialObject == 216)
        return static_cast<char>(initialObject);

    void* object = reinterpret_cast<void*>(initialObject);
    const bool busy =
        reinterpret_cast<bool(__fastcall*)(void*, void*)>(
            CSF_AT(void**, object, 0)[8 / 4])(object, nullptr);

    if (!busy)
    {
        uint8_t pending = 1;
        if (!self->field_1098)
        {
            const uint32_t currentObject = self->field_0004;
            const uint32_t owner = currentObject ? currentObject - 216 : 0;
            if (!C_749b20_TakeByteFlag(
                    reinterpret_cast<void*>(owner + 68), nullptr))
                pending = 0;
        }
        self->field_1098 = pending;
    }

    const char pending = static_cast<char>(self->field_1098);
    if (!pending)
        return pending;

    const uint32_t currentObject = self->field_0004;
    const uint32_t owner = currentObject ? currentObject - 216 : 0;
    if (GetSelectionValue(reinterpret_cast<void*>(owner + 68), nullptr, 2))
    {
        const uint32_t selectedObject = self->field_0004;
        const uint32_t selectedOwner = selectedObject ? selectedObject - 216 : 0;
        const int secondValue =
            GetSelectionValue(reinterpret_cast<void*>(selectedOwner + 68), nullptr, 1);
        const int firstValue =
            GetSelectionValue(reinterpret_cast<void*>(selectedOwner + 68), nullptr, 0);
        GetGlobalDword_73C020();
        StoreArgsIntoFetchedSlotAndProcess(firstValue, secondValue);
    }
    else
    {
        GetGlobalDword_73C020();
        InvokeMethodOnFetchedObject();
    }

    alignas(8) uint8_t rulesBytes[0x94];
    BuildRulesBlock(self, nullptr, reinterpret_cast<uint32_t*>(rulesBytes));
    CoreFiles_SetFileOption(0);
    GetGlobal8B0688();

    if (g_OnlineMatchFlag)
    {
        int* rules = reinterpret_cast<int*>(GetValue_8B08F8());
        MpRules_CopyBlockAndApply(rules, nullptr, rulesBytes);
    }
    else
    {
        int* rules = reinterpret_cast<int*>(GetValue_8B08F8());
        CopyRulesBlockAndApply(rules, nullptr, rulesBytes);
    }

    void* result = CoreFiles_SetFileOption(1);
    self->field_1098 = 0;
    return static_cast<char>(reinterpret_cast<std::uintptr_t>(result));
}

HOOK(0x00550490, S_0054ecf0_ProcessPendingRules, "auto");
