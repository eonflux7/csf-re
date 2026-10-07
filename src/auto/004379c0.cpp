#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

// The object embeds an MSVC std::string at offset 0x38. Its 16-byte "union"
// (inline buffer / heap pointer) lives at 0x38 and the reserved capacity lives
// at 0x4c. When the capacity is below 0x10 the small-buffer optimization is in
// effect and the buffer is inline at 0x38, otherwise 0x38 holds a heap pointer.
static void* __fastcall CGuiControl_GetStringData_4379C0(CGuiControl* self, void* /*edx*/)
{
    if (self->field_004c < 0x10u)
        return reinterpret_cast<uint8_t*>(self) + 0x38;
    return reinterpret_cast<void*>(self->field_0038);
}

HOOK(0x004379C0, CGuiControl_GetStringData_4379C0, "auto");
