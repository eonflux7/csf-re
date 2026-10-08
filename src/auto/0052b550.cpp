#include <cstdint>

#include "globals.h"
#include "hook.h"

// Pre-close helper invoked by the close routine (sub_52B020, not yet reimplemented).
CSF_FUNC(int, __fastcall, MenuMessageBox_PreCloseCleanup, 0x0052B020, (void* self, void* edx));

// Forwards a call to the child's vtable slot 16; takes this+0x90 as its single stack argument.
CSF_FUNC(int, __fastcall, C_00720DF0_ForwardCallToChildVtblSlot16, 0x00720DF0, (void* self, void* edx, int a2));

// Calls vtable slot 0xA8 (index 42) of a child object with one stack argument (__thiscall).
static void InvokeChildSetStateSlot(void* child, int stateArgument)
{
    ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, child, 0)[42])(child, nullptr, stateArgument);
}

// Closes the message box: if it is open and has a primary child, pre-close cleanup runs,
// child states are forwarded, the open flag is cleared, and two pending values are
// pushed into the object returned by vtable slot 0x110 of the primary child.
static char __fastcall MenuMessageBox_CloseAndReportResult(void* self, void* /*edx*/)
{
    if (CSF_AT(uint8_t, self, 0x80) == 0)
        return 0;

    if (CSF_AT(void*, self, 0x58) == nullptr)
        return 0;

    MenuMessageBox_PreCloseCleanup(self, nullptr);

    if (void* childAt5C = CSF_AT(void*, self, 0x5c))
        InvokeChildSetStateSlot(childAt5C, CSF_AT(uint8_t, self, 0x8b8) == 0 ? 1 : 0);

    if (void* childAt68 = CSF_AT(void*, self, 0x68))
        InvokeChildSetStateSlot(childAt68, CSF_AT(uint8_t, self, 0x8b8) == 0 ? 1 : 0);

    if (void* childAt6C = CSF_AT(void*, self, 0x6c))
        InvokeChildSetStateSlot(childAt6C, CSF_AT(uint8_t, self, 0x8b9) == 0 ? 1 : 0);

    if (void* childAt70 = CSF_AT(void*, self, 0x70))
        InvokeChildSetStateSlot(childAt70, CSF_AT(uint8_t, self, 0x8b9) == 0 ? 1 : 0);

    if (void* childAt74 = CSF_AT(void*, self, 0x74))
        InvokeChildSetStateSlot(childAt74, CSF_AT(uint8_t, self, 0x8b9) == 0 ? 1 : 0);

    C_00720DF0_ForwardCallToChildVtblSlot16(
        CSF_AT(void*, self, 0x58),
        nullptr,
        static_cast<int>(reinterpret_cast<uintptr_t>(static_cast<uint8_t*>(self) + 0x90)));

    InvokeChildSetStateSlot(CSF_AT(void*, self, 0x58), 0);

    void* owner = CSF_AT(void*, self, 0x58);
    CSF_AT(uint8_t, self, 0x80) = 0;

    // Vtable slot 0x110 (index 68) of the primary child returns the result target object.
    void* reportTarget =
        ((void*(__fastcall*)(void*, void*))CSF_AT(void**, owner, 0)[68])(owner, nullptr);

    // Vtable slot 0x68 (index 26) with pending value A.
    ((void(__fastcall*)(void*, void*, uint32_t))CSF_AT(void**, reportTarget, 0)[26])(
        reportTarget, nullptr, CSF_AT(uint32_t, self, 0x8a0));

    // Vtable slot 0x6C (index 27) with pending value B; its low byte is the return value.
    int reportResult =
        ((int(__fastcall*)(void*, void*, uint32_t))CSF_AT(void**, reportTarget, 0)[27])(
            reportTarget, nullptr, CSF_AT(uint32_t, self, 0x8a4));

    return static_cast<char>(reportResult);
}

HOOK(0x0052B550, MenuMessageBox_CloseAndReportResult, "auto");
