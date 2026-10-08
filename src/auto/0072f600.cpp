#include <cstdint>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "globals.h"

CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(std::uint32_t, __cdecl, CopyConstructElementArray32, 0x0072F090,
         (std::uint32_t destination, int count, std::uint32_t* source,
          void* container, int choiceId));
CSF_FUNC(std::uint32_t, __fastcall, InsertElementCopies32_72F220, 0x0072F220,
         (void* self, void* edx, char* position, unsigned int count,
          std::uint32_t* source));

static char* __fastcall CGuiChoiceButton_AddChoiceId(
    CGuiChoiceButton* self, void* /*edx*/, int choiceId)
{
    auto* records = reinterpret_cast<std::uint8_t*>(self) + 0x1F0;
    const auto begin = CSF_AT(std::uint32_t, self, 0x1F4);
    const std::uint32_t initialCount = begin != 0
        ? static_cast<std::uint32_t>(
              static_cast<std::int32_t>(
                  CSF_AT(std::uint32_t, records, 8) - begin) >> 5)
        : 0;

    std::uint32_t result = 0;
    if (initialCount != 0) {
        auto cursor = CSF_AT(std::uint32_t, self, 0x1F4);
        while (CSF_AT(int, reinterpret_cast<void*>(cursor), 0) != choiceId) {
            ++result;
            cursor += 32;
            if (result >= initialCount)
                break;
        }
        if (result < initialCount)
            return reinterpret_cast<char*>(result);
    }

    std::uint32_t temporary[8];
    temporary[0] = static_cast<std::uint32_t>(choiceId);
    const auto currentBegin = CSF_AT(std::uint32_t, records, 4);
    temporary[7] = 7;
    temporary[6] = 0;
    CSF_AT(std::uint16_t, temporary, 8) = 0;

    std::uint32_t used = 0;
    if (currentBegin != 0) {
        used = static_cast<std::uint32_t>(
            static_cast<std::int32_t>(
                CSF_AT(std::uint32_t, records, 8) - currentBegin) >> 5);
    }

    if (currentBegin != 0 &&
        used < static_cast<std::uint32_t>(
                   static_cast<std::int32_t>(
                       CSF_AT(std::uint32_t, records, 12) - currentBegin) >> 5)) {
        const auto destination = CSF_AT(std::uint32_t, records, 8);
        result = CopyConstructElementArray32(destination, 1, temporary,
                                             records, choiceId);
        CSF_AT(std::uint32_t, records, 8) = destination + 32;
    } else {
        result = InsertElementCopies32_72F220(
            records, nullptr,
            reinterpret_cast<char*>(CSF_AT(std::uint32_t, records, 8)),
            1, temporary);
    }

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
    return reinterpret_cast<char*>(result);
}

HOOK(0x0072F600, CGuiChoiceButton_AddChoiceId, "auto");
