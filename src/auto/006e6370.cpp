#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __cdecl, RpAtomicSetGeometry, 0x0058EFE0, (uint32_t* a1, int a2, char a3));
CSF_FUNC(int, __cdecl, RpGeometryDestroy, 0x00590FC0, (int a1));
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(void, __fastcall, C_00860170_Construct, 0x006B2830, (C_00860170* self, void*));
CSF_FUNC(int, __fastcall, C_0085e7b8_FindEntryPayloadByKey, 0x006C4870, (C_0085e7b8* self, void* , int key));
CSF_FUNC(int, __fastcall, C_00861c08_ReleaseAtomicState, 0x006E60A0, (void* self, void* edx));

static void __fastcall C_00861c08_Destroy(C_00861c08* self, void* /*edx*/)
{
    *reinterpret_cast<void***>(self) = CSF_VTBL(0x00861CE0);
    self->field_0004 = reinterpret_cast<uint32_t>(CSF_VTBL(0x00861CD4));
    self->field_0008 = reinterpret_cast<uint32_t>(CSF_VTBL(0x00861CB8));

    C_00861c08_ReleaseAtomicState(self, nullptr);

    if (self->field_000c)
    {
        if (CSF_AT(uint32_t, self, 0x1C))
        {
            RpAtomicSetGeometry(
                reinterpret_cast<uint32_t*>(self->field_000c),
                CSF_AT(int, self, 0x1C), 1);
        }

        auto* geometrySlot = reinterpret_cast<uint8_t*>(self) + 0x1C;
        for (int remaining = 3; remaining; --remaining, geometrySlot += 4)
        {
            const int geometry = CSF_AT(int, geometrySlot, 0);
            if (geometry)
            {
                RpGeometryDestroy(geometry);
                CSF_AT(uint32_t, geometrySlot, 0) = 0;
            }
        }
    }

    void* owner = reinterpret_cast<void*>(self->field_0040);
    auto* entries =
        reinterpret_cast<C_0085e7b8* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, owner, 0)[0x1C / 4])(owner, nullptr);

    void* payloadSlot = reinterpret_cast<void*>(
        C_0085e7b8_FindEntryPayloadByKey(entries, nullptr, 5));
    if (payloadSlot)
    {
        void* payload = CSF_AT(void*, payloadSlot, 0);
        if (payload)
        {
            reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, payload, 0)[0])(payload, nullptr, 1);
        }
    }

    owner = reinterpret_cast<void*>(self->field_0040);
    entries =
        reinterpret_cast<C_0085e7b8* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, owner, 0)[0x1C / 4])(owner, nullptr);

    payloadSlot = reinterpret_cast<void*>(
        C_0085e7b8_FindEntryPayloadByKey(entries, nullptr, 7));
    if (payloadSlot)
    {
        void* matrixPayload = CSF_AT(void*, payloadSlot, 0);
        if (matrixPayload)
        {
            if (CSF_AT(uint8_t, matrixPayload, 0x8))
                RwMatrixDestroy(CSF_AT(std::uint32_t*, matrixPayload, 0x4));
            Mem_Free(matrixPayload);
        }
    }

    owner = reinterpret_cast<void*>(self->field_0040);
    if (owner)
    {
        reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, owner, 0)[0x4 / 4])(owner, nullptr, 1);
        self->field_0040 = 0;
    }

    if (self->field_0034)
        Mem_FreeDebug(reinterpret_cast<void*>(self->field_0034));

    self->field_0034 = 0;
    // The original clears four bytes despite the declared byte field.
    CSF_AT(uint32_t, &self->field_0038, 0) = 0;
    self->field_003c = 0;

    self->field_0008 = reinterpret_cast<uint32_t>(CSF_VTBL(0x0082D410));
    *reinterpret_cast<void***>(self) = CSF_VTBL(0x00861C08);
    self->field_0004 = reinterpret_cast<uint32_t>(CSF_VTBL(0x00861C00));
    self->field_0004 = reinterpret_cast<uint32_t>(CSF_VTBL(0x0082D410));

    C_00860170_Construct(reinterpret_cast<C_00860170*>(self), nullptr);
}

HOOK(0x006E6370, C_00861c08_Destroy, "auto");
