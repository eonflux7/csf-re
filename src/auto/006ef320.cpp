#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __cdecl, CoreMemory_AllocUntagged, 0x006910F0, (std::size_t size));
CSF_FUNC(C_00860178*, __fastcall, C_00860178_Construct, 0x006B2820, (C_00860178* self, void*));
CSF_FUNC(void*, __fastcall, GfxFrames_InitIdentityFrame, 0x006BF1E0, (void* self, void*));
CSF_FUNC(int, __fastcall, GfxObject_ConstructEmbeddedState, 0x006BFD00, (C_00860700* self, void*));
CSF_FUNC(int, __fastcall, C_00860894_Init, 0x006C45F0, (C_00860894* self, void*, int a2, int a3));

static C_00862110* __fastcall C_00862110_Construct(C_00862110* self, void* /*edx*/)
{
    C_00860178_Construct(self, nullptr);
    self->vtbl = CSF_VTBL(0x00862110);

    GfxObject_ConstructEmbeddedState(reinterpret_cast<C_00860700*>(&self->field_0008), nullptr);
    self->field_0030 = 0;
    self->field_0004 = 0;

    void* helperBlock = CoreMemory_AllocUntagged(28);
    if (helperBlock)
    {
        C_00860178_Construct(reinterpret_cast<C_00860178*>(helperBlock), nullptr);
        CSF_AT(void**, helperBlock, 0) = CSF_VTBL(0x00862158);
        C_00860894_Init(reinterpret_cast<C_00860894*>(static_cast<uint8_t*>(helperBlock) + 8), nullptr, -1, 0);
        CSF_AT(void*, helperBlock, 4) = self;
        self->field_0034 = reinterpret_cast<uint32_t>(helperBlock);
    }
    else
    {
        self->field_0034 = 0;
    }

    GfxFrames_InitIdentityFrame(reinterpret_cast<void*>(&self->field_0008), nullptr);
    return self;
}

HOOK(0x6EF320, C_00862110_Construct, "auto");
