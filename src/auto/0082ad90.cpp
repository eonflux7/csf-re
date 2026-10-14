// Includes the game-owned shared destructor chunk; no missing address slot.
// Shape: global destructor tail. Original bytes: b9b4ae8b00e9d635e7ff
#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"
#include "legacy/string.h"
#include "types/vc71_string.h"
using Char = char;
using String = legacy::vc71::string<Char>;

using CloseHandleFn=int (__stdcall*)(void*);
CSF_GLOBAL(CloseHandleFn, close_handle, 0x0082D0E0);
static void __cdecl Global_0082AD90() {
    auto* p=reinterpret_cast<uint8_t*>(CSF_ADDR_DATA(0x008BAEB4));
    void* handle=CSF_AT(void*,p,4);
    CSF_AT(void**,p,0)=CSF_VTBL(0x0085EB30);
    close_handle(handle);
    CSF_AT(void*,p,4)=nullptr;
    CSF_AT(void**,p,0)=CSF_VTBL(0x0085EB24);
}
HOOK(0x0082AD90, Global_0082AD90, "initfn");
