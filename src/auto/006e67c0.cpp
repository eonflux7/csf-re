#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, GlobalInt_ResolveAccessor, 0x006B13A0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GfxFrames_CollectGeometryMaterials, 0x006C0BF0, (int geometryHandle, int collectorHandle));
CSF_FUNC(int, __stdcall, GlobalInt_FindNameId, 0x006C1770, (uint8_t* a1));
CSF_FUNC(char, __fastcall, GfxObjectInstances_AppendMaterialReference, 0x006C1C00, (void* self, void* edx, uint32_t* a3));
CSF_FUNC(char*, __cdecl, GfxObjectInstances_FormatAlternateName, 0x006E5750, (char* Buffer, char* Source, int a3));
CSF_FUNC(int, __fastcall, C_00861c08_ClearMaterialReferences, 0x006E60A0, (void* self, void* edx));
CSF_FUNC(unsigned int, __fastcall, GfxObjectInstances_ReserveMaterialReferences, 0x006E66D0, (void* self, void* edx, unsigned int a2));

static char __fastcall C_00861c08_RebuildAlternateMaterials(
    C_00861c08* self, void* /*edx*/)
{
    C_00861c08_ClearMaterialReferences(self, nullptr);

    const int alternateIndex = static_cast<int>(self->field_002c);
    if (alternateIndex < 0)
        return static_cast<char>(alternateIndex);

    const uint32_t geometryOwnerAddress = self->field_000c;
    if (!geometryOwnerAddress)
        return 0;

    void* geometryOwner = reinterpret_cast<void*>(geometryOwnerAddress);
    const int geometryHandle = CSF_AT(int, geometryOwner, 0x18);
    if (!geometryHandle)
        return static_cast<char>(geometryOwnerAddress);

    void* globalObject = reinterpret_cast<void*>(GetGlobalInt());
    void* accessor = reinterpret_cast<void*>(
        GlobalInt_ResolveAccessor(globalObject, nullptr));
    ((int(__fastcall*)(void*, void*))CSF_AT(void**, accessor, 0)[0x10 / 4])(
        accessor, nullptr);

    alignas(8) uint8_t collectorBytes[0x14];
    CSF_AT(uint32_t, collectorBytes, 0x00) = 0;
    CSF_AT(uint32_t, collectorBytes, 0x04) = 0;
    CSF_AT(uint32_t, collectorBytes, 0x08) = 0;
    CSF_AT(int, collectorBytes, 0x0C) = 10;

    GfxFrames_CollectGeometryMaterials(
        geometryHandle, reinterpret_cast<int>(collectorBytes));

    GfxObjectInstances_ReserveMaterialReferences(
        reinterpret_cast<uint8_t*>(self) + 0x30, nullptr,
        CSF_AT(uint32_t, collectorBytes, 0x08));

    alignas(8) uint8_t nameBuffer[0x200];
    uint32_t materialReference;
    CSF_AT(uint32_t, collectorBytes, 0x10) = 0;

    while (CSF_AT(uint32_t, collectorBytes, 0x10) <
           CSF_AT(uint32_t, collectorBytes, 0x08))
    {
        void* materials = reinterpret_cast<void*>(
            CSF_AT(uint32_t, collectorBytes, 0x00));
        void* material = CSF_AT(
            void*, materials, 4 * CSF_AT(uint32_t, collectorBytes, 0x10));
        uint32_t nameId = CSF_AT(uint32_t, material, 0);

        if (nameId)
        {
            char* sourceName = reinterpret_cast<char*>(nameId) + 0x10;
            GfxObjectInstances_FormatAlternateName(
                reinterpret_cast<char*>(nameBuffer), sourceName,
                static_cast<int>(self->field_002c));
            nameId = static_cast<uint32_t>(GlobalInt_FindNameId(nameBuffer));

            if (!nameId && static_cast<int>(self->field_002c) > 0)
            {
                GfxObjectInstances_FormatAlternateName(
                    reinterpret_cast<char*>(nameBuffer), sourceName, 0);
                nameId = static_cast<uint32_t>(GlobalInt_FindNameId(nameBuffer));
            }
        }

        materialReference = nameId;
        if (nameId)
            ++CSF_AT(uint32_t, reinterpret_cast<void*>(nameId), 0x54);

        GfxObjectInstances_AppendMaterialReference(
            reinterpret_cast<uint8_t*>(self) + 0x30, nullptr,
            &materialReference);
        ++CSF_AT(uint32_t, collectorBytes, 0x10);
    }

    void* collectedStorage = reinterpret_cast<void*>(
        CSF_AT(uint32_t, collectorBytes, 0x00));
    if (collectedStorage)
        return static_cast<char>(Mem_Free(collectedStorage));
    return 0;
}

HOOK(0x006E67C0, C_00861c08_RebuildAlternateMaterials, "auto");
