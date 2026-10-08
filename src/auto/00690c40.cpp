#include <cstdint>
#include "globals.h"
#include "hook.h"

// 0x00690A10 (NetZ): validates a byte value through a switch table; touches neither ECX nor EDX.
CSF_FUNC(char, __cdecl, NetZ_IsValidByteValue, 0x00690A10, (char value));
// 0x00646D30 (NetZ): error/exception report (code 0xE0000012 here).
CSF_FUNC(void, __cdecl, Game_RaiseStatusNotice, 0x00646D30, (int a, int b, uint32_t code, int d));

// 0x00690C40: the callee leaves ECX/EDX alone, so the store goes through the caller's ECX with the argument byte
// (DL): a __thiscall on the destination byte with one stack argument (ret 4).
static char __fastcall StoreValidatedByte(uint8_t* dst, void* /*edx*/, char value)
{
    if (NetZ_IsValidByteValue(value)) {
        *dst = static_cast<uint8_t>(value);
        return 1;
    }
    Game_RaiseStatusNotice(0, 0, 0xE0000012u, 0);
    return 0;
}

HOOK(0x00690C40, StoreValidatedByte, "auto");
