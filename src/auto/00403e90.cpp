#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// exception::exception(const exception&) - __thiscall, one stack arg, ret 4
CSF_FUNC(void*, __fastcall, sub_7AF6B1, 0x007AF6B1, (void* self, void* edx, void* a2));

// MenuExtras_AssignStringRange - __thiscall, three stack args, ret 12
CSF_FUNC(void*, __fastcall, MenuExtras_AssignStringRange, 0x00403B60,
         (void* self, void* edx, void* source, std::uint32_t position, std::uint32_t count));

// Constructs C_008769e0 from an exception: base exception copy, base vtable,
// size/flag fields, copies the string range at a2+12 (pos 0, count npos), final vtable.
static void* __fastcall C_008769e0_ConstructFromException(C_008769e0* self, void* /*edx*/, void* a2)
{
    // Base exception copy-construction from the source exception.
    sub_7AF6B1(self, nullptr, a2);

    // Base-class vtable is installed.
    *reinterpret_cast<void***>(self) = CSF_VTBL(0x008769D4);

    // Object fields: capacity-style value, length-style value, flag byte.
    CSF_AT(std::uint32_t, self, 0x24) = 15;
    CSF_AT(std::uint32_t, self, 0x20) = 0;
    CSF_AT(std::uint8_t, self, 0x10) = 0;

    // Copy string range from a2 + 12, position 0, count -1 (npos).
    void* source = reinterpret_cast<std::uint8_t*>(a2) + 12;
    MenuExtras_AssignStringRange(reinterpret_cast<std::uint8_t*>(self) + 0x0C, nullptr,
                                 source, 0u, 0xFFFFFFFFu);

    // Final vtable of this class.
    *reinterpret_cast<void***>(self) = CSF_VTBL(0x008769E0);
    return self;
}

HOOK(0x00403E90, C_008769e0_ConstructFromException, "auto");
