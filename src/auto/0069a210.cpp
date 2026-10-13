#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Writes a NUL-terminated string through this object's sink (field_0004).
// Returns 1 when the sink reports it wrote the full length, otherwise 0.
// A null string is forwarded to the class's slot 0x40 handler as length 0.
static int __fastcall C_0085e760_WriteNullTerminatedText(C_0085e760* self, void* /*edx*/, const char* a2)
{
    if (a2 == nullptr)
        return self->vSlot40(0);

    unsigned int textLength = static_cast<unsigned int>(std::strlen(a2));
    int result = self->vSlot40(textLength);

    if (static_cast<uint8_t>(result) != 0)
    {
        uint32_t sinkField = self->field_0004;
        self->field_0008 += textLength;

        void* sink = reinterpret_cast<void*>(sinkField);
        auto sinkWrite = reinterpret_cast<uint32_t(__fastcall*)(void*, void*, const char**, unsigned int)>(
            CSF_AT(void**, sink, 0)[3]);
        uint32_t written = sinkWrite(sink, nullptr, &a2, textLength);

        return written == textLength ? 1 : 0;
    }

    return result;
}

HOOK(0x0069A210, C_0085e760_WriteNullTerminatedText, "auto");
