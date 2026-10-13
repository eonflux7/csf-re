#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(unsigned int, __fastcall, StrBuf_ReserveForAppend, 0x00694CD0, (void* self, void* edx, int a2));

static unsigned int __fastcall S_00694cd0_AppendString(S_00694cd0* self, void* /*edx*/, const char* text)
{
    // The original references the vtable of S_00694cd0 at this call site.
    (void)CSF_VTBL(0x00694CD0);

    const unsigned int textLength = static_cast<unsigned int>(std::strlen(text));

    // Make room for the new characters first (may change the buffer).
    StrBuf_ReserveForAppend(self, nullptr, static_cast<int>(textLength));

    // Reload the buffer base and current length after the call.
    uint8_t* writePosition = reinterpret_cast<uint8_t*>(self->field_0000 + self->field_000c);
    std::memcpy(writePosition, text, textLength);

    const unsigned int newLength = textLength + self->field_000c;
    self->field_000c = newLength;
    return newLength;
}

HOOK(0x00694DC0, S_00694cd0_AppendString, "auto");
