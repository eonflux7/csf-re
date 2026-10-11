#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"

// Bind-as lines copied from the packet.
CSF_FUNC(int, __fastcall, SceneObj_IsInactive, 0x0048E100, (void* self, void* edx));
CSF_FUNC(int, __fastcall, sub_4D73E0, 0x004D73E0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CharTaskBase_Prepare, 0x004DB110, (void* self, void* edx));

// Prepares the conversation task: if the owner's scene object is active, runs the
// owner-side update (sub_4D73E0 on owner+0x224), sets flag bit 2 in the owner's
// flags at +0x18, then tail-calls CharTaskBase_Prepare on this task.
static int __fastcall TConversacion_PrepareTask(void* self, void* /*edx*/)
{
    uint8_t* owner = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, self, 0x08));

    void* ownerSceneObj = reinterpret_cast<void*>(CSF_AT(uint32_t, owner, 0x14));
    if (!SceneObj_IsInactive(ownerSceneObj, nullptr))
    {
        sub_4D73E0(owner + 0x224, nullptr);
    }

    CSF_AT(uint32_t, owner, 0x18) |= 2u;

    // Tail jump: returns CharTaskBase_Prepare's result unchanged.
    return CharTaskBase_Prepare(self, nullptr);
}

HOOK(0x004D8470, TConversacion_PrepareTask, "auto");
