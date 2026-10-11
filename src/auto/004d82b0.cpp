#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Callee at 0x004A6F70 (tail-jumped to by the original; stack args forwarded, ret 8).
CSF_FUNC(int, __fastcall, DispatchToCommandSubobject, 0x004A6F70, (void* self, void* edx, int a2, int a4));

// Sets bit 0 of the owner's flags word at +0x18, then forwards the call, with the
// stack arguments unchanged, to the command subobject stored at owner+0x310.
static int __fastcall TAutonomo_SetOwnerFlagAndForwardCommand(void* self, void* /*edx*/, int a2, int a3)
{
    uint8_t* owner = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, self, 0x8));
    CSF_AT(uint32_t, owner, 0x18) |= 1u;
    void* commandSubobject = CSF_AT(void*, owner, 0x310);
    return DispatchToCommandSubobject(commandSubobject, nullptr, a2, a3);
}

HOOK(0x4D82B0, TAutonomo_SetOwnerFlagAndForwardCommand, "auto");
