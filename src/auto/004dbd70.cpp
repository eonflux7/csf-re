#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Plain stdcall helper: never reads ecx, so no `this` parameter.
// Reads a1->field_0008 (child query object). If set, it asks a1->field_0010's
// virtual slot 7 for a value, then calls the child's virtual slot 32 with
// (child, a1->field_0008, value). Returns the child call's result, or 0.
static int __stdcall AiState_ForwardQueryToChild(int a1, int a2, int a3) {
    (void)a2;
    auto* self = reinterpret_cast<uint8_t*>(a1);

    int result = CSF_AT(int, self, 0x08);
    if (result) {
        auto* child = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, reinterpret_cast<uint8_t*>(result), 0x10));
        void** childVtbl = CSF_AT(void**, child, 0);

        // Value comes from the object at a1->field_0010, slot 0x1C / 4 = 7.
        auto* valueSource = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, self, 0x10));
        void** valueSourceVtbl = CSF_AT(void**, valueSource, 0);
        int queriedValue = reinterpret_cast<int(__fastcall*)(void*, void*, int)>(valueSourceVtbl[0x1C / 4])(
            valueSource, nullptr, a3);

        // Re-read a1->field_0008 after the call, as the original does.
        // Child slot 0x80 / 4 = 32, arguments (child, a1->field_0008, queriedValue).
        return reinterpret_cast<int(__fastcall*)(void*, void*, int, int)>(childVtbl[0x80 / 4])(
            child, nullptr, CSF_AT(int, self, 0x08), queriedValue);
    }
    return result;
}

HOOK(0x4DBD70, AiState_ForwardQueryToChild, "auto");
