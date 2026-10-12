#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, GetLinkedObjectField10, 0x004DB5D0, (void* self, void*));
CSF_FUNC(int, __fastcall, UnnamedApply_4DC180, 0x004DC180, (void* self, void* edx, int a2));

// Copies a 12-byte block from the linked object's 0x64C area into the object returned by
// GetLinkedObjectField10 (+0x0C), then applies the linked object's 0x21C slot via UnnamedApply_4DC180.
static int __stdcall TEstatico_CopyGuardAnchorAndApply(int a1)
{
    void* self = reinterpret_cast<void*>(static_cast<std::intptr_t>(a1));

    const int linkedValue = GetLinkedObjectValue_4DB5C0(self, nullptr);
    const int sourceAddress = linkedValue ? linkedValue - 4 : 0;
    uint8_t* const sourceBase = reinterpret_cast<uint8_t*>(static_cast<std::intptr_t>(sourceAddress));

    const int fieldObject = GetLinkedObjectField10(self, nullptr);
    uint8_t* const destBase = reinterpret_cast<uint8_t*>(static_cast<std::intptr_t>(fieldObject)) + 12;

    CSF_AT(uint32_t, destBase, 0) = CSF_AT(uint32_t, sourceBase, 0x64C);
    CSF_AT(uint32_t, destBase, 4) = CSF_AT(uint32_t, sourceBase, 0x650);
    CSF_AT(uint32_t, destBase, 8) = CSF_AT(uint32_t, sourceBase, 0x654);

    const int linkedValueAgain = GetLinkedObjectValue_4DB5C0(self, nullptr);
    const int applyArg = linkedValueAgain ? linkedValueAgain - 4 + 540 : 540;
    return UnnamedApply_4DC180(self, nullptr, applyArg);
}

HOOK(0x004AFF80, TEstatico_CopyGuardAnchorAndApply, "auto");
