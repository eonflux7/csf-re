#include <cstdint>
#include "globals.h"
#include "hook.h"

// ===========================================================================
// Commandos: Strike Force - CMenuControls activation handler
//
// Entry at 0x00543A80 (23 bytes) gates on  CMenuControls::field_0134 == 1 and
// tail-jumps into the shared body at 0x005389A0 with
//     ctx = *(std::uint32_t*)( GetGlobalState() + 0x1C )
// That body drives a chain of virtual calls on the current UI objects and
// finally dispatches vtable slot 0x78 on a freshly resolved object.
// Both parts are reproduced here as a single reimplemented function.
// ===========================================================================

// 0x0050D190  int __cdecl GetGlobalState()
CSF_FUNC(int, __cdecl, MenuControls_GetGlobalState, 0x50D190, ());

// 0x006B1380  int __cdecl GetGlobalInt()
CSF_FUNC(int, __cdecl, MenuControls_GetGlobalInt, 0x6B1380, ());

// 0x006B1390  int __thiscall sub_6B1390(std::uint32_t* this)
CSF_FUNC(int, __fastcall, MenuControls_ResolveObject, 0x6B1390, (void* self, void* edx));

// 0x00538870  char __thiscall sub_538870(int this)
CSF_FUNC(char, __fastcall, MenuControls_ResetObject, 0x538870, (void* self, void* edx));

// Tail dispatch to vtable slot 0x78 of the object received in ecx.
// Original epilogue (0x00538A39):
//     mov edx, dword ptr [eax]
//     mov ecx, eax
//     jmp dword ptr [edx + 0x78]
static __declspec(naked) void __fastcall MenuControls_DispatchVirtual78(void* /*object*/)
{
    __asm
    {
        mov eax, ecx
        mov edx, dword ptr [eax]
        mov ecx, eax
        jmp dword ptr [edx + 0x78]
    }
}

static void __fastcall CMenuControls_ProcessActivation(void* self, void* /*edx*/)
{
    // 0x00543A80: cmp dword ptr [ecx+0x134], 1 ; jne -> ret
    if (CSF_AT(int, self, 0x134) != 1)
        return;

    // 0x00543A89: GetGlobalState(); ctx = [eax+0x1C]
    void* ctx = CSF_AT(void*, reinterpret_cast<void*>(MenuControls_GetGlobalState()), 0x1C);

    // 0x005389A3: if (ctx->field_003C != 0) { ... }
    if (CSF_AT(void*, ctx, 0x3C))
    {
        void* selector = reinterpret_cast<void*>(
            MenuControls_ResolveObject(reinterpret_cast<void*>(MenuControls_GetGlobalInt()), nullptr));

        // 0x005389BA: call [edx+0x6C] ; test al, al
        if (((unsigned char (__fastcall*)(void*, void*))CSF_AT(void**, selector, 0)[0x6C / 4])(selector, nullptr))
        {
            // 0x005389C1: reload ctx->field_003C
            void* target = CSF_AT(void*, ctx, 0x3C);
            // 0x005389C6: call [eax+0x7C]
            ((void (__fastcall*)(void*, void*))CSF_AT(void**, target, 0)[0x7C / 4])(target, nullptr);

            void* helper = reinterpret_cast<void*>(
                MenuControls_ResolveObject(reinterpret_cast<void*>(MenuControls_GetGlobalInt()), nullptr));
            // 0x005389D9: call [edx+0x70]
            ((void (__fastcall*)(void*, void*))CSF_AT(void**, helper, 0)[0x70 / 4])(helper, nullptr);
        }
    }

    // 0x005389DC: trigger = ResolveObject(GetGlobalInt()); call [edx+0x74] (arg 0)
    void* trigger = reinterpret_cast<void*>(
        MenuControls_ResolveObject(reinterpret_cast<void*>(MenuControls_GetGlobalInt()), nullptr));

    if (!((unsigned char (__fastcall*)(void*, void*, int))CSF_AT(void**, trigger, 0)[0x74 / 4])(trigger, nullptr, 0))
        return;

    // 0x005389F5: if (ctx->field_0040 != 0)
    void* primary = CSF_AT(void*, ctx, 0x40);
    if (primary)
    {
        // 0x00538A00: call [eax+0xDC] ; test eax, eax
        if (((int (__fastcall*)(void*, void*))CSF_AT(void**, primary, 0)[0xDC / 4])(primary, nullptr))
        {
            // 0x00538A0A: reload ctx->field_0040
            void* primaryReloaded = CSF_AT(void*, ctx, 0x40);
            // 0x00538A0F: call [edx+0x134]
            ((void (__fastcall*)(void*, void*))CSF_AT(void**, primaryReloaded, 0)[0x134 / 4])(primaryReloaded, nullptr);
        }
    }

    // 0x00538A15: if (ctx->field_002C != 0) call [eax+0x134]
    void* secondary = CSF_AT(void*, ctx, 0x2C);
    if (secondary)
    {
        ((void (__fastcall*)(void*, void*))CSF_AT(void**, secondary, 0)[0x134 / 4])(secondary, nullptr);
    }

    // 0x00538A26: sub_538870(ctx)
    MenuControls_ResetObject(ctx, nullptr);

    // 0x00538A2D: final = ResolveObject(GetGlobalInt()); tail dispatch [edx+0x78]
    void* final = reinterpret_cast<void*>(
        MenuControls_ResolveObject(reinterpret_cast<void*>(MenuControls_GetGlobalInt()), nullptr));
    MenuControls_DispatchVirtual78(final);
}

HOOK(0x543A80, CMenuControls_ProcessActivation, "auto");
