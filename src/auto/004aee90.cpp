#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, AiFncStatic_ApplyCachedProvider, 0x004AD600,
         (void* self, void* edx, void* a2, uint32_t* a3));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0,
         (void* self, void*));
CSF_FUNC(int, __fastcall, UnnamedApply_4DC180, 0x004DC180,
         (void* self, void* edx, int a2));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10,
         (void* self, void* edx));

static char __fastcall AiFncStatic_UpdateCoverAttackState(
    void* self, void* /*edx*/, C_0085e7b8* actor, int updateArgument)
{
    if (!CSF_AT(int, self, 0x3B8))
        return 0;

    auto* attackState = reinterpret_cast<uint8_t*>(self) + 0x3B4;
    reinterpret_cast<void(__fastcall*)(void*, void*, C_0085e7b8*, int)>(
        CSF_AT(void**, attackState, 0)[0x2C / 4])(
            attackState, nullptr, actor, updateArgument);

    const int state = CSF_AT(int, self, 0x3B8);
    if (state != 2 && state != 3)
        return 0;

    void* owner = CSF_AT(void*, self, 0x2C);
    void* condition = CSF_AT(void*, owner, 0x44);
    if (!reinterpret_cast<bool(__fastcall*)(void*, void*)>(
            CSF_AT(void**, condition, 0)[0x90 / 4])(condition, nullptr))
        return 0;

    int linkedValue = GetLinkedObjectValue_4DB5C0(actor, nullptr);
    uint32_t linkedBase = linkedValue
        ? static_cast<uint32_t>(linkedValue) - 4
        : 0;
    UnnamedApply_4DC180(
        actor, nullptr, static_cast<int>(linkedBase + 0x5C4));

    linkedValue = GetLinkedObjectValue_4DB5C0(actor, nullptr);
    linkedBase = linkedValue
        ? static_cast<uint32_t>(linkedValue) - 4
        : 0;

    auto* stateData = reinterpret_cast<uint32_t*>(
        reinterpret_cast<uint8_t*>(self) + 0x3C8);
    void* provider = GetCachedObjectProvider(actor, nullptr);
    AiFncStatic_ApplyCachedProvider(
        reinterpret_cast<void*>(linkedBase + 0x5C4),
        nullptr, provider, stateData);

    CSF_AT(float, self, 0x3C0) = 0.0f;
    CSF_AT(float, self, 0x3C4) = 0.0f;
    CSF_AT(int, self, 0x3B8) = 1;
    CSF_AT(float, self, 0x3C0) = 0.1f;
    CSF_AT(float, self, 0x3C4) = 0.1f;
    return 1;
}

HOOK(0x004AEE90, AiFncStatic_UpdateCoverAttackState, "auto");
