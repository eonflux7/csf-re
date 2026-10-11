#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Callees, bound by address with the signatures from the packet.
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, sub_4AEB90, 0x004AEB90, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, sub_4AF100, 0x004AF100, (void* self, void* edx));
CSF_FUNC(uint32_t, __fastcall, sub_4C5D60, 0x004C5D60, (void* self, void* edx, int a1));
CSF_FUNC(int, __fastcall, sub_4DBCB0, 0x004DBCB0, (void* self, void* edx, int a2));

// Original: uint32_t __stdcall sub_4C5E50(int a1, int a2, int a3), with the entry ecx kept as `this`
// (saved in edi and passed to sub_4C5D60). Stack args: 3 -> ret 12.
static uint32_t __fastcall TFncAtacarDesdePosicion_AttackFromPositionOrRelease(
    void* self, void* /*edx*/, int a1, int a2, int a3)
{
    (void)a2;
    (void)a3;

    void* target = reinterpret_cast<void*>(static_cast<uintptr_t>(static_cast<uint32_t>(a1)));

    int linkedValue = GetLinkedObjectValue_4DB5C0(target, nullptr);
    void* linkedObject = linkedValue
        ? reinterpret_cast<void*>(static_cast<uintptr_t>(static_cast<uint32_t>(linkedValue - 4)))
        : nullptr;

    if (sub_4AEB90(linkedObject, nullptr) != -1)
        return sub_4C5D60(self, nullptr, a1);

    linkedValue = GetLinkedObjectValue_4DB5C0(target, nullptr);
    if (linkedValue)
        sub_4AF100(reinterpret_cast<void*>(static_cast<uintptr_t>(static_cast<uint32_t>(linkedValue - 4))), nullptr);
    else
        sub_4AF100(nullptr, nullptr);

    return static_cast<uint32_t>(sub_4DBCB0(target, nullptr, 0));
}

HOOK(0x004C5E50, TFncAtacarDesdePosicion_AttackFromPositionOrRelease, "auto");
