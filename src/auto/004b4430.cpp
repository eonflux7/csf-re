#include <cstdint>
#include "globals.h"
#include "hook.h"

static CSF_FUNC(void, __fastcall, AiDistracted_UpdateLinkedState, 0x004B3430, (void* self, void* edx));
static CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
static CSF_FUNC(void, __stdcall, PrepareStaticAiState, 0x004DC3C0, (void* a1));

static void __stdcall AiAlert_UpdateDistractedLinkedState(void* state)
{
    PrepareStaticAiState(state);
    const int linkedObjectValue = GetLinkedObjectValue_4DB5C0(state, nullptr);
    void* linkedState = linkedObjectValue
        ? reinterpret_cast<void*>(static_cast<std::uintptr_t>(linkedObjectValue) - 4)
        : nullptr;
    AiDistracted_UpdateLinkedState(linkedState, nullptr);
}

HOOK(0x004B4430, AiAlert_UpdateDistractedLinkedState, "auto");
