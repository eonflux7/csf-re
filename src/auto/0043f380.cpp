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

// Assigns a string into the embedded string member at this+0x0C.
CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0,
         (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));

// __thiscall with 5 stack args (ret 0x14): ecx = this, edx unused.
static C_0082d410* __fastcall C_0082d410_InitFromSourceString(C_0082d410* self, void* /*edx*/,
                                                             void* vtablePtr, std::uint32_t owner,
                                                             C_00833908* ownerRef, void* sourceString,
                                                             char flag) {
    // Store vtable, owner id, and back-reference.
    CSF_AT(void*, self, 0x00) = vtablePtr;
    self->field_0004 = owner;
    self->field_0008 = static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(ownerRef));

    // Reset the embedded string member at +0x0C: capacity 15, length 0, empty buffer.
    void* stringBody = reinterpret_cast<void*>(reinterpret_cast<std::uint8_t*>(self) + 0x0C);
    CSF_AT(std::uint32_t, self, 0x24) = 15;
    CSF_AT(std::uint32_t, self, 0x20) = 0;
    CSF_AT(std::uint8_t, self, 0x10) = 0;

    // Copy the whole source string (position 0, count -1).
    AssignStringSubstring(stringBody, nullptr, sourceString, 0, 0xFFFFFFFFu);

    // Copy the source's dword at +0x1C into this+0x28.
    CSF_AT(std::uint32_t, self, 0x28) = CSF_AT(std::uint32_t, sourceString, 0x1C);

    // Trailing flag byte and cleared byte.
    CSF_AT(std::uint8_t, self, 0x2C) = static_cast<std::uint8_t>(flag);
    CSF_AT(std::uint8_t, self, 0x2D) = 0;

    return self;
}

HOOK(0x0043F380, C_0082d410_InitFromSourceString, "auto");
