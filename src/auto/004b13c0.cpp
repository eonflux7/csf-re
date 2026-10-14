#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Bound callees (addresses from the packet).
CSF_FUNC(void, __stdcall, PrepareStaticAiState, 0x004DC3C0, (void* a1));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));

typedef char(__fastcall* ObjectCharSlot40Fn)(void* self, void* edx);

static char __fastcall TFncMacroOrden_ConsumePendingDispatch(TFncMacroOrden* self, void* /*edx*/, int a2) {
    PrepareStaticAiState(reinterpret_cast<void*>(a2));

    char result = CSF_AT(char, self, 0xB8);
    if (result) {
        CSF_AT(char, self, 0xB8) = 0;

        int linked = GetLinkedObjectValue_4DB5C0(reinterpret_cast<void*>(a2), nullptr);
        if (linked) {
            void* linkedObj = reinterpret_cast<void*>(linked);
            void* innerObj = CSF_AT(void*, linkedObj, 0x714);
            ObjectCharSlot40Fn fn = reinterpret_cast<ObjectCharSlot40Fn>(CSF_AT(void**, innerObj, 0)[40]);
            return fn(innerObj, nullptr);
        } else {
            // EAX is zero on this original path: a null-object field access,
            // not an image global. Preserve the fault, do not fabricate storage.
            void* fallbackObj = CSF_AT(void*, reinterpret_cast<void*>(linked), 0x718);
            ObjectCharSlot40Fn fn = reinterpret_cast<ObjectCharSlot40Fn>(CSF_AT(void**, fallbackObj, 0)[40]);
            return fn(fallbackObj, nullptr);
        }
    }
    return result;
}

HOOK(0x004B13C0, TFncMacroOrden_ConsumePendingDispatch, "auto");
