#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(unsigned int, __fastcall, CoreZlibLog_AppendText_694DC0, 0x00694DC0, (void* self, void* edx, char* a2));
CSF_FUNC(int, __fastcall, CoreZlibLog_AppendPlain_694EA0, 0x00694EA0, (void* self, void* edx, char* a2));

static unsigned int __fastcall CoreZlibLog_EndGroup_695260(S_00694cd0* self, void* /*edx*/)
{
    uint32_t depth = self->field_0004;
    uint8_t wasNested = CSF_AT(uint8_t, self, 0x11C + 8 * depth);

    if (depth) {
        self->field_0004 = depth - 1;
    }

    uint32_t indentLength = self->field_0004 + self->field_0010;
    if (indentLength) {
        memset(reinterpret_cast<uint8_t*>(self) + 0x14, 0x20, indentLength);
    }
    reinterpret_cast<uint8_t*>(self)[0x14 + indentLength] = 0;

    if (wasNested) {
        CoreZlibLog_AppendText_694DC0(self, nullptr, const_cast<char*>(CSF_STR(0x83F284, " ")));
    } else {
        CoreZlibLog_AppendText_694DC0(self, nullptr, reinterpret_cast<char*>(self) + 0x14);
    }

    uint8_t closeFlag = CSF_AT(uint8_t, self, 0x11C + 8 * self->field_0004);
    if (closeFlag) {
        return CoreZlibLog_AppendText_694DC0(self, nullptr, const_cast<char*>(CSF_STR(0x838FF4, ")")));
    }
    return static_cast<unsigned int>(CoreZlibLog_AppendPlain_694EA0(self, nullptr, const_cast<char*>(CSF_STR(0x838FF4, ")"))));
}

HOOK(0x00695260, CoreZlibLog_EndGroup_695260, "auto");
