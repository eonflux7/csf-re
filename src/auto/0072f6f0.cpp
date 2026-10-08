#include <cstdint>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "globals.h"

CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(std::uint32_t, __fastcall, StringEqualsText, 0x00696B60,
         (std::uint16_t* self, void* edx, char* text));
CSF_FUNC(std::uint32_t*, __fastcall, AssignStringText, 0x00697130,
         (std::uint32_t* self, void* edx, char* text));
CSF_FUNC(std::uint32_t, __fastcall, AppendElement32, 0x0072F590,
         (void* self, void* edx, std::uint32_t* element));

static void* __fastcall CGuiChoiceButton_AddChoiceText(
    CGuiChoiceButton* self, void* /*edx*/, char* text)
{
    if (text == nullptr)
        return nullptr;

    const auto begin = CSF_AT(std::uint32_t, self, 0x1F4);
    const std::uint32_t initialCount = begin != 0
        ? static_cast<std::uint32_t>(
              static_cast<std::int32_t>(
                  CSF_AT(std::uint32_t, self, 0x1F8) - begin) >> 5)
        : 0;

    std::uint32_t result = begin;
    std::uint32_t index = 0;
    std::uint32_t offset = 0;

    if (initialCount != 0) {
        for (;;) {
            const auto record = CSF_AT(std::uint32_t, self, 0x1F4) + offset;
            result = StringEqualsText(
                reinterpret_cast<std::uint16_t*>(record + 4), nullptr, text);
            if (static_cast<std::uint8_t>(result) != 0) {
                if (index < initialCount)
                    return reinterpret_cast<void*>(result);
                break;
            }

            ++index;
            offset += 32;
            if (index >= initialCount)
                break;
        }
    }

    std::uint32_t temporary[8];
    temporary[7] = 7;
    temporary[6] = 0;
    CSF_AT(std::uint16_t, temporary, 8) = 0;
    temporary[0] = 0;

    AssignStringText(temporary + 1, nullptr, text);
    result = AppendElement32(
        reinterpret_cast<std::uint8_t*>(self) + 0x1F0,
        nullptr, temporary);

    if (initialCount == 0) {
        result = reinterpret_cast<std::uint32_t(__fastcall*)(
            void*, void*, int)>(CSF_AT(void**, self, 0)[0x2BC / 4])(
                self, nullptr, 1);
    }

    if (temporary[7] >= 8) {
        result = temporary[2];
        if (result != 0) {
            result = static_cast<std::uint32_t>(
                Mem_FreeDebug(reinterpret_cast<void*>(result)));
        }
    }
    return reinterpret_cast<void*>(result);
}

HOOK(0x0072F6F0, CGuiChoiceButton_AddChoiceText, "auto");
