#include <cstdint>
#include <cstddef>
#include "hook.h"
#include "types/auto_classes.h"
#include "core/memory.h"
#include "globals.h"

// Global accessor (0x4FDBB0): no arguments, returns the global object pointer in eax.
CSF_FUNC(void*, __cdecl, GetGlobalValue_8B0568, 0x4FDBB0, ());

// Mission-ready probe (0x44FB00): no arguments, result tested through al.
CSF_FUNC(bool, __cdecl, IsMissionReady_44FB00, 0x44FB00, ());

// Returns a sub-object pointer from the global object (0x4FE4A0): __thiscall, no stack args.
CSF_FUNC(void*, __fastcall, GetGlobalSubobject_4FE4A0, 0x4FE4A0, (void* self, void* edx));

// Panel handlers (0x53DFA0 / 0x53DEF0): __thiscall with one stack arg, ret 4.
CSF_FUNC(bool, __fastcall, CMenuMissionInfo_TryHandleSecondaryPanel, 0x53DFA0,
         (CMenuMissionInfo* self, void* edx, int a1));
CSF_FUNC(bool, __fastcall, CMenuMissionInfo_TryHandleTertiaryPanel, 0x53DEF0,
         (CMenuMissionInfo* self, void* edx, int a1));

// Returns non-zero when the current entry index is below the entry count and either the
// global-object conditions hold or one of the two panel handlers accepts the argument.
static int __fastcall CMenuMissionInfo_CheckAdvanceCondition(CMenuMissionInfo* self, void* /*edx*/, int a2)
{
    if (self->field_0090 < self->field_008c) {
        if (IsMissionReady_44FB00()) {
            void* globalObject = GetGlobalValue_8B0568();
            void* subObject = GetGlobalSubobject_4FE4A0(globalObject, nullptr);
            if (subObject != nullptr) {
                globalObject = GetGlobalValue_8B0568();
                subObject = GetGlobalSubobject_4FE4A0(globalObject, nullptr);
                if (CSF_AT(uint32_t, subObject, 0x1C) == 0) {
                    return 1;
                }
            }
        }
    }

    if (CMenuMissionInfo_TryHandleSecondaryPanel(self, nullptr, a2)) {
        return 1;
    }
    if (CMenuMissionInfo_TryHandleTertiaryPanel(self, nullptr, a2)) {
        return 1;
    }
    return 0;
}

HOOK(0x0053E040, CMenuMissionInfo_CheckAdvanceCondition, "auto");
