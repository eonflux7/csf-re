#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, MenuExtras_ParseDescriptionId, 0x00698D90, (const char* text));
CSF_FUNC(void*, __fastcall, GrowButtonArray, 0x0041E5F0, (void* self, void* edx, int growth));

static int __fastcall CGuiControl_AppendParsedDescriptionId(CGuiControl* self, void* /*edx*/, int a2)
{
    (void)CSF_VTBL(0x0041E5F0);

    int parsedId = MenuExtras_ParseDescriptionId(reinterpret_cast<const char*>(static_cast<uintptr_t>(a2)));

    S_0041e5f0* buttonArray = reinterpret_cast<S_0041e5f0*>(reinterpret_cast<uint8_t*>(self) + 0x170);

    uint32_t count = CSF_AT(uint32_t, self, 0x178);
    if (count < buttonArray->field_0004) {
        // no growth needed
    } else {
        GrowButtonArray(buttonArray, nullptr, static_cast<int>(buttonArray->field_000c));
    }

    uint32_t index = buttonArray->field_0008;
    uint8_t* base = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(buttonArray->field_0000));
    int* slot = reinterpret_cast<int*>(base + 4 * index);

    uint32_t next = index + 1;
    buttonArray->field_0008 = next;
    *slot = parsedId;
    return static_cast<int>(next);
}

HOOK(0x00452230, CGuiControl_AppendParsedDescriptionId, "auto");
