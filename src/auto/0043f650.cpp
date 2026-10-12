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

static void* __fastcall C_008769ec_ConstructFromException(C_008769ec* self, void* /*edx*/, void* a2)
{
    // Base exception copy-construction from the source exception.
    sub_7AF6B1(self, nullptr, a2);

    // Base-class vtable is installed first.
    *(void***)self = CSF_VTBL(0x008769D4);

    // Object fields: size/capacity-style values and flag byte.
    CSF_AT(std::uint32_t, self, 0x24) = 15;
    CSF_AT(std::uint32_t, self, 0x20) = 0;
    CSF_AT(std::uint8_t, self, 0x10) = 0;

    // Copy string range from a2 + 12, position 0, count -1 (npos).
    void* source = (std::uint8_t*)a2 + 12;
    MenuExtras_AssignStringRange((void*)((std::uint8_t*)self + 0x0C), nullptr,
                                 source, 0u, 0xFFFFFFFFu);

    *(void***)self = CSF_VTBL(0x008769EC);
    return self;
}

HOOK(0x0043F650, C_008769ec_ConstructFromException, "auto");
