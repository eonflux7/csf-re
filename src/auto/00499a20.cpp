#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Callee bound by address: sub_4993E0 (__fastcall, no stack args, ret 0)
CSF_FUNC(int, __fastcall, PrepareRequestSlotState, 0x004993E0, (void* self, void* edx));

// Builds a 0x2C-byte request object (vtable C_0082d448 + 0xFFFFFFFF fields)
// on the stack, queries the slot's object for a value, then submits the request.
static int __fastcall CPlayerCharacter_BuildAndQueryRequest(CPlayerCharacter* self, void* /*edx*/)
{
    PrepareRequestSlotState(self, nullptr);

    self->vSlot8C(0, 0, -1, 0);

    // Request object layout (11 dwords = 0x2C bytes):
    //   [0]  vtable C_0082d448
    //   [1..3] 0xFFFFFFFF
    //   [4]  query result + 1
    //   [5..10] zero
    uint32_t request[11] = {};
    request[0] = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(CSF_VTBL(0x0082D448)));
    std::memset(&request[1], 0xFF, 12);
    request[5] = 0;
    request[6] = 0;
    request[7] = 0;
    request[8] = 0;
    request[9] = 0;
    request[10] = 0;
    request[4] = 0;

    // First query: vtable slot 0x84 on the object at this->field_0038
    void* slotObj = reinterpret_cast<void*>(self->field_0038);
    int queryResult = reinterpret_cast<int(__fastcall*)(void*, void*)>(
        CSF_AT(void**, slotObj, 0)[0x84 / 4])(slotObj, nullptr);

    request[4] = static_cast<uint32_t>(queryResult + 1);
    request[5] = 0;

    // Submit: vtable slot 0x8C on the object at this->field_0038, passing the request
    void* submitObj = reinterpret_cast<void*>(self->field_0038);
    return reinterpret_cast<int(__fastcall*)(void*, void*, uint32_t*)>(
        CSF_AT(void**, submitObj, 0)[0x8C / 4])(submitObj, nullptr, request);
}

HOOK(0x00499A20, CPlayerCharacter_BuildAndQueryRequest, "auto");
