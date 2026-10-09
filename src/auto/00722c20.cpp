#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CGuiListBoxBase_ProcessHeaderAndColumnText, 0x00714D30, (CGuiListBoxBase* self, void*));

enum {
    RowTextOffset = offsetof(CGuiListBox, m_pRowText)
};

static __declspec(naked) int __fastcall
CGuiListBox_TailProcessRowText(CGuiListBox* self, void* /*edx*/)
{
    __asm {
        mov eax, dword ptr [ecx + RowTextOffset]
        test eax, eax
        jne ProcessRowText
        ret

    ProcessRowText:
        mov ecx, eax
        mov eax, dword ptr [ecx]
        jmp dword ptr [eax + 0x1c]
    }
}

static int __fastcall
CGuiListBox_ProcessHeaderColumnsAndRowText(CGuiListBox* self, void* /*edx*/)
{
    CGuiListBoxBase_ProcessHeaderAndColumnText(self, nullptr);
    [[clang::musttail]] return CGuiListBox_TailProcessRowText(self, nullptr);
}

HOOK(0x00722C20, CGuiListBox_ProcessHeaderColumnsAndRowText, "auto");
