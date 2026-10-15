#include <cstdint>
#include "globals.h"
#include "hook.h"
CSF_FUNC(uint32_t*, __cdecl, Alloc, 0x006910F0, (uint32_t bytes));
CSF_FUNC(uint32_t*, __fastcall, BuyNode, 0x00782220, (void* self, void* edx, uint32_t* next, uint32_t* prev, void* value));
CSF_FUNC(uint32_t, __fastcall, IncSize, 0x0073FE80, (void* self, void* edx, uint32_t count));
// Lookup a keyed 16-byte record in a pointer list, optionally creating it.
static uint32_t* __fastcall FindOrCreateKeyedRecord(void* self, void*, uint32_t key, uint8_t create) {
    auto* head = CSF_AT(uint32_t*, self, 8);
    auto* node = reinterpret_cast<uint32_t*>(head[0]);
    while (node != head) {
        auto* record = reinterpret_cast<uint32_t*>(node[2]);
        if (record[0] == key && record) return record;
        node = reinterpret_cast<uint32_t*>(node[0]);
    }
    if (!create) return nullptr;
    auto* record = Alloc(0x10);
    record[0] = key; record[1] = 0; record[2] = 0; record[3] = 0;
    head = CSF_AT(uint32_t*, self, 8);
    auto* list = static_cast<uint8_t*>(self) + 4;
    auto* added = BuyNode(list, nullptr, head, reinterpret_cast<uint32_t*>(head[1]), &record);
    IncSize(list, nullptr, 1);
    head[1] = reinterpret_cast<uint32_t>(added);
    *reinterpret_cast<uint32_t*>(added[1]) = reinterpret_cast<uint32_t>(added);
    return record;
}
HOOK(0x00782330, FindOrCreateKeyedRecord, "stl");
