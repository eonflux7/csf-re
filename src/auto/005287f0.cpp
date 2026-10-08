#include <cstdint>
#include "hook.h"
#include "core/memory.h"
#include "globals.h"

static __declspec(naked) char __fastcall MenuMessageBox_TailUpdateLastChild(
    void* self, void* /*edx*/)
{
    __asm {
        mov ecx, dword ptr [ecx + 0x10]
        mov eax, dword ptr [ecx]
        jmp dword ptr [eax + 0x134]
    }
}

static char __fastcall MenuMessageBox_UpdateChildren(
    void* self, void* /*edx*/)
{
    char enabled = CSF_AT(char, self, 5);
    if (!enabled)
        return enabled;

    enabled = CSF_AT(char, self, 4);
    if (!enabled)
        return enabled;

    void* child = CSF_AT(void*, self, 8);
    ((void (__fastcall*)(void*, void*))
        CSF_AT(void**, child, 0)[0x134 / 4])(child, nullptr);

    child = CSF_AT(void*, self, 0x0C);
    ((void (__fastcall*)(void*, void*))
        CSF_AT(void**, child, 0)[0x134 / 4])(child, nullptr);

    [[clang::musttail]]
    return MenuMessageBox_TailUpdateLastChild(self, nullptr);
}

HOOK(0x005287F0, MenuMessageBox_UpdateChildren, "auto");
