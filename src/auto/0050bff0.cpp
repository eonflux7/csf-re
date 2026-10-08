#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"
#include "globals.h"

// Original 0x0050BFF0:
//   v = GetGlobalInt(); sub_6B1660(v) with v in ecx;
//   if (m_pOverlay) { m_pOverlay->vtbl[0](1); m_pOverlay = 0; }
//   return this->vtbl[2](this);   // tail jmp dword ptr [edx+8], args forwarded
//
// The side effects are done by a C++ __fastcall helper that returns `this` in
// eax. The naked entry loads the helper's address through a C++ static pointer
// (so the compiler resolves the name), calls it, then tail-jumps through the
// vtable slot at +8 with ecx = this and the caller's stack untouched.

CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, GlobalIntApply, 0x006B1660, (void* self, void* edx));

static CMenuScreenFader* __fastcall CMenuScreenFader_ReleaseOverlayAndPrepareDispatch(
    CMenuScreenFader* self, void* /*edx*/) {
    int globalValue = GetGlobalInt();
    GlobalIntApply(reinterpret_cast<void*>(static_cast<uintptr_t>(static_cast<uint32_t>(globalValue))), nullptr);

    void* overlay = reinterpret_cast<void*>(static_cast<uintptr_t>(self->m_pOverlay));
    if (overlay) {
        void** overlayVtbl = *reinterpret_cast<void***>(overlay);
        reinterpret_cast<void(__fastcall*)(void*, void*, int)>(overlayVtbl[0])(overlay, nullptr, 1);
        self->m_pOverlay = 0;
    }
    return self;
}

typedef CMenuScreenFader* (__fastcall FnPrepareDispatch)(CMenuScreenFader*, void*);
static FnPrepareDispatch* const s_pPrepareDispatch =
    CMenuScreenFader_ReleaseOverlayAndPrepareDispatch;

// Naked entry: ecx = this on entry. Calls the helper (stack-neutral, returns
// this in eax), then tail-jumps through this->vtbl[+8] exactly as the original.
static __declspec(naked) void CMenuScreenFader_ReleaseOverlayAndDispatch() {
    __asm {
        mov eax, dword ptr [s_pPrepareDispatch]
        call eax
        mov ecx, eax
        mov edx, dword ptr [ecx]
        jmp dword ptr [edx + 8]
    }
}

HOOK(0x0050BFF0, CMenuScreenFader_ReleaseOverlayAndDispatch, "auto");
