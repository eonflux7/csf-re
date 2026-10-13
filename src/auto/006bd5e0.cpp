#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RpWorldForAllWorldSectors, 0x0058CFF0, (int a1, int (__cdecl* a2)(int*, int), int a3));
CSF_FUNC(char, __fastcall, S_006bc310_InitializeSectorEntries, 0x006BCD10, (void* self, void* edx));
CSF_FUNC(int, __fastcall, ParticleInner_Update, 0x006C4330, (void* self, void*));
CSF_FUNC(int, __cdecl, GetGlobalValue_6C5760, 0x006C5760, ());
CSF_FUNC(int, __stdcall, GfxSkin_ForwardToRwEngineSlot20, 0x006C7B40, (int a1, int a2));

static unsigned int __fastcall S_006bc310_UpdateWorldSectorParticles(
    S_006bc310* self, void* /*edx*/)
{
    void* globalState = reinterpret_cast<void*>(GetGlobalValue_6C5760());
    void* sectorProcessor = CSF_AT(void*, globalState, 0xF0);

    if (!self->field_00a4)
    {
        S_006bc310_InitializeSectorEntries(self, nullptr);
        RpWorldForAllWorldSectors(
            static_cast<int>(self->field_0004),
            reinterpret_cast<int (__cdecl*)(int*, int)>(
                CSF_ADDR_FN(0x006EE680)),
            reinterpret_cast<int>(&self->field_0094));
        self->field_00a4 = 1;
    }

    GetGlobalValue_6C5760();
    GfxSkin_ForwardToRwEngineSlot20(1, 0);

    unsigned int count = self->field_009c;
    unsigned int index = 0;
    uint32_t entryOffset = 0;

    if (count)
    {
        do
        {
            void* entry = reinterpret_cast<void*>(
                self->field_0094 + entryOffset);

            if (((bool (__fastcall*)(void*, void*, void*, int, int))
                    CSF_AT(void**, sectorProcessor, 0)[0x6C / 4])(
                    sectorProcessor, nullptr, entry, 0, 0))
            {
                void* currentEntry = reinterpret_cast<void*>(
                    self->field_0094 + entryOffset);
                ParticleInner_Update(
                    CSF_AT(void*, currentEntry, 0x10), nullptr);
            }

            count = self->field_009c;
            ++index;
            entryOffset += 20;
        }
        while (index < count);
    }

    return count;
}

HOOK(0x006BD5E0, S_006bc310_UpdateWorldSectorParticles, "auto");
