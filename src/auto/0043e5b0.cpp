#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// First byte of the panel-key constant. The original's repe cmpsb runs with ecx=1,
// so only the first character is compared, not a full strcmp.
CSF_GLOBAL(uint8_t, g_byte_82E65C, 0x0082E65C);

// Entry layout (196 bytes each, array base at self+0x34, index at self+0x1C):
//   +0x08 : pointer to value string (valid when capacity at +0x1C >= 0x10)
//   +0x1C : capacity of value string
//   +0x24 : inline buffer, or pointer to heap buffer when name capacity >= 0x10
//   +0x38 : capacity of name string

static uint32_t* __fastcall Campaign_ResolveCurrentPanelKey(uint8_t* self, void* /*edx*/)
{
    int panelIndex = CSF_AT(int, self, 0x1C);
    if (panelIndex == -1)
        return nullptr;

    uint8_t* panelArray = CSF_AT(uint8_t*, self, 0x34);
    uint8_t* panelEntry = panelArray + 196 * panelIndex;

    // Select the name text: inline buffer if capacity < 16, otherwise heap pointer.
    uint32_t nameCapacity = CSF_AT(uint32_t, panelEntry, 0x38);
    const char* nameText = (nameCapacity < 0x10)
        ? reinterpret_cast<const char*>(panelEntry + 0x24)
        : reinterpret_cast<const char*>(CSF_AT(uint32_t, panelEntry, 0x24));

    // Single-byte comparison against the constant (see note above).
    if (static_cast<uint8_t>(nameText[0]) == g_byte_82E65C)
    {
        uint32_t valueCapacity = CSF_AT(uint32_t, panelEntry, 0x1C);
        if (valueCapacity < 0x10)
            return reinterpret_cast<uint32_t*>(panelEntry + 8);
        return reinterpret_cast<uint32_t*>(CSF_AT(uint32_t, panelEntry, 8));
    }

    return reinterpret_cast<uint32_t*>(const_cast<char*>(nameText));
}

HOOK(0x0043E5B0, Campaign_ResolveCurrentPanelKey, "auto");
