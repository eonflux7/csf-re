#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(void*, __fastcall, AssignStringRange_429B60, 0x00429B60,
         (void* self, void* edx, const char* text, unsigned int length));

static CMenuScenarioSelect* __fastcall CMenuScenarioSelect_ResetSelectionFields(
    CMenuScenarioSelect* self, void* edx)
{
    self->field_0014 = 0;
    self->field_0074 = 0;
    self->field_0058 = 0;
    self->field_005c = 0;
    self->field_0060 = 0;
    self->field_006c = 0;

    AssignStringRange_429B60(reinterpret_cast<uint8_t*>(self) + 0x78, edx,
                      CSF_STR(0x0082E65C, ""), 0);

    self->field_0068 = 0;
    self->field_0064 = 0;

    AssignStringRange_429B60(reinterpret_cast<uint8_t*>(self) + 0x1C, nullptr,
                      CSF_STR(0x0082E65C, ""), 0);

    self->field_0038 = 0;
    self->field_0094 = 0.0f;
    self->field_0040 = 0;
    self->field_003c = 0xFFFFFFFFu;

    return self;
}

// Taking the address in C++ resolves the decorated function symbol.
static CMenuScenarioSelect* (__fastcall* s_resetSelectionFields)(
    CMenuScenarioSelect*, void*) = &CMenuScenarioSelect_ResetSelectionFields;

static __declspec(naked) void CMenuScenarioSelect_ResetSelectionState()
{
    __asm {
        mov eax, dword ptr [s_resetSelectionFields]
        call eax
        mov ecx, eax
        mov eax, dword ptr [ecx]
        jmp dword ptr [eax + 0x40]
    }
}

HOOK(0x0052DD20, CMenuScenarioSelect_ResetSelectionState, "auto");
