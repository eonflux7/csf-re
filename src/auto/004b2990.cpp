#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(TAiState*, __fastcall, TAiState_Constructor, 0x004DBF70, (TAiState*, void*));
static TFncInteractivo* __fastcall TFncInteractivo_Construct(TFncInteractivo* self, void*) {
    TAiState_Constructor(reinterpret_cast<TAiState*>(self), nullptr);
    self->vtbl = CSF_VTBL(0x0083A788);
    TAiState_Constructor(reinterpret_cast<TAiState*>(reinterpret_cast<char*>(self)+4), nullptr);
    CSF_AT(void*, self, 4) = CSF_VTBL(0x00838C28);
    TAiState_Constructor(reinterpret_cast<TAiState*>(reinterpret_cast<char*>(self)+8), nullptr);
    CSF_AT(void*, self, 8) = CSF_VTBL(0x00838CB0);
    TAiState_Constructor(reinterpret_cast<TAiState*>(reinterpret_cast<char*>(self)+12), nullptr);
    CSF_AT(void*, self, 12) = CSF_VTBL(0x00838D38);
    TAiState_Constructor(reinterpret_cast<TAiState*>(reinterpret_cast<char*>(self)+16), nullptr);
    CSF_AT(void*, self, 16) = CSF_VTBL(0x00838DC8);
    self->field_0018 = self->field_001c = 0xffffffffu;
    CSF_AT(void*, self, 20) = CSF_VTBL(0x0082D448);
    self->field_0020 = self->field_0024 = self->field_0028 = 0;
    return self;
}
HOOK(0x004B2990, TFncInteractivo_Construct, "auto");
