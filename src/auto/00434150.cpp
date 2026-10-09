#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringRange_429B60, 0x00429B60, (void* self, void* edx, char* text, unsigned int length));

static C_0082d410* __fastcall C_0082d410_ConstructFromText(C_0082d410* self, void* /*edx*/, uint32_t* a2)
{
    const char* text = reinterpret_cast<const char*>(a2);

    CSF_AT(uint32_t, self, 0x18) = 15;
    CSF_AT(uint32_t, self, 0x14) = 0;
    self->field_0004 = (self->field_0004 & 0xFFFFFF00u);

    // Manual length scan (the original inlines this loop; no strlen call).
    const char* cursor = text;
    while (*cursor != '\0') {
        ++cursor;
    }
    unsigned int length = static_cast<unsigned int>(cursor - text);

    AssignStringRange_429B60(self, nullptr, const_cast<char*>(text), length);
    return self;
}

HOOK(0x00434150, C_0082d410_ConstructFromText, "auto");
