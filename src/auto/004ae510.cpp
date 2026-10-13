#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(TAiState*, __fastcall, TAiState_Constructor, 0x004DBF70, (TAiState* self, void*));

// Constructs a TFncIrAParapeto (move-to-cover task) in place and returns it.
static TFncIrAParapeto* __fastcall TFncIrAParapeto_Construct(TFncIrAParapeto* self, void* /*edx*/)
{
    TAiState_Constructor(self, nullptr);
    self->vtbl = CSF_VTBL(0x00839D70);

    // First embedded sub-object at +0x04.
    TAiState* firstSub = reinterpret_cast<TAiState*>(reinterpret_cast<uint8_t*>(self) + 0x04);
    TAiState_Constructor(firstSub, nullptr);
    *reinterpret_cast<void**>(firstSub) = reinterpret_cast<void*>(CSF_ADDR_DATA(0x00837858));

    // Second embedded sub-object at +0x08.
    TAiState* secondSub = reinterpret_cast<TAiState*>(reinterpret_cast<uint8_t*>(self) + 0x08);
    TAiState_Constructor(secondSub, nullptr);
    *reinterpret_cast<void**>(secondSub) = reinterpret_cast<void*>(CSF_ADDR_DATA(0x008378E0));

    self->field_000c = 0;
    self->field_0010 = 0;
    self->field_0014 = 0;
    self->field_0018 = 10;
    self->field_001c = 0;
    self->field_0020 = 0;
    self->field_0024 = 0;

    return self;
}

HOOK(0x004AE510, TFncIrAParapeto_Construct, "auto");
