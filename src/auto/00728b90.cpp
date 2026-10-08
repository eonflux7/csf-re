#include <cstdint>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "globals.h"

CSF_FUNC(int, __fastcall, C_00863f60_SetNamedFourWordValue, 0x00713D10,
         (C_00865a10* self, void* edx, const std::uint32_t* value,
          const char* name));

static int __fastcall C_00865a10_SetNamedFourWordValue(
    C_00865a10* self, void* /*edx*/,
    const std::uint32_t* value, const char* name)
{
    const char* objectName =
        ((const char* (__fastcall*)(void*, void*))self->vtbl[10])(
            self, nullptr);

    const auto* input = reinterpret_cast<const unsigned char*>(name);
    const auto* expected =
        reinterpret_cast<const unsigned char*>(objectName);

    for (;;) {
        const unsigned char character = *input;
        if (character != *expected)
            return C_00863f60_SetNamedFourWordValue(
                self, nullptr, value, name);

        if (character == 0)
            break;

        ++input;
        ++expected;
    }

    CSF_AT(std::uint32_t, self, 0x30) = value[0];
    CSF_AT(std::uint32_t, self, 0x34) = value[1];
    CSF_AT(std::uint32_t, self, 0x38) = value[2];

    const std::uint32_t lastWord = value[3];
    CSF_AT(std::uint32_t, self, 0x3C) = lastWord;
    return static_cast<int>(lastWord);
}

HOOK(0x00728B90, C_00865a10_SetNamedFourWordValue, "auto");
