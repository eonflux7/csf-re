#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, StringBase_AssignCString, 0x00429B60,
         (void* self, void* edx, char* text, unsigned int length));

static CMenuLoadGame* __fastcall CMenuLoadGame_PrepareLoadState(
    CMenuLoadGame* self, void* edx)
{
    self->field_0018 = 0;
    self->field_011c = 0;
    self->field_0100 = 0;
    StringBase_AssignCString(
        reinterpret_cast<uint8_t*>(self) + 0x1C,
        edx, const_cast<char*>(CSF_STR(0x0082E65C, "")), 0);
    self->field_0104 = 0;
    return self;
}

// Take the decorated helper's address in C++, not in inline assembly.
static CMenuLoadGame* (__fastcall* s_PrepareLoadState)(
    CMenuLoadGame*, void*) = &CMenuLoadGame_PrepareLoadState;

static __declspec(naked) void CMenuLoadGame_ResetStateAndForwardToVirtual()
{
    __asm {
        mov eax, dword ptr [s_PrepareLoadState]
        call eax
        mov ecx, eax
        mov eax, dword ptr [ecx]
        jmp dword ptr [eax + 0x40]
    }
}

HOOK(0x00539960, CMenuLoadGame_ResetStateAndForwardToVirtual, "auto");
