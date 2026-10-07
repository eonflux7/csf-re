#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_GLOBAL(void*, g_pDispatchTable, 0x008B68C0);

static int __stdcall ForwardToCdeclHandler(int a1, int a2, int a3, int a4, int a5)
{
    auto pTable = static_cast<uint8_t*>(g_pDispatchTable);
    auto pFunc = reinterpret_cast<int (__cdecl *)(int, int, int, int, int)>(*reinterpret_cast<void**>(pTable + 0x34));
    return pFunc(a1, a2, a3, a4, a5);
}

HOOK(0x00726770, ForwardToCdeclHandler, "auto");
