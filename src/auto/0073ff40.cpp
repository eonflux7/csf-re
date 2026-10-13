#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
CSF_FUNC(uint32_t, __fastcall, Ready, 0x0064B250, (void* self, void* edx));
CSF_FUNC(uint32_t, __fastcall, Send, 0x00742670, (void* self, void* edx, uint32_t a, uint32_t b, uint32_t c));
CSF_FUNC(uint8_t*, __cdecl, Alloc, 0x006910F0, (size_t bytes));
CSF_FUNC(uint32_t*, __stdcall, Buy, 0x0073FB10, (uint32_t next, uint32_t prev, uint8_t** value));
CSF_FUNC(uint32_t, __fastcall, Inc, 0x0073FE80, (void* self, void* edx, uint32_t count));
// Game message insertion, not an STL member (survey cluster 460).
static uint8_t __fastcall InsertMessage_0073ff40(uint8_t* self, void*, uint32_t a, uint32_t b, uint32_t c) {
    uint32_t result = Ready(self, nullptr);
    if (static_cast<uint8_t>(result)) {
        Send(self, nullptr, a, b, c);
        uint8_t* value = Alloc(20);
        result = reinterpret_cast<uintptr_t>(value);
        if (value) {
            value[16] = 0;
            *reinterpret_cast<uint32_t*>(value) = a;
            *reinterpret_cast<uint32_t*>(value + 4) = b;
            *reinterpret_cast<uint32_t*>(value + 12) = c;
            uint32_t head = *reinterpret_cast<uint32_t*>(self + 144);
            uint32_t* node = Buy(head, *reinterpret_cast<uint32_t*>(head + 4), &value);
            result = Inc(self + 140, nullptr, 1);
            *reinterpret_cast<uint32_t**>(head + 4) = node;
            *reinterpret_cast<uint32_t**>(node[1]) = node;
        }
    }
    return static_cast<uint8_t>(result);
}
HOOK(0x0073FF40, InsertMessage_0073ff40, "stl");
