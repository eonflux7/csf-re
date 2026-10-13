#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(void, __stdcall, PrepareStaticAiState, 0x004DC3C0, (void* a1));
CSF_FUNC(int, __fastcall, ResolveEntryLookupValue, 0x00471540, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, GetLinkedObjectField10, 0x004DB5D0, (void* self, void*));

static int __stdcall TOrden_RunOrderStep(C_0085e760* a1)
{
    PrepareStaticAiState(a1);

    int linked = GetLinkedObjectValue_4DB5C0(a1, nullptr);
    int v2 = linked ? linked - 4 : 0;

    // owner = *(v2 + 0x2c); inner = *(owner + 0x30); inner->vtbl[40]()
    void* base = reinterpret_cast<void*>(static_cast<uintptr_t>(v2));
    void* owner = CSF_AT(void*, base, 0x2c);
    void* inner = CSF_AT(void*, owner, 0x30);
    ((void(__fastcall*)(void*, void*))CSF_AT(void**, inner, 0)[40])(inner, nullptr);

    int v3 = ResolveEntryLookupValue(a1, nullptr);
    int result = GetLinkedObjectField10(a1, nullptr);

    void* resultPtr = reinterpret_cast<void*>(static_cast<uintptr_t>(result));
    if (CSF_AT(uint8_t, resultPtr, 0xB0)) {
        int v5 = GetLinkedObjectValue_4DB5C0(reinterpret_cast<void*>(static_cast<uintptr_t>(v3)), nullptr);
        if (v5) {
            void* target = reinterpret_cast<void*>(static_cast<uintptr_t>(v5));
            void* child = CSF_AT(void*, target, 0x714);
            return ((int(__fastcall*)(void*, void*))CSF_AT(void**, child, 0)[40])(child, nullptr);
        }
        // Original: xor eax,eax; mov ecx,[eax+0x718] -> reads the global pointer stored at absolute address 0x718.
        void* nullBase = reinterpret_cast<void*>(static_cast<uintptr_t>(0));
        void* fallback = CSF_AT(void*, nullBase, 0x718);
        return ((int(__fastcall*)(void*, void*))CSF_AT(void**, fallback, 0)[40])(fallback, nullptr);
    }
    return result;
}

HOOK(0x004AD040, TOrden_RunOrderStep, "auto");
