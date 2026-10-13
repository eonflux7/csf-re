#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));
CSF_FUNC(void, __fastcall, C_00860170_Construct, 0x006B2830, (C_00860170* self, void*));
CSF_FUNC(void**, __fastcall, ReleaseManagedResource, 0x006D0970, (void* self, void* edx, void** a2));

static void __fastcall C_00860db0_Destroy(C_00860db0* self, void* /*edx*/)
{
    const uint32_t firstCount = self->field_000c;
    *reinterpret_cast<void***>(self) = CSF_VTBL(0x00860DB0);

    if (firstCount)
    {
        uint32_t index = 0;
        do
        {
            void* resource = reinterpret_cast<void**>(self->field_0004)[index];
            if (resource)
            {
                reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
                    CSF_AT(void**, resource, 0)[1])(resource, nullptr, 1);
                reinterpret_cast<void**>(self->field_0004)[index] = nullptr;
            }
            ++index;
        } while (index < self->field_000c);
    }

    const uint32_t secondCount = self->field_001c;
    self->field_000c = 0;
    if (secondCount)
    {
        uint32_t index = 0;
        do
        {
            void* resource = reinterpret_cast<void**>(self->field_0014)[index];
            if (resource)
            {
                reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
                    CSF_AT(void**, resource, 0)[1])(resource, nullptr, 1);
                reinterpret_cast<void**>(self->field_0014)[index] = nullptr;
            }
            ++index;
        } while (index < self->field_001c);
    }

    const bool hasManagedResources = self->field_0030 != 0;
    self->field_001c = 0;
    if (hasManagedResources)
    {
        do
        {
            void* firstNode = CSF_AT(
                void*, reinterpret_cast<void*>(self->field_002c), 0);
            void* resourceHolder = CSF_AT(void*, firstNode, 8);
            ReleaseManagedResource(
                self, nullptr, CSF_AT(void**, resourceHolder, 0));
        } while (self->field_0030);
    }

    void* sentinel = reinterpret_cast<void*>(self->field_002c);
    void* node = CSF_AT(void*, sentinel, 0);
    CSF_AT(void*, sentinel, 0) = sentinel;

    sentinel = reinterpret_cast<void*>(self->field_002c);
    CSF_AT(void*, sentinel, 4) = sentinel;

    const bool hasNodes = node != reinterpret_cast<void*>(self->field_002c);
    self->field_0030 = 0;
    if (hasNodes)
    {
        do
        {
            void* nextNode = CSF_AT(void*, node, 0);
            Mem_FreeDebug(node);
            node = nextNode;
        } while (node != reinterpret_cast<void*>(self->field_002c));
    }

    if (self->field_002c)
        Mem_FreeDebug(reinterpret_cast<void*>(self->field_002c));
    self->field_002c = 0;

    if (self->field_0014)
    {
        CoreMemory_Free(reinterpret_cast<void*>(self->field_0014));
        self->field_0014 = 0;
    }

    if (self->field_0004)
    {
        CoreMemory_Free(reinterpret_cast<void*>(self->field_0004));
        self->field_0004 = 0;
    }

    C_00860170_Construct(reinterpret_cast<C_00860170*>(self), nullptr);
}

HOOK(0x006D0A00, C_00860db0_Destroy, "auto");
