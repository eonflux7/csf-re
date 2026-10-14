#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(uint32_t*, __fastcall, sub_4A9C80, 0x004A9C80, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(uint32_t*, __fastcall, AiCoverAttackTasks_GetPositionData, 0x004A9CA0, (void* self, void* edx));
CSF_FUNC(char, __fastcall, sub_4AA310, 0x004AA310, (void* self, void* edx, int a2));
CSF_FUNC(bool, __fastcall, sub_4DCDC0, 0x004DCDC0, (void* self, void* edx, uint32_t* a2));

static int __fastcall StaticGuard_CanEngageTarget(S_004aa310* self, void* /*edx*/, S_004abb90* a2)
{
    // Code-address annotation: function value, not a vtable.
    [[maybe_unused]] static const auto kStaticGuardVtbl = reinterpret_cast<void**>(CSF_ADDR_FN(0x004AA310));

    // Table pointer taken from the global block returned by GetValue8B0554().
    S_004dc8f0* table = CSF_AT(S_004dc8f0*, reinterpret_cast<uint8_t*>(GetValue8B0554()), 8152);

    // Scratch slot the callee fills with its result (original: [ebp-8], 8 bytes of room).
    alignas(8) uint8_t v7Bytes[8];
    uint32_t* v7 = reinterpret_cast<uint32_t*>(v7Bytes);

    uint32_t* v4 = sub_4A9C80(a2, nullptr, v7);

    int result = 0;
    if (sub_4DCDC0(table, nullptr, v4))
    {
        uint32_t* positionData = AiCoverAttackTasks_GetPositionData(a2, nullptr);
        if (sub_4AA310(self, nullptr, static_cast<int>(reinterpret_cast<uintptr_t>(positionData))))
            return 1;
    }
    return result;
}

HOOK(0x004AEE30, StaticGuard_CanEngageTarget, "auto");
