#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"
#include "core/memory.h"

CSF_FUNC(void, __fastcall, ApplyGlobalValueSettings, 0x005233D0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, sub_526CA0, 0x00526CA0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, sub_522DB0, 0x00522DB0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Resets a CSoundManager: reinstalls its vtables and string/data tags, applies
// global volume settings, frees the instance pool and the positional slot table,
// re-initialises the sub-object at +0x10, then restores the final tags.
static int __fastcall CSoundManager_ReleasePoolsAndReset(CSoundManager* self, void* /*edx*/)
{
    self->vtbl = CSF_VTBL(0x841088);
    self->field_0004 = reinterpret_cast<uint32_t>(CSF_ADDR_DATA(0x84106C));
    self->field_0008 = reinterpret_cast<uint32_t>(CSF_ADDR_DATA(0x841064));

    ApplyGlobalValueSettings(self, nullptr);

    uint32_t instancePool = self->m_instancePool;
    if (instancePool) {
        sub_526CA0(reinterpret_cast<void*>(instancePool), nullptr);
        Mem_Free(reinterpret_cast<void*>(instancePool));
        self->m_instancePool = 0;
    }

    if (self->m_slots) {
        Mem_Free(reinterpret_cast<void*>(self->m_slots));
        self->m_slots = 0;
    }

    int result = sub_522DB0(reinterpret_cast<void*>(&self->field_0010), nullptr);

    self->field_0008 = reinterpret_cast<uint32_t>(CSF_ADDR_DATA(0x840FE4));
    self->field_0004 = reinterpret_cast<uint32_t>(CSF_ADDR_DATA(0x82D410));
    return result;
}

HOOK(0x005238B0, CSoundManager_ReleasePoolsAndReset, "auto");
