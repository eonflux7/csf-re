#include <cstdint>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "globals.h"

// __thiscall sub_724D20(int *this) -> CGuiStatic_DispatchTextObject
static int (__fastcall* const CGuiStatic_DispatchTextObject)(
    CGuiChoiceButton*, void*) =
    reinterpret_cast<int (__fastcall*)(CGuiChoiceButton*, void*)>(CSF_ADDR_FN(0x00724D20));

// __thiscall sub_4856E0(std::uint32_t *this, unsigned int pos, unsigned int len)  (ret 8)
static std::uint32_t* (__fastcall* const EraseWideStringRange)(
    void*, void*, unsigned int, unsigned int) =
    reinterpret_cast<std::uint32_t* (__fastcall*)(
        void*, void*, unsigned int, unsigned int)>(CSF_ADDR_FN(0x004856E0));

// Dispatches the control's text object, then for every wide-string entry in the
// choice vector (stride 0x20, begin at +0x1F4 / end at +0x1F8) erases the
// occupied character range [used-begin, end).
static int __fastcall CGuiChoiceButton_ResetChoiceStrings(
    CGuiChoiceButton* self, void* /*edx*/)
{
    CGuiStatic_DispatchTextObject(self, nullptr);

    const std::uint32_t begin = CSF_AT(std::uint32_t, self, 0x1F4);
    std::uint32_t remaining = 0;
    if (begin != 0)
    {
        const std::uint32_t difference =
            CSF_AT(std::uint32_t, self, 0x1F8) - begin;
        remaining = static_cast<std::uint32_t>(
            static_cast<std::int32_t>(difference) >> 5);
    }

    std::uint32_t entryOffset = 0;
    while (remaining != 0)
    {
        auto* entry = reinterpret_cast<std::uint8_t*>(
            static_cast<std::uintptr_t>(
                CSF_AT(std::uint32_t, self, 0x1F4) + entryOffset));

        const std::uint32_t capacity =
            CSF_AT(std::uint32_t, entry, 0x1C);
        void* stringObject = entry + 4;
        void* inlineStorage = entry + 8;
        const std::uint32_t inlineAddress =
            static_cast<std::uint32_t>(
                reinterpret_cast<std::uintptr_t>(inlineStorage));

        const std::uint32_t initialBuffer = capacity < 8
            ? inlineAddress
            : CSF_AT(std::uint32_t, inlineStorage, 0);

        const std::uint32_t endAddress = initialBuffer +
            CSF_AT(std::uint32_t, stringObject, 0x14) * 2u;

        const std::uint32_t beginAddress = capacity < 8
            ? inlineAddress
            : CSF_AT(std::uint32_t, inlineStorage, 0);

        const std::uint32_t comparisonAddress = capacity < 8
            ? inlineAddress
            : CSF_AT(std::uint32_t, inlineStorage, 0);

        const std::uint32_t position = beginAddress != 0
            ? static_cast<std::uint32_t>(
                static_cast<std::int32_t>(
                    beginAddress - comparisonAddress) >> 1)
            : 0;

        const std::uint32_t length = endAddress != 0
            ? static_cast<std::uint32_t>(
                static_cast<std::int32_t>(
                    endAddress - beginAddress) >> 1)
            : 0;

        EraseWideStringRange(stringObject, nullptr, position, length);

        entryOffset += 0x20;
        --remaining;
    }

    return static_cast<int>(remaining);
}

HOOK(0x0072EEC0, CGuiChoiceButton_ResetChoiceStrings, "auto");
