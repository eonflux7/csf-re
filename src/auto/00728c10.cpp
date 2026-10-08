#include <cstdint>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "globals.h"

CSF_FUNC(int, __fastcall, C_00863f60_SetNamedByteValue, 0x00713D30,
         (C_00865a10* self, void* edx, int value, const char* name));

// Compares the given name against this object's own name; when they are equal,
// stores the byte value into field_0040, otherwise forwards to the 0x713D30
// handler which routes the value to the named sub-object.
static int __fastcall C_00865a10_SetNamedByteValue(
    C_00865a10* self, void* /*edx*/, int value, const char* name)
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

    self->field_0040 = static_cast<std::uint8_t>(value);
    return 0;
}

HOOK(0x00728C10, C_00865a10_SetNamedByteValue, "auto");
