// Includes the game-owned shared destructor chunk; no missing address slot.
// Shape: global destructor tail. Original bytes: b998ad8b00e91630e7ff
#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"
#include "legacy/string.h"
#include "types/vc71_string.h"
using Char = char;
using String = legacy::vc71::string<Char>;

CSF_FUNC(void, __fastcall, S_004713e0_ReleaseOwnedStorage, 0x0069B3B0, (S_004713e0* self, void*));
static void __cdecl Global_0082AD80() {
    auto* p=reinterpret_cast<uint8_t*>(CSF_ADDR_DATA(0x008BAD98));
    CSF_AT(void**,p,0)=CSF_VTBL(0x0085EAC0);
    S_004713e0_ReleaseOwnedStorage(reinterpret_cast<S_004713e0*>(p+0xD4),nullptr);
    S_004713e0_ReleaseOwnedStorage(reinterpret_cast<S_004713e0*>(p+0x90),nullptr);
    S_004713e0_ReleaseOwnedStorage(reinterpret_cast<S_004713e0*>(p+0x4C),nullptr);
    S_004713e0_ReleaseOwnedStorage(reinterpret_cast<S_004713e0*>(p+8),nullptr);
    CSF_AT(void**,p,0)=CSF_VTBL(0x0085EA70);
}
HOOK(0x0082AD80, Global_0082AD80, "initfn");
