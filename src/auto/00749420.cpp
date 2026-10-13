#include <cstdint>
#include "globals.h"
#include "hook.h"
CSF_FUNC(uint32_t, __fastcall, Ready, 0x0064B250, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, Assign, 0x00488830, (void* self, void* edx, void* source, uint32_t pos, uint32_t count));
CSF_FUNC(uint32_t, __fastcall, Send, 0x00751060, (void* self, void* edx, uint32_t a, uint32_t b, uint32_t s0, uint32_t s1, uint32_t s2, uint32_t s3, uint32_t s4, uint32_t s5, uint32_t s6, uint32_t end));
CSF_FUNC(void*, __fastcall, Init, 0x00697130, (void* self, void* edx, const void* text));
CSF_FUNC(uint32_t*, __stdcall, Buy, 0x007491E0, (uint32_t* next, uint32_t* prev, void* value));
CSF_FUNC(uint32_t, __fastcall, Inc, 0x0073FE80, (void* self, void* edx, uint32_t count));
CSF_FUNC(uint32_t, __cdecl, Free, 0x00690E60, (void* block));
static uint8_t __fastcall ApplyFallbackSelection(uint8_t* self, void*, uint32_t a, uint32_t b, void* source, uint32_t end) {
    if (static_cast<uint8_t>(Ready(self, nullptr))) {
        alignas(4) uint32_t copy[7];
        copy[6] = 7; copy[5] = 0;
        *reinterpret_cast<uint16_t*>(copy + 1) = 0;
        Assign(copy, nullptr, source, 0, 0xFFFFFFFFu);
        Send(self, nullptr, a, b, copy[0], copy[1], copy[2], copy[3], copy[4], copy[5], copy[6], end);
    }
    alignas(4) uint32_t record[16];
    record[8] = 7; record[7] = 0;
    *reinterpret_cast<uint16_t*>(record + 3) = 0;
    reinterpret_cast<uint8_t*>(record)[40] = 0;
    Init(record + 2, nullptr, reinterpret_cast<void*>(CSF_ADDR_DATA(0x0082E65C)));
    record[1] = b; record[9] = static_cast<uint16_t>(end);
    record[11] = 0; record[12] = 0xFFFFFFFFu; record[13] = 0xFFFFFFFFu; record[14] = 0xFFFFFFFFu;
    reinterpret_cast<uint8_t*>(record)[60] = 0;
    reinterpret_cast<uint8_t*>(record)[61] = 0;
    reinterpret_cast<uint8_t*>(record)[62] = 0;
    record[0] = a;
    Assign(record + 2, nullptr, source, 0, 0xFFFFFFFFu);
    auto* head = *reinterpret_cast<uint32_t**>(self + 1120);
    uint32_t* node = Buy(head, reinterpret_cast<uint32_t*>(head[1]), record);
    Inc(self + 1116, nullptr, 1);
    uint32_t result = reinterpret_cast<uintptr_t>(node);
    head[1] = result;
    *reinterpret_cast<uint32_t**>(node[1]) = node;
    if (record[8] >= 8) {
        result = record[3];
        if (result) result = Free(reinterpret_cast<void*>(result));
    }
    return static_cast<uint8_t>(result);
}
HOOK(0x00749420, ApplyFallbackSelection, "stl");
