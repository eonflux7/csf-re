#include <cstdint>
#include "types/auto_classes.h"
#include "hook.h"

// Returns the value stored at offset 0x284 (int).
static int __fastcall CMenuAccountEdit_GetField0284(CMenuAccountEdit* self, void* /*edx*/)
{
    return static_cast<int>(self->field_0284);
}

HOOK(0x0055C430, CMenuAccountEdit_GetField0284, "auto");
