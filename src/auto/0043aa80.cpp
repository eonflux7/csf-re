#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Signature of the virtual slot at +0x6C: returns a pointer to four dwords (rect-like record).
using MinimapGetRectFn = uint32_t* (__fastcall*)(void* self, void* edx);
// Signature of the virtual slot at +0x68: consumes a four-dword record.
using MinimapSubmitRectFn = void (__fastcall*)(void* self, void* edx, uint32_t* record);

static void __cdecl Minimap_SubmitRectWithFullAlpha(int* self)
{
    if (self == nullptr)
        return;

    // Read the record provided by the object's getter (vtable slot 0x6C / 4 = 27).
    auto getRect = reinterpret_cast<MinimapGetRectFn>(CSF_AT(void**, self, 0)[27]);
    uint32_t* source = getRect(self, nullptr);

    // Copy the first three dwords; the fourth component is replaced with the constant below.
    uint32_t record[4];
    record[0] = source[0];
    record[1] = source[1];
    record[2] = source[2];
    record[3] = 0x437F0000u; // float 255.0f

    // Submit the record through vtable slot 0x68 / 4 = 26.
    auto submitRect = reinterpret_cast<MinimapSubmitRectFn>(CSF_AT(void**, self, 0)[26]);
    submitRect(self, nullptr, record);
}

HOOK(0x0043AA80, Minimap_SubmitRectWithFullAlpha, "auto");
