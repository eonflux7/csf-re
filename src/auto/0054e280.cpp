#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static int __fastcall C_008443c8_WriteString(
    C_008443c8* self, void* /*edx*/, const char* text, int /*unused*/)
{
    const int length = static_cast<int>(std::strlen(text));
    void* destination = reinterpret_cast<void*>(self->field_0004);
    const bool accepted =
        reinterpret_cast<bool (__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, destination, 0)[0x40 / 4])(
                destination, nullptr, length);

    if (accepted)
        return self->vSlot60(reinterpret_cast<int>(text), length);
    return 0;
}

HOOK(0x0054E280, C_008443c8_WriteString, "auto");
