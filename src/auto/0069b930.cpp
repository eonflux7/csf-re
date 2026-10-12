#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0, (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));

// Initializes a record { uint32 id; C_0082d410 text; bool flag @+8; int zero @+0x18; int capacity @+0x1C }
// from a source pair (id dword, followed by a string), then assigns the string into the embedded text object.
static int __stdcall InitStringRecordFromPair(int a1, std::uint32_t* a2)
{
    int result = a1;
    if (a1 != 0)
    {
        std::uint8_t* record = reinterpret_cast<std::uint8_t*>(static_cast<std::uintptr_t>(a1));
        CSF_AT(std::uint32_t, record, 0x00) = a2[0];
        CSF_AT(std::int32_t, record, 0x1C) = 15;
        CSF_AT(std::int32_t, record, 0x18) = 0;
        CSF_AT(std::uint8_t, record, 0x08) = 0;

        auto* text = reinterpret_cast<C_0082d410*>(record + 4);
        void* source = a2 + 1;
        // The original leaves the source pointer in edx and also pushes it as the first stack argument.
        void* assigned = AssignStringSubstring(text, source, source, 0u, 0xFFFFFFFFu);
        return static_cast<int>(reinterpret_cast<std::uintptr_t>(assigned));
    }
    return result;
}

HOOK(0x0069B930, InitStringRecordFromPair, "auto");
