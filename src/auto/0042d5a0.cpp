#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Callees, bound by address (signatures as given in the packet).
CSF_FUNC(int, __cdecl, GetGlobalValue_6C5760, 0x006C5760, ());
CSF_FUNC(int, __fastcall, sub_42D0B0, 0x0042D0B0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, sub_42D140, 0x0042D140, (void* self, void* edx));
CSF_FUNC(int, __fastcall, sub_42D2B0, 0x0042D2B0, (void* self, void* edx));
CSF_FUNC(char, __fastcall, sub_6C7C30, 0x006C7C30, (void* self, void* edx));
CSF_FUNC(int, __fastcall, sub_6C8420, 0x006C8420, (void* self, void* edx, void* a2));

static char __fastcall HudWidget_RefreshTextureAndFlush(void* self, void* /*edx*/)
{
    // v2 = *(C_0085e7b8**)(GetGlobalValue() + 4)
    void* globalBlock = reinterpret_cast<void*>(GetGlobalValue_6C5760());
    C_0085e7b8* v2 = CSF_AT(C_0085e7b8*, globalBlock, 0x4);

    // sub_6C8420(v2, this + 8)
    sub_6C8420(v2, nullptr, reinterpret_cast<uint8_t*>(self) + 0x8);

    // v3 = *(int**)(this + 4)
    int* v3 = CSF_AT(int*, self, 0x4);
    sub_42D0B0(v3, nullptr);

    // Original re-reads v3[1] and v3[2] after the call.
    if (CSF_AT(int, v3, 0x4) && CSF_AT(int, v3, 0x8)) {
        sub_42D140(v3, nullptr);
        sub_42D2B0(v3, nullptr);
    }

    // Original tail-jumps to sub_6C7C30 with ecx = v2; return value passes through.
    return sub_6C7C30(v2, nullptr);
}

HOOK(0x0042D5A0, HudWidget_RefreshTextureAndFlush, "auto");
