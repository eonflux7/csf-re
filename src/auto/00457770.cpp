#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Callee bound by address: 0x456EC0 (__thiscall, ecx = this, still intact at the call site).
CSF_FUNC(int, __fastcall, SceneLoader_IsEntryTextAlreadyPresent, 0x00456EC0, (void* self, void* edx, char* a2));
// Callee bound by address: 0x457580 (__stdcall, one stack argument).
CSF_FUNC(int, __stdcall, HudPortraits_CreateEntryFromName, 0x00457580, (void* a1));
// Callee bound by address: 0x41E5F0 (__thiscall array growth).
CSF_FUNC(void*, __fastcall, S_0041e5f0_GrowArrayCapacity, 0x0041E5F0, (S_0041e5f0* self, void* /*edx*/, int additionalCapacity));

// Vtable references listed in the packet.
[[maybe_unused]] static const auto kEntryVtbl = CSF_VTBL(0x00456EC0);
[[maybe_unused]] static const auto kEntryArrayVtbl = CSF_VTBL(0x0041E5F0);

static void __fastcall HudPortraits_SetEntryText(S_00456ec0* self, void* /*edx*/, uint32_t* a2)
{
    if (a2 == nullptr)
        return;
    if (CSF_AT(uint8_t, a2, 0) == 0)
        return;
    if (SceneLoader_IsEntryTextAlreadyPresent(self, nullptr, reinterpret_cast<char*>(a2)))
        return;

    int entryValue = HudPortraits_CreateEntryFromName(a2);
    if (entryValue == 0)
        return;

    uint8_t* entryObject = reinterpret_cast<uint8_t*>(entryValue);
    CSF_AT(uint32_t, entryObject, 4) = self->field_0018;

    auto* entryArray = reinterpret_cast<S_0041e5f0*>(reinterpret_cast<uint8_t*>(self) + 0x10);
    if (entryArray->field_0008 >= entryArray->field_0004)
        S_0041e5f0_GrowArrayCapacity(entryArray, nullptr, entryArray->field_000c);

    uint32_t index = entryArray->field_0008;
    uint32_t* slot = reinterpret_cast<uint32_t*>(entryArray->field_0000 + 4 * index);
    entryArray->field_0008 = index + 1;
    *slot = static_cast<uint32_t>(entryValue);
}

HOOK(0x00457770, HudPortraits_SetEntryText, "auto");
