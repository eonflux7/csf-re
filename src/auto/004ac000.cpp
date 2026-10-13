#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(void, __stdcall, PrepareStaticAiState, 0x004DC3C0, (void* a1));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));

// Forwards the AI state to vtable slot 0x30 of the object linked to it.
// If no object is linked, the same slot is called on the fallback object:
// its "this" is the address 0x258 and its vtable pointer is read from 0x258.
static int __stdcall DispatchStaticAiStateToLinkedObject(TAiState* a1)
{
    PrepareStaticAiState(a1);
    int linkedObjectValue = GetLinkedObjectValue_4DB5C0(a1, nullptr);
    if (linkedObjectValue != 0)
    {
        void* linkedSubObject = reinterpret_cast<void*>(
            static_cast<uintptr_t>(static_cast<uint32_t>(linkedObjectValue) - 4 + 0x258));
        void** linkedVtable = CSF_AT(void**, linkedSubObject, 0);
        return ((int(__fastcall*)(void*, void*, TAiState*))(linkedVtable[0x30 / 4]))(
            linkedSubObject, nullptr, a1);
    }

    // Fallback: ecx = 0x258 (a literal, not a dereference), then the vtable is
    // loaded from the memory at that address.
    void* fallbackBase = nullptr;
    void* fallbackThis = static_cast<uint8_t*>(fallbackBase) + 0x258;
    void** fallbackVtable = CSF_AT(void**, fallbackBase, 0x258);
    return ((int(__fastcall*)(void*, void*, TAiState*))(fallbackVtable[0x30 / 4]))(
        fallbackThis, nullptr, a1);
}

HOOK(0x4AC000, DispatchStaticAiStateToLinkedObject, "auto");
