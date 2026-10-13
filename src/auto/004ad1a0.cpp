#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(void, __stdcall, PrepareStaticAiState, 0x004DC3C0, (void* a1));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));

static int __stdcall AiOrder_PrepareAndDispatchLinkedState(void* aiState) {
    PrepareStaticAiState(aiState);
    int linkedObjectValue = GetLinkedObjectValue_4DB5C0(aiState, nullptr);

    uint32_t holderAddr;
    if (linkedObjectValue != 0) {
        uint32_t entryAddr = static_cast<uint32_t>(linkedObjectValue) - 4;
        holderAddr = CSF_AT(uint32_t, reinterpret_cast<void*>(entryAddr), 0x2C);
    } else {
        // Original reads the holder pointer from absolute address 0x2C (xor eax,eax; mov eax,[eax+0x2c]).
        void* const nullBase = nullptr;
        holderAddr = CSF_AT(uint32_t, nullBase, 0x2C);
    }

    void* target = reinterpret_cast<void*>(
        CSF_AT(uint32_t, reinterpret_cast<void*>(holderAddr), 0x30));

    // vtable slot 0xA0 / 4 = 40, __thiscall with no stack arguments.
    auto dispatch = reinterpret_cast<int (__fastcall*)(void*, void*)>(CSF_AT(void**, target, 0)[40]);
    return dispatch(target, nullptr);
}

HOOK(0x004AD1A0, AiOrder_PrepareAndDispatchLinkedState, "auto");
