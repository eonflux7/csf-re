#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Bound callees (original addresses, signatures as given in the packet).
CSF_FUNC(void, __fastcall, InitializeTemporaryString, 0x0064A6B0, (void* self, void* edx, std::int32_t source));
CSF_FUNC(void*, __fastcall, sub_64AD60, 0x0064AD60, (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, sub_64CEB0, 0x0064CEB0, (void* self, void* edx, char a2, void* a3));
CSF_FUNC(void*, __fastcall, sub_66B590, 0x0066B590, (void* self, void* edx));

// Sends the "TeHeCogido" (has been caught) command for this object through the
// command dispatcher and returns the dispatcher's result.
static char __fastcall C_00858180_SendHasBeenCaughtCommand(C_00858180* self, void* /*edx*/)
{
    // Temporary 8-byte object at [ebp-0xC]: its first dword holds `self`
    // (the original pushes ecx = this there), and the callee initializes it
    // from the string constant.
    alignas(8) std::uint8_t tempObjBytes[8];
    *reinterpret_cast<C_00858180**>(tempObjBytes) = self;

    InitializeTemporaryString(tempObjBytes, nullptr, reinterpret_cast<std::int32_t>(CSF_STR(0x0086797C, "TeHeCogido")));

    // Result of sub_66B590 is stored at [ebp-4] and its address is handed to sub_64AD60.
    std::uint32_t commandStringField = 0;
    commandStringField = reinterpret_cast<std::uint32_t>(sub_66B590(tempObjBytes, nullptr));

    void* commandHandle = sub_64AD60(self, nullptr,
        static_cast<int>(reinterpret_cast<std::uintptr_t>(&commandStringField)));

    return sub_64CEB0(self, nullptr, 0, commandHandle);
}

HOOK(0x00742140, C_00858180_SendHasBeenCaughtCommand, "auto");
