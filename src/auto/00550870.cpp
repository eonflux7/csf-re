#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, S_0054ecf0_PrepareRules, 0x0054EFC0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, S_0054ecf0_UpdateRules, 0x0054F120, (void* self, void* edx));
CSF_FUNC(int, __fastcall, S_0054ecf0_ExportRulesBlock, 0x0054F680, (void* self, void* edx, uint32_t* a1));
CSF_FUNC(int, __cdecl, GetGlobal8B0688, 0x00550CC0, ());
CSF_FUNC(int, __cdecl, GetValue_8B08F8, 0x0055A9C0, ());
CSF_FUNC(int, __fastcall, CopyRulesBlockAndApply, 0x0055A9F0, (int* self, void* , const void* sourceBlock));
CSF_FUNC(int, __fastcall, MpRules_CopyBlockAndApply, 0x0055AA20, (int* self, void* , const void* sourceBlock));
CSF_FUNC(void*, __cdecl, CoreFiles_SetFileOption, 0x00698600, (char locked));
CSF_FUNC(char, __fastcall, C_749b20_TakeByteFlag, 0x00749B20, (void* self, void*));

CSF_GLOBAL(std::uint8_t, g_OnlineMatchFlag, 0x008B068C);

static char __fastcall S_0054ecf0_ApplyPendingRules(S_0054ecf0* self, void* /*edx*/)
{
    const uint32_t ownerAddress = self->field_0004;
    char result = static_cast<char>(ownerAddress);

    if (ownerAddress && ownerAddress != 216)
    {
        void* owner = reinterpret_cast<void*>(ownerAddress);
        const bool busy =
            (reinterpret_cast<bool(__fastcall*)(void*, void*)>(
                CSF_AT(void**, owner, 0)[8 / 4]))(owner, nullptr);

        if (!busy)
        {
            const uint32_t currentOwner = self->field_0004;
            const uint32_t baseAddress = currentOwner ? currentOwner - 216 : 0;
            const char pending = C_749b20_TakeByteFlag(
                reinterpret_cast<void*>(baseAddress + 88), nullptr);
            self->field_109a |= static_cast<std::uint8_t>(pending);
        }

        result = static_cast<char>(self->field_109a);
        if (result)
        {
            alignas(8) std::uint8_t rulesBlockBytes[0x94];

            S_0054ecf0_PrepareRules(self, nullptr);
            S_0054ecf0_UpdateRules(self, nullptr);
            S_0054ecf0_ExportRulesBlock(
                self, nullptr, reinterpret_cast<uint32_t*>(rulesBlockBytes));

            CoreFiles_SetFileOption(0);
            GetGlobal8B0688();

            if (g_OnlineMatchFlag)
            {
                int* rules = reinterpret_cast<int*>(GetValue_8B08F8());
                MpRules_CopyBlockAndApply(rules, nullptr, rulesBlockBytes);
            }
            else
            {
                int* rules = reinterpret_cast<int*>(GetValue_8B08F8());
                CopyRulesBlockAndApply(rules, nullptr, rulesBlockBytes);
            }

            result = static_cast<char>(
                reinterpret_cast<std::uintptr_t>(CoreFiles_SetFileOption(1)));
            self->field_109a = 0;
        }
    }

    return result;
}

HOOK(0x00550870, S_0054ecf0_ApplyPendingRules, "auto");
