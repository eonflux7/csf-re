#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int, __fastcall, ParseIntegerComponent, 0x00692C40, (std::uint32_t component));
CSF_FUNC(std::int32_t, __cdecl, ParseThumbnailIndex, 0x007AFD89, (const char* text));

static int __fastcall CoreParser_ParseTypedValueToInt(int* self, void* /*edx*/) {
    const int valueType = CSF_AT(int, self, 0x00);
    if (valueType == 0) {
        return ParseIntegerComponent(CSF_AT(std::uint32_t, self, 0x18));
    }
    if (valueType != 3 && valueType != 4) {
        return 0;
    }

    const std::uint32_t stringRef = CSF_AT(std::uint32_t, self, 0x04);
    std::uint32_t copyLength = CSF_AT(std::uint32_t, stringRef, 0x00);
    if (copyLength > 0xFFu) {
        copyLength = 255;
    }

    char text[0x100];
    std::memcpy(text, CSF_AT(const void*, stringRef, 0x04), copyLength);
    text[copyLength] = 0;

    return ParseThumbnailIndex(text);
}

HOOK(0x00692800, CoreParser_ParseTypedValueToInt, "auto");
