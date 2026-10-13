#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(uint32_t*, __fastcall, sub_6BE440, 0x006BE440, (void* self, void* edx, int a2));
CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0,
         (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));

// The packet lists 0x006BE440 as this class's vtable; it is also the callee bound above.
[[maybe_unused]] static const auto kEnvironmentVtable = CSF_VTBL(0x006BE440);

static int __fastcall GfxEnvironment_AppendClumpDescriptor(S_006be440* self, void* /*edx*/,
                                                           int sourceEntry, int replacementIndex)
{
    if (self->field_0008 >= self->field_0004) {
        int growArg = replacementIndex;
        if (growArg == 0)
            growArg = static_cast<int>(self->field_000c);
        sub_6BE440(self, nullptr, growArg);
    }

    // Re-read the count after a possible growth call.
    std::uint32_t index = self->field_0008;
    std::uint8_t* entry = reinterpret_cast<std::uint8_t*>(self->field_0000) + 56u * index;
    self->field_0008 = index + 1;

    std::uint8_t* src = reinterpret_cast<std::uint8_t*>(sourceEntry);

    CSF_AT(std::uint32_t, entry, 0x00) = CSF_AT(std::uint32_t, src, 0x00);
    CSF_AT(std::uint32_t, entry, 0x04) = CSF_AT(std::uint32_t, src, 0x04);

    AssignStringSubstring(entry + 0x08, nullptr, src + 0x08, 0u, static_cast<std::uint32_t>(-1));

    CSF_AT(std::uint32_t, entry, 0x24) = CSF_AT(std::uint32_t, src, 0x24);
    CSF_AT(std::uint32_t, entry, 0x28) = CSF_AT(std::uint32_t, src, 0x28);
    CSF_AT(std::uint32_t, entry, 0x2C) = CSF_AT(std::uint32_t, src, 0x2C);
    CSF_AT(std::uint32_t, entry, 0x30) = CSF_AT(std::uint32_t, src, 0x30);
    CSF_AT(std::uint32_t, entry, 0x34) = CSF_AT(std::uint32_t, src, 0x34);

    return static_cast<int>(reinterpret_cast<std::uintptr_t>(entry));
}

HOOK(0x006BE5C0, GfxEnvironment_AppendClumpDescriptor, "auto");
