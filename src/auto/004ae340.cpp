#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, AiCore_ConstructStateData, 0x004A9D80, (uint32_t* self, void*));
CSF_FUNC(TAiState*, __fastcall, TAiState_Constructor, 0x004DBF70, (TAiState* self, void*));

static TFncRastrear* __fastcall TFncRastrear_Construct(TFncRastrear* self, void* /*edx*/)
{
    TAiState_Constructor(reinterpret_cast<TAiState*>(self), nullptr);
    *(void***)self = CSF_VTBL(0x008397E0);

    // Sub-object at +0x04
    TAiState_Constructor(reinterpret_cast<TAiState*>(reinterpret_cast<uint8_t*>(self) + 0x04), nullptr);
    CSF_AT(void*, self, 0x04) = CSF_VTBL(0x00837380);

    // Sub-object at +0x08
    TAiState_Constructor(reinterpret_cast<TAiState*>(reinterpret_cast<uint8_t*>(self) + 0x08), nullptr);
    CSF_AT(void*, self, 0x08) = CSF_VTBL(0x00837408);

    // Sub-object at +0x0C
    TAiState_Constructor(reinterpret_cast<TAiState*>(reinterpret_cast<uint8_t*>(self) + 0x0C), nullptr);
    CSF_AT(void*, self, 0x0C) = CSF_VTBL(0x00837490);

    CSF_AT(uint32_t, self, 0x10) = 0;
    CSF_AT(uint32_t, self, 0x14) = 0;
    CSF_AT(uint32_t, self, 0x18) = 0;
    CSF_AT(uint32_t, self, 0x1C) = 10;
    CSF_AT(uint32_t, self, 0x20) = 0;
    CSF_AT(uint32_t, self, 0x24) = 0;
    CSF_AT(uint32_t, self, 0x28) = 0;
    CSF_AT(uint32_t, self, 0x2C) = 10;

    AiCore_ConstructStateData(
        reinterpret_cast<uint32_t*>(reinterpret_cast<uint8_t*>(self) + 0x30), nullptr);

    CSF_AT(uint32_t, self, 0x44) = 0;
    CSF_AT(uint32_t, self, 0x48) = 0;
    CSF_AT(uint32_t, self, 0x4C) = 0;

    return self;
}

HOOK(0x004AE340, TFncRastrear_Construct, "auto");
