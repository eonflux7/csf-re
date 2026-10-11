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

// AssignStringSubstring(dst, src, position, count): copies a substring into the string at dst.
CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0,
         (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));

// Copy-constructs the character data record at 0x0082D410 from a source record.
// Copies the vtable pointer and two header dwords, copies the embedded string
// at +0x0C, then copies the trailing fields at +0x28..+0x34.
static C_0082d410* __fastcall C_0082d410_CopyFromSource(C_0082d410* self, void* /*edx*/, C_0082d410** source)
{
    CSF_AT(void*, self, 0x00) = CSF_AT(void*, source, 0x00);
    self->field_0004 = CSF_AT(std::uint32_t, source, 0x04);
    self->field_0008 = CSF_AT(std::uint32_t, source, 0x08);

    AssignStringSubstring(reinterpret_cast<std::uint8_t*>(self) + 0x0C, nullptr,
                          reinterpret_cast<const std::uint8_t*>(source) + 0x0C,
                          0u, 0xFFFFFFFFu);

    CSF_AT(std::uint32_t, self, 0x28) = CSF_AT(std::uint32_t, source, 0x28);
    CSF_AT(std::uint32_t, self, 0x2C) = CSF_AT(std::uint32_t, source, 0x2C);
    CSF_AT(std::uint32_t, self, 0x30) = CSF_AT(std::uint32_t, source, 0x30);
    CSF_AT(std::uint32_t, self, 0x34) = CSF_AT(std::uint32_t, source, 0x34);

    return self;
}

HOOK(0x007625A0, C_0082d410_CopyFromSource, "auto");
