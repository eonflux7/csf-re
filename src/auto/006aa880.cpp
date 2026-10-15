#include <cstdint>
#include "globals.h"
#include "hook.h"
CSF_FUNC(void, __fastcall, Clear, 0x006A9A60, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, MakeResponse, 0x006A8B10, (void* self, void* edx, int index, void* value));
CSF_FUNC(uint32_t*, __fastcall, BuyNode, 0x006A8E30, (void* self, void* edx, uint32_t* next, uint32_t* prev, void* value));
CSF_FUNC(uint32_t, __fastcall, IncSize, 0x006A9FE0, (void* self, void* edx, uint32_t count));
static uint32_t* __fastcall HandleGatheringResponse(void* self, void*) {
    Clear(self, nullptr);
    auto* head = CSF_AT(uint32_t*, self, 0x44);
    auto* node = reinterpret_cast<uint32_t*>(head[0]);
    int index = 0;
    while (node != head) {
        void* value = MakeResponse(self, nullptr, index, node + 2);
        auto* list = static_cast<uint8_t*>(self) + 0x50;
        auto* sentinel = CSF_AT(uint32_t*, list, 4);
        auto* added = BuyNode(list, nullptr, sentinel, reinterpret_cast<uint32_t*>(sentinel[1]), &value);
        IncSize(list, nullptr, 1);
        sentinel[1] = reinterpret_cast<uint32_t>(added);
        *reinterpret_cast<uint32_t*>(added[1]) = reinterpret_cast<uint32_t>(added);
        node = reinterpret_cast<uint32_t*>(node[0]);
        head = CSF_AT(uint32_t*, self, 0x44);
        ++index;
    }
    return head;
}
HOOK(0x006AA880, HandleGatheringResponse, "stl");
