#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(void, __fastcall, AiDistracted_InitializeCoreActionState, 0x004B6140, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));

static int __stdcall AiDistracted_InitializeAction(int owner)
{
    int linked = GetLinkedObjectValue_4DB5C0(reinterpret_cast<void*>(owner), nullptr);
    std::uint32_t base = linked ? static_cast<std::uint32_t>(linked) - 4 : 0;
    AiDistracted_InitializeCoreActionState(
        reinterpret_cast<void*>(base + 812), nullptr, owner);

    linked = GetLinkedObjectValue_4DB5C0(reinterpret_cast<void*>(owner), nullptr);
    base = linked ? static_cast<std::uint32_t>(linked) - 4 : 0;
    CSF_AT(float, reinterpret_cast<void*>(base), 0x338) = 0.3f;
    CSF_AT(float, reinterpret_cast<void*>(base), 0x33C) = 0.3f;
    return static_cast<int>(base);
}

HOOK(0x004B7EA0, AiDistracted_InitializeAction, "auto");
