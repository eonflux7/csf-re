#include <cstdint>
#include "globals.h"
#include <cstring>
#include "hook.h"
#include "core/memory.h"

static CSF_FUNC(void*, __fastcall, AssignStringRange_429B60, 0x00429B60, (void* self, void* edx, char* text, unsigned int length));

static int __fastcall HudStringShow_SetText(void* self, void* /*edx*/, const char* text)
{
    void* result = AssignStringRange_429B60(
        static_cast<uint8_t*>(self) + 4,
        nullptr,
        const_cast<char*>(text),
        static_cast<unsigned int>(std::strlen(text)));
    CSF_AT(uint32_t, self, 0x48) = 0;
    return static_cast<int>(reinterpret_cast<intptr_t>(result));
}

HOOK(0x004F4850, HudStringShow_SetText, "auto");
