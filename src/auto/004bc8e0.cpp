#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Callee: CoverAttack state setter. Original: mov ecx, a2; push this+4; call -> __thiscall with one stack arg, ret 4.
CSF_FUNC(int, __fastcall, CoverAttack_SetState_4DC150, 0x004DC150, (void* self, void* edx, int a2));

// Resets the TFncMacroOrden order-tracking fields, then forwards to CoverAttack_SetState
// with the TAiState argument and the address of the embedded sub-object at +4.
static int __fastcall TFncMacroOrden_ResetFieldsAndSetState(TFncMacroOrden* self, void* /*edx*/, TAiState* a2)
{
    CSF_AT(int, self, 0xD4) = 0;
    CSF_AT(int, self, 0xE4) = 0;
    CSF_AT(int, self, 0xF4) = 0;
    CSF_AT(int, self, 0xA0) = -1;
    CSF_AT(int, self, 0xA4) = -1;
    CSF_AT(uint8_t, self, 0xB8) = 0;
    CSF_AT(uint8_t, self, 0xB9) = 0;
    CSF_AT(int, self, 0xB4) = 0;

    // Address of the embedded sub-object (this + 4), passed as an int stack argument.
    int subObjectAddress = static_cast<int>(reinterpret_cast<uintptr_t>(self) + 4);
    return CoverAttack_SetState_4DC150(a2, nullptr, subObjectAddress);
}

HOOK(0x004BC8E0, TFncMacroOrden_ResetFieldsAndSetState, "auto");
