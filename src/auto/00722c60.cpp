#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// 0x714E10: base-class helper invoked before dispatching to the row text object.
CSF_FUNC(int, __fastcall, CGuiListBoxBase_ReleaseHeaderAndColumnTexts, 0x714E10,
         (CGuiListBox* self, void* edx));

// Tail-dispatches to the row text object's vtable slot +0x38 (slot 14).
// emulates:  mov eax,[ecx+0x218]; test eax,eax; je done;
//            mov ecx,eax; mov eax,[ecx]; jmp [eax+0x38] ; done: ret
static __declspec(naked) int __fastcall CGuiListBox_TailDispatchRowTextSlot14(
    CGuiListBox* self, void* edx)
{
    __asm
    {
        mov eax, dword ptr [ecx + 0x218]
        test eax, eax
        jne dispatchRowText
        ret
    dispatchRowText:
        mov ecx, eax
        mov eax, dword ptr [ecx]
        jmp dword ptr [eax + 0x38]
    }
}

static int __fastcall CGuiListBox_RenderRowText(
    CGuiListBox* self, void* /*edx*/)
{
    CGuiListBoxBase_ReleaseHeaderAndColumnTexts(self, nullptr);
    [[clang::musttail]] return CGuiListBox_TailDispatchRowTextSlot14(self, nullptr);
}

HOOK(0x722C60, CGuiListBox_RenderRowText, "auto");
