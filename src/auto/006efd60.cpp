#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RpPTankAtomicDestroy, 0x005821A0, (uint32_t* a1));
CSF_FUNC(int, __cdecl, RpAtomicSetFrame, 0x0058F300, (int a1, int a2));
CSF_FUNC(int, __cdecl, RwFrameDestroy, 0x00623B10, (int frame));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(void, __fastcall, RpPrtStd_ResetBaseState, 0x006C9FC0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SceneLoader_GetObjectRegistry, 0x00726C40, (void* self, void* edx));

static void __fastcall C_00862240_ClearParticles(
    C_00862240* self, void* /*edx*/)
{
    void* global = reinterpret_cast<void*>(GetGlobalInt());
    void* registry = reinterpret_cast<void*>(
        SceneLoader_GetObjectRegistry(global, nullptr));

    for (uint32_t i = 0; i < self->field_00b4; ++i)
    {
        if (self->field_00cc)
        {
            uint32_t object = reinterpret_cast<uint32_t*>(self->field_00ac)[i];
            reinterpret_cast<void(__fastcall*)(void*, void*, uint32_t)>(
                CSF_AT(void**, registry, 0)[0x18 / 4])(
                    registry, nullptr, object);
        }
        else
        {
            void* object = reinterpret_cast<void*>(
                reinterpret_cast<uint32_t*>(self->field_00ac)[i]);
            if (object)
            {
                reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
                    CSF_AT(void**, object, 0)[0x4 / 4])(
                        object, nullptr, 1);
                reinterpret_cast<uint32_t*>(self->field_00ac)[i] = 0;
            }
        }
    }

    uint32_t atomicCount = self->field_00c4;
    self->field_00b4 = 0;
    if (atomicCount)
    {
        uint32_t i = 0;
        do
        {
            uint32_t atomic =
                reinterpret_cast<uint32_t*>(self->field_00bc)[i];
            int frame = CSF_AT(int, reinterpret_cast<void*>(atomic), 0x4);
            RpAtomicSetFrame(static_cast<int>(atomic), 0);
            RwFrameDestroy(frame);
            RpPTankAtomicDestroy(reinterpret_cast<uint32_t*>(
                reinterpret_cast<uint32_t*>(self->field_00bc)[i]));
            ++i;
        }
        while (i < self->field_00c4);
    }

    void** embeddedVtable = reinterpret_cast<void**>(self->field_0024);
    self->field_00c4 = 0;
    reinterpret_cast<void(__fastcall*)(void*, void*)>(
        embeddedVtable[0x24 / 4])(
            reinterpret_cast<uint8_t*>(self) + 0x24, nullptr);

    RpPrtStd_ResetBaseState(self, nullptr);
}

HOOK(0x006EFD60, C_00862240_ClearParticles, "auto");
