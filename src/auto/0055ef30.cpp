#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_FUNC(void*, __cdecl, GetGlobalState, 0x006A6D70, ());

CSF_GLOBAL(const char*, g_pszMenu_P_MsgBox, 0x00899B28);
CSF_GLOBAL(const char*, g_pszMenu_P_LbTpQM, 0x00899B4C);
CSF_GLOBAL(const char*, g_pszMenu_P_LbCMch, 0x00899B50);
CSF_GLOBAL(const char*, g_pszMenu_P_LbOMch, 0x00899B54);
CSF_GLOBAL(const char*, g_pszMenu_P_LbFrie, 0x00899B58);

using VirtualNoArgsFn = uint32_t(__fastcall*)(void*, void*);
using VirtualNameFn = uint32_t(__fastcall*)(void*, void*, const char*);
using VirtualIntFn = uint32_t(__fastcall*)(void*, void*, int);

static void* AsObject(uint32_t value)
{
    return reinterpret_cast<void*>(static_cast<uintptr_t>(value));
}

static uint32_t CallVirtualNoArgs(void* object, uint32_t byteOffset)
{
    return reinterpret_cast<VirtualNoArgsFn>(
        CSF_AT(void**, object, 0)[byteOffset / 4])(object, nullptr);
}

static uint32_t CallVirtualWithName(void* object, uint32_t byteOffset, const char* name)
{
    return reinterpret_cast<VirtualNameFn>(
        CSF_AT(void**, object, 0)[byteOffset / 4])(object, nullptr, name);
}

static uint32_t CallVirtualWithInt(void* object, uint32_t byteOffset, int argument)
{
    return reinterpret_cast<VirtualIntFn>(
        CSF_AT(void**, object, 0)[byteOffset / 4])(object, nullptr, argument);
}

static int __cdecl MenuMpOnline_IsOnlineGateOpen()
{
    void* globalState = GetGlobalState();
    return static_cast<int>(CallVirtualNoArgs(globalState, 0x1C));
}

static uint32_t __fastcall CMenuMpOnline_ProcessPendingDialogsBody(
    void* self, void* /*edx*/)
{
    uint32_t result = CSF_AT(uint32_t, self, 0x4C);

    if (result == 0)
    {
        result = CallVirtualWithName(
            CSF_AT(void*, self, 0x3C), 0x5C, g_pszMenu_P_MsgBox);
        CSF_AT(uint32_t, self, 0x4C) = result;
        result = CallVirtualNoArgs(AsObject(result), 0x20);
    }

    if (CSF_AT(uint8_t, self, 0x34) != 0)
    {
        result = CallVirtualWithName(
            CSF_AT(void*, self, 0x3C), 0x58, g_pszMenu_P_LbTpQM);
        CSF_AT(uint8_t, self, 0x34) = 0;
    }
    else if (CSF_AT(uint8_t, self, 0x35) != 0)
    {
        result = CallVirtualWithName(
            CSF_AT(void*, self, 0x3C), 0x58, g_pszMenu_P_LbOMch);
        CSF_AT(uint8_t, self, 0x35) = 0;
    }
    else if (CSF_AT(uint8_t, self, 0x36) != 0)
    {
        result = CallVirtualWithName(
            CSF_AT(void*, self, 0x3C), 0x58, g_pszMenu_P_LbCMch);
        CSF_AT(uint8_t, self, 0x36) = 0;
    }
    else
    {
        if (CSF_AT(uint8_t, self, 0x37) == 0)
            return result;

        result = CallVirtualWithName(
            CSF_AT(void*, self, 0x3C), 0x5C, g_pszMenu_P_LbFrie);
        if (result != 0)
        {
            CSF_AT(uint8_t, AsObject(result), 0x188) = 0;
            result = CallVirtualWithName(
                CSF_AT(void*, self, 0x3C), 0x58, g_pszMenu_P_LbFrie);
        }
        CSF_AT(uint8_t, self, 0x37) = 0;
    }

    void* stateChild = CSF_AT(void*, self, 0x14);
    if (stateChild == nullptr)
        return result;

    CallVirtualNoArgs(stateChild, 0x16C);
    return CallVirtualWithInt(CSF_AT(void*, self, 0x14), 0xA8, 1);
}

// This eight-byte POD is returned in EDX:EAX by the x86 MSVC ABI.
struct MenuMpOnlineStepOutcome
{
    uint32_t returnValue;
    void* tailSelf;
};

static_assert(sizeof(MenuMpOnlineStepOutcome) == 8);

static MenuMpOnlineStepOutcome __fastcall CMenuMpOnline_ProcessPendingDialogsStep(
    void* self, void* /*edx*/)
{
    if (MenuMpOnline_IsOnlineGateOpen() == 0)
        return {0, self};

    return {CMenuMpOnline_ProcessPendingDialogsBody(self, nullptr), nullptr};
}

// Taking the address in C++ lets the compiler resolve the decorated function symbol.
static MenuMpOnlineStepOutcome (__fastcall* s_ProcessPendingDialogsStep)(void*, void*) =
    &CMenuMpOnline_ProcessPendingDialogsStep;

// The helper leaves the normal result in EAX and a tail-dispatch receiver in EDX.
// The indirect jump retains the original caller's return address and stack.
static __declspec(naked) int __fastcall CMenuMpOnline_ProcessPendingDialogs(
    void* /*self*/, void* /*edx*/)
{
    __asm {
        mov eax, dword ptr [s_ProcessPendingDialogsStep]
        call eax
        test edx, edx
        jne TailJumpToVirtualHandler
        ret
    TailJumpToVirtualHandler:
        mov ecx, edx
        mov eax, dword ptr [ecx]
        jmp dword ptr [eax+0x5c]
    }
}

HOOK(0x0055EF30, CMenuMpOnline_ProcessPendingDialogs, "auto");
