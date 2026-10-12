#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __fastcall, CoverAttack_SetStance_492720, 0x00492720, (void* self, void* edx, int a2));
CSF_FUNC(void, __fastcall, AiDistracted_InitializeCoreActionState, 0x004B6140, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, AiCoverAttackTasks_SetCoverTransition, 0x004D1100, (void* self, void* , int a2, int a3));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

static std::uint8_t* AiDistracted_GetCoreObject(void* owner)
{
    const int linked = GetLinkedObjectValue_4DB5C0(owner, nullptr);
    return linked
        ? reinterpret_cast<std::uint8_t*>(static_cast<std::uintptr_t>(linked) - 4)
        : nullptr;
}

static int __fastcall AiDistracted_DispatchCoreAction(
    void* self, void* /*edx*/, int ownerValue, char action)
{
    void* owner = reinterpret_cast<void*>(ownerValue);
    std::uint8_t* core = AiDistracted_GetCoreObject(owner);

    if (CSF_AT(int, core, 0x608) != -1 ||
        CSF_AT(int, core, 0x60C) != -1)
    {
        core = AiDistracted_GetCoreObject(owner);
        AiDistracted_InitializeCoreActionState(core + 0x32C, nullptr, ownerValue);

        core = AiDistracted_GetCoreObject(owner);
        CSF_AT(float, core, 0x338) = 0.3f;
        CSF_AT(float, core, 0x33C) = 0.3f;

        core = AiDistracted_GetCoreObject(owner);
        CSF_AT(int, core, 0x3B8) = 1;
        CSF_AT(float, core, 0x3C0) = 0.0f;
        CSF_AT(float, core, 0x3C4) = 0.0f;
        CSF_AT(float, core, 0x3C0) = 0.1f;
        CSF_AT(float, core, 0x3C4) = 0.1f;
    }

    CSF_AT(char, self, 0x4) = action;

    core = AiDistracted_GetCoreObject(owner);
    CSF_AT(std::uint32_t, core, 0x680) |= 1u;

    core = AiDistracted_GetCoreObject(owner);
    void* globalState = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
    AiCoverAttackTasks_SetCoverTransition(
        core + 0x690, nullptr, 0, CSF_AT(int, globalState, 0xA98));

    core = AiDistracted_GetCoreObject(owner);
    CoverAttack_SetStance_492720(CSF_AT(void*, core, 0x2C), nullptr, 3);

    core = AiDistracted_GetCoreObject(owner);
    void* controller = CSF_AT(void*, core, 0x2C);
    return reinterpret_cast<int(__fastcall*)(void*, void*, int, int, int, int)>(
        CSF_AT(void**, controller, 0)[0x8C / 4])(
            controller, nullptr, 0, 0, -1, 0);
}

HOOK(0x004BA700, AiDistracted_DispatchCoreAction, "auto");
