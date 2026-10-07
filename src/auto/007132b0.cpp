#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

// Returns a pointer to the sub-object / member block at offset 0xC30 of this.
static char* __fastcall CInputDirectInput_GetBlock0C30(CInputDirectInput* self, void* /*edx*/)
{
    return reinterpret_cast<char*>(&self->field_0c30);
}

HOOK(0x007132B0, CInputDirectInput_GetBlock0C30, "auto");
