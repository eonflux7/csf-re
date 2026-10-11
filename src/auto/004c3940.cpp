#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __fastcall, CoverAttack_SetStance_492720, 0x00492720, (void* self, void* edx, int a2));
CSF_FUNC(int, __stdcall, AiCore_PushFinDebugIaEntry, 0x004BF4D0, (int a1));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));

static int __stdcall AiStateFactory_InitializeLinkedActor(int state)
{
    int linked = GetLinkedObjectValue_4DB5C0(reinterpret_cast<void*>(state), nullptr);
    void* actor = reinterpret_cast<void*>(linked ? linked - 4 : 0);
    CoverAttack_SetStance_492720(CSF_AT(void*, actor, 0x2C), nullptr, 2);

    linked = GetLinkedObjectValue_4DB5C0(reinterpret_cast<void*>(state), nullptr);
    actor = reinterpret_cast<void*>(linked ? linked - 4 : 0);
    void* controller = CSF_AT(void*, actor, 0x2C);
    reinterpret_cast<void(__fastcall*)(void*, void*, int, int, int, int)>(
        CSF_AT(void**, controller, 0)[0x8C / 4])(
            controller, nullptr, 0, 0, -1, 0);

    linked = GetLinkedObjectValue_4DB5C0(reinterpret_cast<void*>(state), nullptr);
    actor = reinterpret_cast<void*>(linked ? linked - 4 : 0);
    void* stateMember = reinterpret_cast<std::uint8_t*>(actor) + 0x2D4;
    reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
        CSF_AT(void**, stateMember, 0)[0x28 / 4])(
            stateMember, nullptr, state);

    linked = GetLinkedObjectValue_4DB5C0(reinterpret_cast<void*>(state), nullptr);
    actor = reinterpret_cast<void*>(linked ? linked - 4 : 0);
    CSF_AT(float, actor, 0x2E0) = 1.0f;
    CSF_AT(float, actor, 0x2E4) = 1.0f;

    return AiCore_PushFinDebugIaEntry(state);
}

HOOK(0x004C3940, AiStateFactory_InitializeLinkedActor, "auto");
