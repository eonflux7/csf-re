#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, InitializePrimaryState, 0x004DE700, (void* self, void* edx));
CSF_FUNC(int, __fastcall, C_0083e124_ReleaseResources, 0x004DEA50, (void* self, void* edx));
CSF_FUNC(char, __fastcall, InitializeSecondaryState, 0x004DF7E0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, ReleaseSecondaryState, 0x004E00F0, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, ConstructSecondaryState, 0x004E03F0, (void* self, void* edx));
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(char, __fastcall, UnregisterState, 0x006B1A30, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, RegisterState, 0x006B1E50, (void* self, void* edx, void** a2));

static uint8_t __fastcall C_0083e124_Initialize(C_0083e124* self, void* /*edx*/)
{
    UnregisterState(reinterpret_cast<void*>(GetGlobalInt()), nullptr,
                    reinterpret_cast<int>(self));

    if (self->field_0004)
    {
        C_0083e124_ReleaseResources(self, nullptr);
        self->field_0004 = 0;
    }

    self->field_0018 = 0;
    self->field_001c = 0;
    CSF_AT(uint32_t, self, 0x0C) = 0;
    CSF_AT(uint32_t, self, 0x08) = 0;
    CSF_AT(int32_t, self, 0x10) = 1287568416;
    CSF_AT(int32_t, self, 0x14) = -859915232;
    CSF_AT(uint8_t, self, 0x05) = 0;
    self->field_0040 = 0;

    char initialized = 1;
    void* primaryState = Mem_Alloc(40);
    if (primaryState)
        CSF_AT(uint8_t, primaryState, 0x00) = 0;

    self->field_0018 = reinterpret_cast<uint32_t>(primaryState);
    if (primaryState)
    {
        initialized = InitializePrimaryState(primaryState, nullptr);
        if (!initialized)
            C_0083e124_ReleaseResources(self, nullptr);
    }

    self->field_0004 = initialized;
    if (initialized)
    {
        void* storage = Mem_Alloc(48);
        uint32_t* secondaryState = nullptr;
        if (storage)
        {
            // The packet also identifies this constructor address as a vtable reference.
            (void)CSF_VTBL(0x004E03F0);
            secondaryState = ConstructSecondaryState(storage, nullptr);
        }

        self->field_001c = reinterpret_cast<uint32_t>(secondaryState);
        if (secondaryState)
        {
            char secondaryInitialized = InitializeSecondaryState(
                secondaryState, nullptr, reinterpret_cast<int>(self));
            self->field_0004 = secondaryInitialized;
            if (!secondaryInitialized)
            {
                void* failedState = reinterpret_cast<void*>(self->field_001c);
                if (failedState)
                {
                    ReleaseSecondaryState(failedState, nullptr);
                    Mem_Free(failedState);
                    self->field_001c = 0;
                }
            }
        }
    }

    RegisterState(reinterpret_cast<void*>(GetGlobalInt()), nullptr,
                  reinterpret_cast<void**>(&self->vtbl));
    return self->field_0004;
}

HOOK(0x004DEE90, C_0083e124_Initialize, "auto");
