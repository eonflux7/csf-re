#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Embedded subobject initializer at +0x10 (bound as in packet).
CSF_FUNC(unsigned int*, __fastcall, InitEmbeddedSubobject, 0x00524000, (void* self, void* edx));

static uint32_t* __fastcall CSoundManager_Construct(CSoundManager* self, void* /*edx*/)
{
    self->field_0004 = (uint32_t)(uintptr_t)CSF_ADDR_DATA(0x0082D428);
    self->field_0008 = (uint32_t)(uintptr_t)CSF_VTBL(0x00840FE4);
    CSF_AT(void*, self, 0x0) = CSF_VTBL(0x00841088);
    self->field_0004 = (uint32_t)(uintptr_t)CSF_ADDR_DATA(0x0084106C);
    self->field_0008 = (uint32_t)(uintptr_t)CSF_ADDR_DATA(0x00841064);

    InitEmbeddedSubobject((void*)&self->field_0010, nullptr);

    self->m_slots = 0;
    self->field_0050 = 0;
    self->field_0054 = 0;
    self->m_slotCapacity = 10;
    self->m_masterVolume = 1.0f;
    self->m_effectsVolume = 1.0f;
    self->field_0068 = 0;
    self->field_006c = 0;
    self->field_0070 = 0;
    self->field_000c = 0;
    self->m_instancePool = 0;
    self->field_005c = 0;
    self->field_0044 = 0;
    self->m_musicStream = 0;
    self->field_0079 = 0;
    self->field_0074 = 0;
    self->field_0078 = 0;

    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x00524080, CSoundManager_Construct, "auto");
