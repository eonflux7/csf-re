#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_FUNC(void, __stdcall, PrepareStaticAiState, 0x004DC3C0, (void* a1));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(char, __fastcall, AiAttackTasks_AddToActionFlag, 0x0048C9D0, (uint8_t* self, void* , char delta));

static int __stdcall AiOrder_ProcessStaticAiState(void* a1) {
    PrepareStaticAiState(a1);

    // First linked-object lookup: its result (minus 4) selects the owner whose 0x204 field
    // is passed as `this` to AiAttackTasks_AddToActionFlag. A null result uses address 0.
    const uintptr_t firstLinked = static_cast<uintptr_t>(GetLinkedObjectValue_4DB5C0(a1, nullptr));
    const uintptr_t firstBase = firstLinked ? firstLinked - 4 : 0;
    void* actionOwner = CSF_AT(void*, reinterpret_cast<void*>(firstBase), 0x2C);
    uint8_t* actionSelf = CSF_AT(uint8_t*, actionOwner, 0x204);
    AiAttackTasks_AddToActionFlag(actionSelf, nullptr, -1);

    // Second lookup decides which object receives the virtual call.
    const uintptr_t secondLinked = static_cast<uintptr_t>(GetLinkedObjectValue_4DB5C0(a1, nullptr));
    if (secondLinked) {
        void* linkedOwner = CSF_AT(void*, reinterpret_cast<void*>(secondLinked), 0x28);
        void** linkedVtbl = CSF_AT(void**, linkedOwner, 0);
        return ((int(__fastcall*)(void*, void*, int))linkedVtbl[0xB4 / 4])(linkedOwner, nullptr, 0);
    }

    // Fallback: the original reads the owner pointer from absolute address 0x2C (this = null base).
    void* fallbackOwner = CSF_AT(void*, reinterpret_cast<void*>(uintptr_t{0}), 0x2C);
    void** fallbackVtbl = CSF_AT(void**, fallbackOwner, 0);
    return ((int(__fastcall*)(void*, void*, int))fallbackVtbl[0xB4 / 4])(fallbackOwner, nullptr, 0);
}

HOOK(0x004ACAE0, AiOrder_ProcessStaticAiState, "auto");
