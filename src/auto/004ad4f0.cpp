#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));

// Resolves the object linked to `ownerHandle`, walks owner -> +0x34 -> +0x310 to find
// a subsystem object, and forwards `argument` to that object's virtual slot 36 (+0x90).
static int __stdcall DispatchToLinkedSubsystem(int ownerHandle, int argument)
{
    int linkedObjectValue = GetLinkedObjectValue_4DB5C0(
        reinterpret_cast<void*>(static_cast<uintptr_t>(static_cast<uint32_t>(ownerHandle))), nullptr);

    void* linkBase;
    if (linkedObjectValue != 0)
        linkBase = reinterpret_cast<void*>(static_cast<uintptr_t>(static_cast<uint32_t>(linkedObjectValue - 4)));
    else
        linkBase = nullptr;

    void* linkedOwner = CSF_AT(void*, linkBase, 0x2C);
    void* ownerWorld = CSF_AT(void*, linkedOwner, 0x34);
    void* subsystem = CSF_AT(void*, ownerWorld, 0x310);

    using SubsystemSlot36Fn = int(__fastcall*)(void* self, void* edx, int arg);
    SubsystemSlot36Fn slot36 = reinterpret_cast<SubsystemSlot36Fn>(CSF_AT(void**, subsystem, 0)[144 / 4]);
    return slot36(subsystem, nullptr, argument);
}

HOOK(0x004AD4F0, DispatchToLinkedSubsystem, "auto");
