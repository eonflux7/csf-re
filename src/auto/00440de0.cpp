#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Node-allocating helper of S_00440480 (thiscall, 2 stack args, ret 8).
CSF_FUNC(C_0082d410*, __fastcall, BriefingList_AllocNode, 0x00440480,
         (S_00440480* self, void* edx, int a2, uint32_t a3));

// Converts a 32-bit stored link value into a node address.
static void* AsNode(uint32_t value) {
    return reinterpret_cast<void*>(static_cast<uintptr_t>(value));
}

// Links a new entry into the circular list headed by this->field_0004.
// Returns the first node whose flag byte at +0x2D is set (the list sentinel).
static int* __fastcall BriefingList_LinkEntry(S_00440480* self, void* /*edx*/, int a2) {
    // Referenced vtable of S_00440480 (kept as the original references it).
    (void)CSF_VTBL(0x00440480);

    void* argRecord = AsNode(static_cast<uint32_t>(a2));
    void* sourceRecord = AsNode(CSF_AT(uint32_t, argRecord, 4));

    void* head = AsNode(self->field_0004);
    CSF_AT(uint32_t, head, 4) = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(
        BriefingList_AllocNode(self, nullptr,
                               static_cast<int>(CSF_AT(uint32_t, sourceRecord, 4)),
                               self->field_0004)));

    self->field_0008 = CSF_AT(uint32_t, argRecord, 8);

    void* v4 = AsNode(self->field_0004);
    void* result = AsNode(CSF_AT(uint32_t, v4, 4));

    if (CSF_AT(uint8_t, result, 0x2D) != 0) {
        CSF_AT(uint32_t, v4, 0) = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(v4));
        CSF_AT(uint32_t, AsNode(self->field_0004), 8) = self->field_0004;
    } else {
        void* cur = result;
        uint32_t next = CSF_AT(uint32_t, cur, 0);
        while (CSF_AT(uint8_t, AsNode(next), 0x2D) == 0) {
            cur = AsNode(next);
            next = CSF_AT(uint32_t, cur, 0);
        }
        CSF_AT(uint32_t, v4, 0) = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(cur));

        void* head2 = AsNode(self->field_0004);
        void* v8 = AsNode(CSF_AT(uint32_t, head2, 4));
        result = AsNode(CSF_AT(uint32_t, v8, 8));
        while (CSF_AT(uint8_t, result, 0x2D) == 0) {
            v8 = result;
            result = AsNode(CSF_AT(uint32_t, result, 8));
        }
        CSF_AT(uint32_t, head2, 8) = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(v8));
    }

    return reinterpret_cast<int*>(result);
}

HOOK(0x00440DE0, BriefingList_LinkEntry, "auto");
