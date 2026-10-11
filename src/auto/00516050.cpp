#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Original: 0x00694C80, __fastcall (ecx = this), returns void**; result unused.
CSF_FUNC(void**, __fastcall, InitTempRecord, 0x00694C80, (void* self, void* edx));

// Original: 0x00515E80, __thiscall with 2 stack args (ret 8).
CSF_FUNC(int, __fastcall, PrepareRecordForCall, 0x00515E80, (void* self, void* edx, void* a1, int a2));

// Original: 0x00712D10, __thiscall with no args; resets the embedded member at +8.
CSF_FUNC(void*, __fastcall, ResetEmbeddedMemberAt8, 0x00712D10, (void** self, void* edx));

// __thiscall(this = ecx, a2 = [ebp+8], a3 = [ebp+0xC]) with ret 8, written as __fastcall with a dummy edx.
static char __fastcall ProfileManager_CallVirtualWithTemporaryRecord(C_00840af8* self, void* /*edx*/, int a2, int a3)
{
    // Scratch record reserved at the original frame size ([ebp-0x918], 0x918 bytes).
    alignas(8) uint8_t recordBytes[0x918];
    S_00694cd0& tempRecord = *reinterpret_cast<S_00694cd0*>(recordBytes);

    InitTempRecord(&tempRecord, nullptr);
    PrepareRecordForCall(self, nullptr, &tempRecord, a3);

    // Virtual slot at byte offset 0x84 -> index 33 of the vtable; signature (int a2, S* a1), ret 8.
    using VirtualFn = char(__fastcall*)(void* self, void* edx, int a2, void* a1);
    char result = reinterpret_cast<VirtualFn>(CSF_AT(void**, self, 0)[0x84 / 4])(
        self, nullptr, a2, &tempRecord);

    ResetEmbeddedMemberAt8(reinterpret_cast<void**>(&tempRecord), nullptr);
    return result;
}

HOOK(0x00516050, ProfileManager_CallVirtualWithTemporaryRecord, "auto");
