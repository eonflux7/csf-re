#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, CoverAttack_SetState_4DC150, 0x004DC150, (void* self, void* edx, int a2));

// Stdcall, 3 args (ret 12). For state id 19, resets the object linked to the
// AI state (vtable slot 0x8C / 4 = 35, args 0, 0, -1, 0), then forwards to
// CoverAttack_SetState_4DC150(state, 0) in all cases.
static int __stdcall AiMoveTasks_ApplyStateWithLinkedReset(TAiState* a1, int a2, int a3)
{
    if (a2 == 19) {
        int linkedValue = GetLinkedObjectValue_4DB5C0(a1, nullptr);
        int v4;
        if (linkedValue) {
            v4 = linkedValue - 4;
        } else {
            v4 = 0;
        }
        uint8_t* subBase = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(v4));
        void* obj = CSF_AT(void*, subBase, 0x2C);
        void** vtbl = CSF_AT(void**, obj, 0);
        ((void(__fastcall*)(void*, void*, int, int, int, int))vtbl[0x8C / 4])(obj, nullptr, 0, 0, -1, 0);
    }
    return CoverAttack_SetState_4DC150(a1, nullptr, 0);
}

HOOK(0x004AAA20, AiMoveTasks_ApplyStateWithLinkedReset, "auto");
