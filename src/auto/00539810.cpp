// CMenuLoadGame: 0x00539810 (expanded through 0x00539720 dispatch)
// Notifies the child at +0x18 with (this, 0) via vtbl[0x60], then runs the shared
// screen dispatch (originally tail-jumped into). Ends with a tail jump through the
// child's vtable slot 0xa8 on the "juego" exit path.

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

CSF_FUNC(int, __cdecl, GetGlobalValue_8B0568, 0x4FDBB0, ());
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x50D190, ());
CSF_FUNC(int, __fastcall, MenuSystem_ToggleChildFlag, 0x50D300, (int self, void* edx));
CSF_FUNC(int, __fastcall, CMenuMissionInfo_EnterPanelState, 0x53D740, (int a1, void* a2, int a3));

CSF_GLOBAL(const char*, g_pszMenu_P_Campan, 0x899AD8);
CSF_GLOBAL(const char*, g_pszMenu_P_Juego, 0x899AF0);
CSF_GLOBAL(const char*, g_pszMenu_P_MeJueg, 0x899AF4);
CSF_GLOBAL(const char*, g_pszMenu_P_Tactic, 0x899B14);

// Child pointer for the tail-jump thunk (loaded via a static to avoid relying on edi).
static void* s_tailTargetChild;

// Tail-jump thunk: ecx = child, edx = child vtbl, then jmp [edx + 0xa8].
static __declspec(naked) int CMenuLoadGame_TailCallChildVtableA8() {
    __asm {
        mov ecx, dword ptr [s_tailTargetChild]
        mov edx, dword ptr [ecx]
        jmp dword ptr [edx + 0xa8]
    }
}

