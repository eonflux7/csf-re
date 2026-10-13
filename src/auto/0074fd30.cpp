#include "hook.h"
#include "globals.h"

// Bound by address: TaskStack::ResetOverflow (__thiscall, no stack args).
CSF_FUNC(void, __fastcall, sub_7538C0, 0x007538C0, (void* self, void* edx));

// void __stdcall (int a1, TaskStack* a2, int a3, int a4, int a5)
// Only a2 is used; it is loaded into ecx and ResetOverflow is called on it.
static void __stdcall NetPostmanDdl_ResetTaskStackOverflow(int a1, void* taskStack, int a3, int a4, int a5)
{
    sub_7538C0(taskStack, nullptr);
}

HOOK(0x0074FD30, NetPostmanDdl_ResetTaskStackOverflow, "auto");
