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

// Initializes the text input subobject embedded at +0x7C with the given string.
CSF_FUNC(void*, __fastcall, TextInputSubobject_Init, 0x00697130, (void* self, void* edx, uint8_t* arg));

// Sets the station/session name text. A non-empty string is handed to the embedded
// text input subobject at +0x7C; otherwise the 16-bit text slot is cleared, either in
// the inline buffer at +0x80 or in the heap buffer whose pointer is stored at +0x80
// when the capacity at +0x94 is 8 or more.
static void* __fastcall NetSession_SetStationNameText(void* self, void* /*edx*/, uint8_t* text)
{
    void* result = text;
    if (text != nullptr && *text != 0)
    {
        return TextInputSubobject_Init(static_cast<uint8_t*>(self) + 0x7C, nullptr, text);
    }

    uint16_t* slot;
    if (CSF_AT(uint32_t, self, 0x94) < 8u)
    {
        slot = reinterpret_cast<uint16_t*>(static_cast<uint8_t*>(self) + 0x80);
    }
    else
    {
        slot = reinterpret_cast<uint16_t*>(CSF_AT(uint32_t, self, 0x80));
    }
    *slot = 0;
    return result;
}

HOOK(0x0073D0A0, NetSession_SetStationNameText, "auto");
