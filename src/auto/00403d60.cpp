#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, MenuExtras_AssignString, 0x00403C60, (void* self, int registerArgument, char* text, std::uint32_t length));

static C_0082d410* __fastcall C_0082d410_InitWithCopiedText(C_0082d410* self, void* /*edx*/, uint32_t* a2)
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

    // edx still holds the a2 argument when the original calls MenuExtras_AssignString.
    MenuExtras_AssignString(self, reinterpret_cast<int>(a2), const_cast<char*>(text), length);
    return self;
}

HOOK(0x00403D60, C_0082d410_InitWithCopiedText, "auto");

