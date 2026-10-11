#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(void*, __fastcall, AssignStringRange_488830, 0x00488830, (void* self, void* edx, void* source, std::uint32_t position, std::uint32_t count));
CSF_FUNC(S_004f21d0*, __fastcall, S_004f21d0_ResetAndAssign, 0x00488BE0, (S_004f21d0* self, void* , void* a2));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __cdecl, MenuMessageBox_ResolveEntryByKey, 0x00692440, (int code, int flag));
CSF_FUNC(void*, __cdecl, FormatCountTextBuffer, 0x00696A80, (void* outString));
CSF_FUNC(void*, __fastcall, ConstructWideString, 0x00696F50, (void* self, void* edx, wchar_t* String));
CSF_FUNC(int, __fastcall, FormatterSetPattern, 0x00697100, (void* self, void* edx, const char* pattern));
CSF_FUNC(int, __cdecl, GetGlobalDword_73C020, 0x0073C020, ());
CSF_FUNC(int, __fastcall, MpSpawnPoints_FindPlayerIndex, 0x0074A190, (void* self, void* edx, int a2));
CSF_FUNC(char*, __fastcall, MpSpawnPoints_GetPlayerEntry, 0x0074A520, (void* self, void* edx, unsigned int a2));

CSF_GLOBAL(int, g_NumberFormattingMode, 0x0085E684);

// The formatter consumes a 0x1C-byte string passed by value, followed
// by the substitution arguments. Keep this ABI carrier trivially copyable.
struct MpSpawnPoints_FormatArgument
{
    std::uint32_t words[7];
};

