#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CGuiListBoxBase_ProcessHeaderAndColumnEntries, 0x714DA0,
         (CGuiListBox* self, void* edx));

static __declspec(naked) int __fastcall CGuiListBox_TailDispatchRowText(
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
        jmp dword ptr [eax + 0x34]
    }
}

static int __fastcall CGuiListBox_DrawRowText(
    CGuiListBox* self, void* /*edx*/)
{
    CGuiListBoxBase_ProcessHeaderAndColumnEntries(self, nullptr);
    [[clang::musttail]] return CGuiListBox_TailDispatchRowText(self, nullptr);
}

HOOK(0x722C40, CGuiListBox_DrawRowText, "auto");
