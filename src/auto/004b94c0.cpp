#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(TAiState*, __fastcall, TAiState_Constructor, 0x004DBF70, (TAiState* self, void*));
CSF_FUNC(uint32_t*, __fastcall, sub_4B27B0, 0x004B27B0, (void* self, void* edx));

static uint32_t* __fastcall TFncMacroOrden_Construct(TFncMacroOrden* self, void* /*edx*/) {
    TAiState_Constructor(reinterpret_cast<TAiState*>(self), nullptr);
    *reinterpret_cast<void***>(self) = CSF_VTBL(0x0083A948);

    void* subObject = reinterpret_cast<uint8_t*>(self) + 4;
    TAiState_Constructor(reinterpret_cast<TAiState*>(subObject), nullptr);
    *reinterpret_cast<void**>(subObject) = reinterpret_cast<void*>(CSF_ADDR_DATA(0x00838090));

    sub_4B27B0(reinterpret_cast<uint8_t*>(self) + 8, nullptr);

    CSF_AT(uint32_t, self, 0xCC) = 0;
    CSF_AT(uint32_t, self, 0xD0) = 0;
    CSF_AT(uint32_t, self, 0xD4) = 0;
    CSF_AT(uint32_t, self, 0xD8) = 10;
    CSF_AT(uint32_t, self, 0xDC) = 0;
    CSF_AT(uint32_t, self, 0xE0) = 0;
    CSF_AT(uint32_t, self, 0xE4) = 0;
    CSF_AT(uint32_t, self, 0xE8) = 10;
    CSF_AT(uint32_t, self, 0xEC) = 0;
    CSF_AT(uint32_t, self, 0xF0) = 0;
    CSF_AT(uint32_t, self, 0xF4) = 0;
    CSF_AT(uint32_t, self, 0xF8) = 10;

    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x004B94C0, TFncMacroOrden_Construct, "auto");
