#include <cstdint>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "globals.h"

CSF_FUNC(int, __fastcall, C_00863f60_SetNamedByteValue, 0x00713D30,
         (C_008657e0* self, void* edx, int value, const char* name));

static int __fastcall C_008657e0_SetNamedByteValue(
    C_008657e0* self, void* /*edx*/, int value, const char* name)
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
            return C_00863f60_SetNamedByteValue(
                self, nullptr, value, name);

        if (character == 0)
            break;

        ++input;
        ++expected;
    }

    self->field_004c = static_cast<std::uint8_t>(value);
    return 0;
}

HOOK(0x007285A0, C_008657e0_SetNamedByteValue, "auto");
