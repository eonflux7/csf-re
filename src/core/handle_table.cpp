// Generational handle -> object lookup. 464 callers in the original; first hand-written hook (pipeline proof).
#include <cstddef>
#include <cstdint>
#include "hook.h"

struct CHandle {
    uint32_t unk0;
    int32_t generation;  // must match CHandleObject::generation
    int32_t index;       // slot in CHandleTable::entries
};
static_assert(sizeof(CHandle) == 0xC);

struct CHandleObject {
    uint8_t unk0[0x20];
    int32_t generation;
};
static_assert(offsetof(CHandleObject, generation) == 0x20);

struct CHandleTable {
    uint8_t unk0[0x10];
    CHandleObject** entries;
    uint32_t unk14;
    int32_t count;

    CHandleObject* Resolve(const CHandle* h);
};
static_assert(offsetof(CHandleTable, entries) == 0x10);
static_assert(offsetof(CHandleTable, count) == 0x18);

CHandleObject* CHandleTable::Resolve(const CHandle* h) {
    int32_t i = h->index;
    if (i < 0 || i >= count)
        return nullptr;
    CHandleObject* o = entries[i];
    if (o && o->generation != h->generation)
        return nullptr;
    return o;
}
HOOK_METHOD(0x44FC30, &CHandleTable::Resolve, "core");
