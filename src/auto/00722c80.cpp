#include <cstdint>
#include "types/auto_classes.h"
#include "hook.h"

// Returns whether the pointer-sized field at +0x218 of CGuiListBox is non-zero.
static int __fastcall CGuiListBox_HasField0218(CGuiListBox* self, void* /*edx*/)
{
    return self->field_0218 != 0;
}

HOOK(0x00722C80, CGuiListBox_HasField0218, "auto");
