#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(void, __stdcall, PrepareStaticAiState, 0x004DC3C0, (void* a1));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, AttackTasks_SetMode, 0x0047B210, (void* self, void*, int mode));

// Returns (linked object value - 4), or 0 when the linked object value is 0.
static uint32_t GetLinkedObjectBaseOrZero(void* a1) {
    uint32_t linkedValue = static_cast<uint32_t>(GetLinkedObjectValue_4DB5C0(a1, nullptr));
    return linkedValue ? linkedValue - 4 : 0;
}

static int __stdcall AiState_ResetAttackModeIfActive(void* a1) {
    PrepareStaticAiState(a1);

    uint32_t base = GetLinkedObjectBaseOrZero(a1);
    uint32_t state = CSF_AT(uint32_t, reinterpret_cast<uint8_t*>(base), 0x2C);

    if (CSF_AT(int, reinterpret_cast<uint8_t*>(state), 0x1D8) != 3) {
        return static_cast<int>(state);
    }

    uint32_t modeBase = GetLinkedObjectBaseOrZero(a1);
    uint32_t modeOwner = CSF_AT(uint32_t, reinterpret_cast<uint8_t*>(modeBase), 0x718);
    AttackTasks_SetMode(reinterpret_cast<void*>(modeOwner), nullptr, 2);

    uint32_t linkedRaw = static_cast<uint32_t>(GetLinkedObjectValue_4DB5C0(a1, nullptr));
    if (linkedRaw) {
        uint32_t objBase = linkedRaw - 4;
        void* subObject = reinterpret_cast<void*>(
            CSF_AT(uint32_t, reinterpret_cast<uint8_t*>(objBase), 0x714));
        return ((int(__fastcall*)(void*, void*))CSF_AT(void**, subObject, 0)[160 / 4])(subObject, nullptr);
    }

    uint8_t* nullBase = nullptr;
    void* globalObject = reinterpret_cast<void*>(CSF_AT(uint32_t, nullBase, 0x718));
    return ((int(__fastcall*)(void*, void*))CSF_AT(void**, globalObject, 0)[160 / 4])(globalObject, nullptr);
}

HOOK(0x004AD430, AiState_ResetAttackModeIfActive, "auto");
