#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(void, __stdcall, PrepareStaticAiState, 0x004DC3C0, (void* a1));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, AttackTasks_SetMode, 0x0047B210, (void* self, void*, int mode));

// Prepares the static AI state, resolves the linked object and switches the
// attack task to mode 2. The linked object's field 0x714 (or the absolute
// slot 0x718 when no linked object exists) is passed as `this`.
static int __stdcall AiCover_PrepareAndSetAttackMode(void* a1)
{
    PrepareStaticAiState(a1);

    const uint32_t linkedValue = static_cast<uint32_t>(GetLinkedObjectValue_4DB5C0(a1, nullptr));
    void* taskOwner = nullptr;

    if (linkedValue != 0) {
        void* linkedObject = reinterpret_cast<void*>(static_cast<uintptr_t>(linkedValue));
        taskOwner = CSF_AT(void*, linkedObject, 0x714);
    } else {
        // The original does `xor eax,eax` and loads [0x718] from a null base.
        void* nullBase = nullptr;
        taskOwner = CSF_AT(void*, nullBase, 0x718);
    }

    return AttackTasks_SetMode(taskOwner, nullptr, 2);
}

HOOK(0x004AC040, AiCover_PrepareAndSetAttackMode, "auto");
