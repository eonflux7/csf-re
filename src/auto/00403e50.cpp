#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// exception::exception(const exception&) - __thiscall, one stack arg (ret 4)
CSF_FUNC(void*, __fastcall, sub_7AF6B1, 0x007AF6B1, (void* self, void* edx, void* a2));

// MenuExtras_AssignStringRange - __thiscall, three stack args (ret 12)
CSF_FUNC(void*, __fastcall, MenuExtras_AssignStringRange, 0x00403B60, (void* self, void* edx, void* source, std::uint32_t position, std::uint32_t count));

// Constructs C_008769d4 from an exception-derived source: base ctor, vtable install,
// clears the string-state fields, then assigns the source's string range (whole string).
static int __fastcall C_008769d4_Construct(C_008769d4* self, void* /*edx*/, void* a2)
{
    sub_7AF6B1(self, nullptr, a2);

    self->vtbl = CSF_VTBL(0x008769D4);
    CSF_AT(std::uint32_t, self, 0x24) = 15;
    CSF_AT(std::uint32_t, self, 0x20) = 0;
    CSF_AT(std::uint8_t, self, 0x10) = 0;

    MenuExtras_AssignStringRange(static_cast<std::uint8_t*>(static_cast<void*>(self)) + 0x0C,
                                 nullptr,
                                 static_cast<std::uint8_t*>(a2) + 0x0C,
                                 0u,
                                 0xFFFFFFFFu);

    return static_cast<int>(reinterpret_cast<std::uintptr_t>(self));
}

HOOK(0x00403E50, C_008769d4_Construct, "auto");
