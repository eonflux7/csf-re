#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static CSF_FUNC(void, __stdcall, PrepareStaticAiState, 0x004DC3C0, (void* a1));
static CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
static CSF_FUNC(C_008319c0*, __fastcall, S_004aef70_SelectUsableTarget, 0x004AEF70, (void* self, void*));

// Vtable of S_004aef70, referenced by the original's call sites (type of the callee's this).
[[maybe_unused]] static void* const s_selectUsableTargetVtbl = CSF_VTBL(0x004AEF70);

static C_008319c0* __stdcall AiAlert_SelectLinkedUsableTarget(void* state)
{
    PrepareStaticAiState(state);
    const int linkedObjectValue = GetLinkedObjectValue_4DB5C0(state, nullptr);
    if (linkedObjectValue)
    {
        // Original: lea ecx, [eax - 4] -> this = linked object at value - 4.
        void* linkedTarget = reinterpret_cast<void*>(
            static_cast<std::uintptr_t>(linkedObjectValue) - 4);
        return S_004aef70_SelectUsableTarget(linkedTarget, nullptr);
    }
    // Original: xor ecx, ecx at 0x4B2185 -> ecx is exactly 0 when this call is made.
    void* zeroThis = nullptr;
    return S_004aef70_SelectUsableTarget(zeroThis, nullptr);
}

HOOK(0x004B2160, AiAlert_SelectLinkedUsableTarget, "auto");
