#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Callee: char __fastcall sub_6D5D10(void* self, void* edx, int a2)
CSF_FUNC(char, __fastcall, sub_6D5D10, 0x006D5D10, (void* self, void* edx, int a2));

// Stores the incoming pointer at this+0xC0 and, if the child at this+0xBC exists,
// forwards the pointer to it. The result is the pointer value with its low byte
// replaced by the child's returned byte when a child is present.
static int __fastcall C_006caf00_StoreClumpAndNotifyChild(void* self, void* /*edx*/, uint32_t a2)
{
    int result = static_cast<int>(a2);
    CSF_AT(uint32_t, self, 0xC0) = a2;

    void* child = CSF_AT(void*, self, 0xBC);
    if (child)
    {
        uint8_t childByte = static_cast<uint8_t>(sub_6D5D10(child, nullptr, static_cast<int>(a2)));
        result = static_cast<int>((static_cast<uint32_t>(result) & 0xFFFFFF00u) | childByte);
    }
    return result;
}

HOOK(0x006CAF00, C_006caf00_StoreClumpAndNotifyChild, "auto");
