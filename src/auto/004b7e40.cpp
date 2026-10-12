#include <cstdint>
#include "hook.h"
#include "globals.h"

CSF_FUNC(int, __stdcall, ConfigureLinkedObject, 0x004B2030, (int arg0, uint32_t* a2));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, CoverAttack_SetState_4DC150, 0x004DC150, (void* self, void* edx, int a2));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));

static int __fastcall CoverAttack_SelectState(void* self, void* /*edx*/, void* state, char alternate)
{
    auto* bytes = static_cast<uint8_t*>(self);
    if (alternate)
        return CoverAttack_SetState_4DC150(
            state, nullptr, reinterpret_cast<int>(bytes + 4));

    CoverAttack_SetState_4DC150(
        state, nullptr, reinterpret_cast<int>(bytes + 32));

    const int linkedValue = GetLinkedObjectValue_4DB5C0(state, nullptr);
    const uint32_t linkedBase = linkedValue
        ? static_cast<uint32_t>(linkedValue) - 4
        : 0;
    auto* linkedMember = reinterpret_cast<uint32_t*>(linkedBase + 856);
    void* provider = GetCachedObjectProvider(state, nullptr);
    return ConfigureLinkedObject(reinterpret_cast<int>(provider), linkedMember);
}

HOOK(0x004B7E40, CoverAttack_SelectState, "auto");
