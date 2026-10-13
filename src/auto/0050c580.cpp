#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringRange_429B60, 0x00429B60, (void* self, void* edx, char* text, unsigned int length));
CSF_FUNC(int, __cdecl, MissionScript_DestroyElementRange, 0x00469760, (int* first, int* last));
CSF_FUNC(unsigned int, __fastcall, GameBddLoader_ConfigureDisplayState, 0x0050C450, (void* self, void* edx, unsigned int a2));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));

CSF_GLOBAL(uint8_t, g_byte_82E65C, 0x0082E65C);

static CMenuScreenFader* __fastcall CMenuScreenFader_Construct(
    CMenuScreenFader* self, void* /*edx*/)
{
    self->vtbl = CSF_VTBL(0x0084004C);

    auto* name = reinterpret_cast<uint8_t*>(self) + 4;
    auto* displayState = reinterpret_cast<uint8_t*>(self) + 0x38;

    CSF_AT(uint32_t, self, 0x1C) = 15;
    self->field_0018 = 0;
    *reinterpret_cast<uint8_t*>(&self->field_0008) = 0;
    CSF_AT(uint32_t, self, 0x3C) = 0;
    CSF_AT(uint32_t, self, 0x40) = 0;
    CSF_AT(uint32_t, self, 0x44) = 0;
    self->field_0048 = 0;

    AssignStringRange_429B60(
        name, nullptr, reinterpret_cast<char*>(&g_byte_82E65C), 0);

    const uint32_t allocation = self->field_0048;
    self->m_bIdle = 1;
    self->field_0030 = 0.0f;
    self->m_state = 0;
    self->m_fadeProgress = 0.0f;

    if (allocation)
    {
        CoreMemory_Free(reinterpret_cast<void*>(allocation));
        self->field_0048 = 0;
    }

    self->m_pOverlay = 0;

    const uint32_t first = CSF_AT(uint32_t, self, 0x3C);
    if (first)
    {
        MissionScript_DestroyElementRange(
            reinterpret_cast<int*>(first),
            reinterpret_cast<int*>(CSF_AT(uint32_t, self, 0x40)));

        if (CSF_AT(uint32_t, self, 0x3C))
            Mem_FreeDebug(reinterpret_cast<void*>(
                CSF_AT(uint32_t, self, 0x3C)));
    }

    CSF_AT(uint32_t, self, 0x3C) = 0;
    CSF_AT(uint32_t, self, 0x40) = 0;
    CSF_AT(uint32_t, self, 0x44) = 0;
    GameBddLoader_ConfigureDisplayState(displayState, nullptr, 4);

    return self;
}

HOOK(0x0050C580, CMenuScreenFader_Construct, "auto");
