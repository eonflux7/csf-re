#include <cstdint>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Original callees, bound by address.
CSF_FUNC(int, __cdecl, GetGlobalValue_8B0568, 0x004FDBB0, ());
CSF_FUNC(void, __fastcall, GlobalMenu_SetSelectedScenarioName, 0x00501990,
         (void* self, void* edx, const char* name, char flag));
CSF_FUNC(int, __cdecl, GetGlobalValue, 0x00521BB0, ());
CSF_FUNC(int, __fastcall, GlobalMenu_BeginSelectionUpdate, 0x00521E80, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GlobalMenu_EndSelectionUpdate, 0x00521F10, (void* self, void* edx));

// Original data.
CSF_GLOBAL(const char*, g_pszMenu_P_Juego, 0x00899AF0);

// Set by the work function when the original would take the indirect tail jump.
static int g_applySelectionTailPending = 0;

// Body of CMenuScenarioSelect::ApplySelection without the final indirect tail jump.
// Returns eax of the non-tail path, or the object whose vtable slot 0xA8 is the tail
// target (with g_applySelectionTailPending set to 1).
static int __fastcall CMenuScenarioSelect_ApplySelectionWork(CMenuScenarioSelect* self, void* /*edx*/) {
    g_applySelectionTailPending = 0;

    // Name string: inline at +0x20 when the length at +0x34 is below 0x10, otherwise a pointer at +0x20.
    const char* selectedName;
    if (self->field_0034 < 0x10u) {
        selectedName = reinterpret_cast<const char*>(reinterpret_cast<uint8_t*>(self) + 0x20);
    } else {
        selectedName = reinterpret_cast<const char*>(self->field_0020);
    }

    int globalObjA = GetGlobalValue_8B0568();
    GlobalMenu_SetSelectedScenarioName(reinterpret_cast<void*>(globalObjA), nullptr, selectedName, 1);

    int globalResult = GetGlobalValue_8B0568();
    void* globalValue = reinterpret_cast<void*>(globalResult);
    if (CSF_AT(uint32_t, globalValue, 0x5C) != 1u) {
        return globalResult;
    }

    int contextA = GetGlobalValue();
    GlobalMenu_BeginSelectionUpdate(reinterpret_cast<void*>(contextA), nullptr);

    // vtable slot 0x60: obj->Method(self, 1)
    {
        void* obj = reinterpret_cast<void*>(self->field_0014);
        void** vtbl = *reinterpret_cast<void***>(obj);
        ((void(__fastcall*)(void*, void*, void*, int))vtbl[0x60 / 4])(obj, nullptr, self, 1);
    }

    int contextB = GetGlobalValue();
    GlobalMenu_EndSelectionUpdate(reinterpret_cast<void*>(contextB), nullptr);

    // vtable slot 0x58: obj->Method(g_pszMenu_P_Juego)
    {
        void* obj = reinterpret_cast<void*>(self->field_0014);
        void** vtbl = *reinterpret_cast<void***>(obj);
        ((void(__fastcall*)(void*, void*, const char*))vtbl[0x58 / 4])(obj, nullptr, g_pszMenu_P_Juego);
    }

    // vtable slot 0x9C: obj->Method(0)
    {
        void* obj = reinterpret_cast<void*>(self->field_0014);
        void** vtbl = *reinterpret_cast<void***>(obj);
        ((void(__fastcall*)(void*, void*, int))vtbl[0x9C / 4])(obj, nullptr, 0);
    }

    // Tail path: the naked thunk jumps through vtable slot 0xA8 of the returned object.
    g_applySelectionTailPending = 1;
    return static_cast<int>(self->field_0014);
}

// Naked entry (ecx = this, __thiscall, ret 0): runs the C++ body, then either returns
// its eax or tail-jumps through [obj->vtbl + 0xA8] with the caller's stack untouched.
static __declspec(naked) void CMenuScenarioSelect_ApplySelection() {
    __asm {
        lea eax, [CMenuScenarioSelect_ApplySelectionWork]
        call eax
        cmp dword ptr [g_applySelectionTailPending], 0
        je L_return_result
        mov ecx, eax
        mov eax, dword ptr [ecx]
        jmp dword ptr [eax + 0xa8]
    L_return_result:
        ret
    }
}

HOOK(0x0052D940, CMenuScenarioSelect_ApplySelection, "auto");