static int __fastcall CMenuLoadGame_ListboxLoadGames(CMenuLoadGame* self, void* /*edx*/) {
    // 0x00539810 prologue: child->vtbl[0x60](self, 0)
    {
        void* childObject = reinterpret_cast<void*>(static_cast<uintptr_t>(self->field_0018));
        void** childVtable = *reinterpret_cast<void***>(childObject);
        using ChildNotifyFn = void(__fastcall*)(void* c, void* edx, CMenuLoadGame* self, int flag);
        reinterpret_cast<ChildNotifyFn>(childVtable[0x60 / 4])(childObject, nullptr, self, 0);
    }

    // 0x00539720 dispatch: self->vtbl[0x30]() returns byte al
    void** selfVtable = *reinterpret_cast<void***>(self);
    using SelfV30Fn = int(__fastcall*)(CMenuLoadGame* self, void* edx);
    int v30Result = reinterpret_cast<SelfV30Fn>(selfVtable[0x30 / 4])(self, nullptr);
    uint8_t v30Al = static_cast<uint8_t>(v30Result);

    if (v30Al == 0) {
        // 0x005397CE
        int gval = GetGlobalValue_8B0568();
        void* gvalPtr = reinterpret_cast<void*>(static_cast<uintptr_t>(gval));
        uint32_t mode = CSF_AT(uint32_t, gvalPtr, 0x5c);
        void* child = reinterpret_cast<void*>(static_cast<uintptr_t>(self->field_0018));
        void** childVtbl = *reinterpret_cast<void***>(child);
        if (mode != 1) {
            // 0x00539801
            using V58Fn = int(__fastcall*)(void* c, void* edx, const char* psz);
            return reinterpret_cast<V58Fn>(childVtbl[0x58 / 4])(child, nullptr, g_pszMenu_P_Campan);
        }
        // 0x005397DE
        {
            using V58Fn = int(__fastcall*)(void* c, void* edx, const char* psz);
            reinterpret_cast<V58Fn>(childVtbl[0x58 / 4])(child, nullptr, g_pszMenu_P_Juego);
        }
        // 0x005397E8: reload child and call vtbl[0x9c](0)
        child = reinterpret_cast<void*>(static_cast<uintptr_t>(self->field_0018));
        childVtbl = *reinterpret_cast<void***>(child);
        {
            using V9cFn = int(__fastcall*)(void* c, void* edx, int a);
            reinterpret_cast<V9cFn>(childVtbl[0x9c / 4])(child, nullptr, 0);
        }
        // 0x005397F5: reload child once more, tail-jump to vtbl[0xa8]
        child = reinterpret_cast<void*>(static_cast<uintptr_t>(self->field_0018));
        s_tailTargetChild = child;
        return CMenuLoadGame_TailCallChildVtableA8();
    }

    // 0x00539730
    int gval = GetGlobalValue_8B0568();
    void* gvalPtr = reinterpret_cast<void*>(static_cast<uintptr_t>(gval));
    uint32_t mode = CSF_AT(uint32_t, gvalPtr, 0x5c);

    if (mode == 1) {
        // 0x0053973C
        int gstate = GetGlobalState_50D190();
        MenuSystem_ToggleChildFlag(gstate, nullptr);
        gstate = GetGlobalState_50D190();
        void* gstatePtr = reinterpret_cast<void*>(static_cast<uintptr_t>(gstate));
        void* v4 = CSF_AT(void*, gstatePtr, 0x10);
        if (v4 != nullptr) {
            void** v4Vtbl = *reinterpret_cast<void***>(v4);
            using V24Fn = int(__fastcall*)(void* c, void* edx);
            reinterpret_cast<V24Fn>(v4Vtbl[0x24 / 4])(v4, nullptr);
        }
        gstate = GetGlobalState_50D190();
        gstatePtr = reinterpret_cast<void*>(static_cast<uintptr_t>(gstate));
        void* v5 = CSF_AT(void*, gstatePtr, 0x14);
        void** v5Vtbl = *reinterpret_cast<void***>(v5);
        using V5cFn = int(__fastcall*)(void* c, void* edx, const char* psz);
        int v6 = reinterpret_cast<V5cFn>(v5Vtbl[0x5c / 4])(v5, nullptr, g_pszMenu_P_Tactic);
        v5Vtbl = *reinterpret_cast<void***>(v5);
        using V74Fn = int(__fastcall*)(void* c, void* edx, int a, int b);
        int v74Result = reinterpret_cast<V74Fn>(v5Vtbl[0x74 / 4])(v5, nullptr, v6, 0);
        if (static_cast<uint8_t>(v74Result) != 0) {
            // 0x0053977D
            void* child = reinterpret_cast<void*>(static_cast<uintptr_t>(self->field_0018));
            void** childVtbl = *reinterpret_cast<void***>(child);
            using V5cFn2 = int(__fastcall*)(void* c, void* edx, const char* psz);
            int ecxArg = reinterpret_cast<V5cFn2>(childVtbl[0x5c / 4])(child, nullptr, g_pszMenu_P_Tactic);
            return CMenuMissionInfo_EnterPanelState(ecxArg, nullptr, 0);
        }
        return v74Result;
    }

    // 0x00539798
    {
        int gstate = GetGlobalState_50D190();
        void* gstatePtr = reinterpret_cast<void*>(static_cast<uintptr_t>(gstate));
        void* v8 = CSF_AT(void*, gstatePtr, 0x14);
        void** v8Vtbl = *reinterpret_cast<void***>(v8);
        using V5cFn = int(__fastcall*)(void* c, void* edx, const char* psz);
        int v9 = reinterpret_cast<V5cFn>(v8Vtbl[0x5c / 4])(v8, nullptr, g_pszMenu_P_Tactic);
        v8Vtbl = *reinterpret_cast<void***>(v8);
        using V74Fn = int(__fastcall*)(void* c, void* edx, int a, int b);
        int v74Result = reinterpret_cast<V74Fn>(v8Vtbl[0x74 / 4])(v8, nullptr, v9, 0);
        if (static_cast<uint8_t>(v74Result) != 0) return v74Result;
        // 0x005397BC
        void* child = reinterpret_cast<void*>(static_cast<uintptr_t>(self->field_0018));
        void** childVtbl = *reinterpret_cast<void***>(child);
        using V58Fn = int(__fastcall*)(void* c, void* edx, const char* psz);
        return reinterpret_cast<V58Fn>(childVtbl[0x58 / 4])(child, nullptr, g_pszMenu_P_MeJueg);
    }
}

HOOK(0x00539810, CMenuLoadGame_ListboxLoadGames, "auto");