static S_004f21d0* __stdcall MpSpawnPoints_FormatLocalizedMessage(
    S_004f21d0* output, int messageKey, int firstPlayerId,
    int secondPlayerId, unsigned int count)
{
    // sized by the original's frame (0x74 bytes): struct sizes in auto_classes.h are lower bounds
    alignas(8) uint8_t resultBytes[0x74] = {};
    S_004f21d0& result = *reinterpret_cast<S_004f21d0*>(resultBytes);
    // sized by the original's frame (0x74 bytes): struct sizes in auto_classes.h are lower bounds
    alignas(8) uint8_t firstNameBytes[0x74] = {};
    S_004f21d0& firstName = *reinterpret_cast<S_004f21d0*>(firstNameBytes);
    // sized by the original's frame (0x74 bytes): struct sizes in auto_classes.h are lower bounds
    alignas(8) uint8_t secondNameBytes[0x74] = {};
    S_004f21d0& secondName = *reinterpret_cast<S_004f21d0*>(secondNameBytes);
    // sized by the original's frame (0x74 bytes): struct sizes in auto_classes.h are lower bounds
    alignas(8) uint8_t localizedPatternBytes[0x74] = {};
    S_004f21d0& localizedPattern = *reinterpret_cast<S_004f21d0*>(localizedPatternBytes);

    result.field_0018 = 7;
    result.field_0014 = 0;
    reinterpret_cast<std::uint16_t*>(&result.field_0004)[0] = 0;

    FormatterSetPattern(&firstName, nullptr, CSF_STR(0x008446B0, "SinNombre"));
    FormatterSetPattern(&secondName, nullptr, CSF_STR(0x008446B0, "SinNombre"));

    const bool hasCount = count != 0xFFFFFFFF;
    bool hasFirstName = false;
    bool hasSecondName = false;

    if (firstPlayerId != -1)
    {
        void* manager = reinterpret_cast<void*>(GetGlobalDword_73C020());
        int index = MpSpawnPoints_FindPlayerIndex(
            CSF_AT(void*, manager, 0xC8), nullptr, firstPlayerId);
        if (index != -1)
        {
            manager = reinterpret_cast<void*>(GetGlobalDword_73C020());
            char* entry = MpSpawnPoints_GetPlayerEntry(
                CSF_AT(void*, manager, 0xC8), nullptr,
                static_cast<unsigned int>(index));
            if (entry)
            {
                AssignStringRange_488830(
                    &firstName, nullptr, entry + 4, 0, 0xFFFFFFFF);
                hasFirstName = true;
            }
        }
    }

    if (secondPlayerId != -1)
    {
        void* manager = reinterpret_cast<void*>(GetGlobalDword_73C020());
        int index = MpSpawnPoints_FindPlayerIndex(
            CSF_AT(void*, manager, 0xC8), nullptr, secondPlayerId);
        if (index != -1)
        {
            manager = reinterpret_cast<void*>(GetGlobalDword_73C020());
            char* entry = MpSpawnPoints_GetPlayerEntry(
                CSF_AT(void*, manager, 0xC8), nullptr,
                static_cast<unsigned int>(index));
            if (entry)
            {
                AssignStringRange_488830(
                    &secondName, nullptr, entry + 4, 0, 0xFFFFFFFF);
                hasSecondName = true;
            }
        }
    }

    wchar_t* message = reinterpret_cast<wchar_t*>(
        MenuMessageBox_ResolveEntryByKey(messageKey, 2));
    ConstructWideString(&localizedPattern, nullptr, message);

    if (hasFirstName || hasSecondName || hasCount)
    {
        void* firstText = nullptr;
        void* secondText = nullptr;

        if (hasSecondName)
        {
            secondText = secondName.field_0018 < 8
                ? static_cast<void*>(&secondName.field_0004)
                : reinterpret_cast<void*>(secondName.field_0004);
        }
        if (hasFirstName)
        {
            firstText = firstName.field_0018 < 8
                ? static_cast<void*>(&firstName.field_0004)
                : reinterpret_cast<void*>(firstName.field_0004);
        }

        const int formattingMode = g_NumberFormattingMode;
        MpSpawnPoints_FormatArgument patternArgument{};

        if (hasFirstName && !hasSecondName && !hasCount)
        {
            patternArgument.words[6] = 7;
            patternArgument.words[5] = 0;
            reinterpret_cast<std::uint16_t*>(&patternArgument.words[1])[0] = 0;
            AssignStringRange_488830(
                &patternArgument, nullptr, &localizedPattern, 0, 0xFFFFFFFF);
        }
        else
        {
            S_004f21d0_ResetAndAssign(
                reinterpret_cast<S_004f21d0*>(&patternArgument),
                nullptr, &localizedPattern);
        }

        // The supplied binding names the fixed output argument; the actual
        // caller also supplies the by-value pattern and variadic substitutions.
        auto format = reinterpret_cast<void* (__cdecl*)(void*, ...)>(
            FormatCountTextBuffer);
        void* formatted;

        if (hasFirstName && hasSecondName)
        {
            if (hasCount)
                formatted = format(&result, patternArgument,
                                   firstText, secondText, count, formattingMode);
            else
                formatted = format(&result, patternArgument,
                                   firstText, secondText, formattingMode);
        }
        else if (hasFirstName || hasSecondName)
        {
            void* name = hasFirstName ? firstText : secondText;
            if (hasCount)
                formatted = format(&result, patternArgument,
                                   name, count, formattingMode);
            else
                formatted = format(&result, patternArgument,
                                   name, formattingMode);
        }
        else
        {
            formatted = format(&result, patternArgument, count, formattingMode);
        }

        AssignStringRange_488830(&result, nullptr, formatted, 0, 0xFFFFFFFF);
    }
    else
    {
        AssignStringRange_488830(
            &result, nullptr, &localizedPattern, 0, 0xFFFFFFFF);
    }

    output->field_0018 = 7;
    output->field_0014 = 0;
    output->field_0004 = 0;
    AssignStringRange_488830(output, nullptr, &result, 0, 0xFFFFFFFF);

    if (localizedPattern.field_0018 >= 8 && localizedPattern.field_0004)
        Mem_FreeDebug(reinterpret_cast<void*>(localizedPattern.field_0004));
    localizedPattern.field_0018 = 7;
    localizedPattern.field_0014 = 0;
    reinterpret_cast<std::uint16_t*>(&localizedPattern.field_0004)[0] = 0;

    if (secondName.field_0018 >= 8 && secondName.field_0004)
        Mem_FreeDebug(reinterpret_cast<void*>(secondName.field_0004));
    secondName.field_0018 = 7;
    secondName.field_0014 = 0;
    reinterpret_cast<std::uint16_t*>(&secondName.field_0004)[0] = 0;

    if (firstName.field_0018 >= 8 && firstName.field_0004)
        Mem_FreeDebug(reinterpret_cast<void*>(firstName.field_0004));
    firstName.field_0018 = 7;
    firstName.field_0014 = 0;
    reinterpret_cast<std::uint16_t*>(&firstName.field_0004)[0] = 0;

    if (result.field_0018 >= 8 && result.field_0004)
        Mem_FreeDebug(reinterpret_cast<void*>(result.field_0004));

    return output;
}

HOOK(0x00555830, MpSpawnPoints_FormatLocalizedMessage, "auto");
