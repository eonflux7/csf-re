#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, InitS00402350Object, 0x00402350, (void* self, void* edx));
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(uint32_t*, __fastcall, GfxObjectProps_EnsureMaterialProperties, 0x006B8020, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GfxObjectProps_ReleaseMaterialReference, 0x006B96C0, (void* self, void* edx, int* a2));
CSF_FUNC(char, __fastcall, GfxObjectProps_ResetProperties, 0x006B9930, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, GfxObjectProps_CopyMaterialReference, 0x006BA370, (void* self, void* edx, void* a2));

CSF_GLOBAL(void*, g_pRwEngineInstance, 0x008B68C0);
CSF_GLOBAL(int, g_MaterialManagerOffset, 0x008BB474);
CSF_GLOBAL(int, g_ObjectPropertiesOffset, 0x008BB478);

// Constructor entries also identified as vtable references in the packet.
[[maybe_unused]] static auto const kMaterialPropertiesVtable = CSF_VTBL(0x006B8020);
[[maybe_unused]] static auto const kAlphaPlaneVtable = CSF_VTBL(0x00402350);

static int __cdecl GfxObjectProps_CopyObjectProperties(
    int destinationObject, S_006b9580* sourceObject)
{
    const int propertiesOffset = g_ObjectPropertiesOffset;
    void* destination = CSF_AT(
        void*, reinterpret_cast<uint8_t*>(destinationObject) + propertiesOffset, 0x4);
    void* source = CSF_AT(
        void*, reinterpret_cast<uint8_t*>(sourceObject) + propertiesOffset, 0x4);

    GfxObjectProps_ResetProperties(destination, nullptr);
    CSF_AT(uint32_t, destination, 0x0) = CSF_AT(uint32_t, source, 0x0);
    CSF_AT(uint32_t, destination, 0x4) |= CSF_AT(uint32_t, source, 0x4);
    CSF_AT(uint32_t, destination, 0x8) = 0;

    if (CSF_AT(void*, source, 0x10))
    {
        GfxObjectProps_EnsureMaterialProperties(destination, nullptr);

        void* material = CSF_AT(void*, CSF_AT(void*, source, 0x10), 0x0);
        int* destinationSlot = CSF_AT(int*, destination, 0x10);
        if (material)
        {
            void* manager = CSF_AT(
                void*, static_cast<uint8_t*>(g_pRwEngineInstance) + g_MaterialManagerOffset, 0x0);
            GfxObjectProps_ReleaseMaterialReference(manager, nullptr, destinationSlot);
            void* copiedMaterial =
                GfxObjectProps_CopyMaterialReference(manager, nullptr, material);
            CSF_AT(void*, destinationSlot, 0x0) = copiedMaterial;
        }
        else
        {
            CSF_AT(int, destinationSlot, 0x0) = 0;
        }

        material = CSF_AT(void*, CSF_AT(void*, source, 0x10), 0x4);
        destinationSlot = reinterpret_cast<int*>(
            static_cast<uint8_t*>(CSF_AT(void*, destination, 0x10)) + 4);
        if (material)
        {
            void* manager = CSF_AT(
                void*, static_cast<uint8_t*>(g_pRwEngineInstance) + g_MaterialManagerOffset, 0x0);
            GfxObjectProps_ReleaseMaterialReference(manager, nullptr, destinationSlot);
            void* copiedMaterial =
                GfxObjectProps_CopyMaterialReference(manager, nullptr, material);
            CSF_AT(void*, destinationSlot, 0x0) = copiedMaterial;
        }
        else
        {
            CSF_AT(int, destinationSlot, 0x0) = 0;
        }

        CSF_AT(uint8_t, CSF_AT(void*, destination, 0x10), 0xC) =
            CSF_AT(uint8_t, CSF_AT(void*, source, 0x10), 0xC);
        CSF_AT(uint8_t, CSF_AT(void*, destination, 0x10), 0xD) =
            CSF_AT(uint8_t, CSF_AT(void*, source, 0x10), 0xD);
        CSF_AT(uint32_t, CSF_AT(void*, destination, 0x10), 0x14) =
            CSF_AT(uint32_t, CSF_AT(void*, source, 0x10), 0x14);
        CSF_AT(uint32_t, CSF_AT(void*, destination, 0x10), 0x18) =
            CSF_AT(uint32_t, CSF_AT(void*, source, 0x10), 0x18);
        CSF_AT(uint32_t, CSF_AT(void*, destination, 0x10), 0x1C) =
            CSF_AT(uint32_t, CSF_AT(void*, source, 0x10), 0x1C);
        CSF_AT(uint32_t, CSF_AT(void*, destination, 0x10), 0x20) =
            CSF_AT(uint32_t, CSF_AT(void*, source, 0x10), 0x20);
        CSF_AT(uint32_t, CSF_AT(void*, destination, 0x10), 0x24) =
            CSF_AT(uint32_t, CSF_AT(void*, source, 0x10), 0x24);
        CSF_AT(uint32_t, CSF_AT(void*, destination, 0x10), 0x10) =
            CSF_AT(uint32_t, CSF_AT(void*, source, 0x10), 0x10);
    }
    else
    {
        void* materialProperties = CSF_AT(void*, destination, 0x10);
        if (materialProperties)
        {
            GfxObjectProps_ReleaseMaterialReference(
                CSF_AT(void*, static_cast<uint8_t*>(g_pRwEngineInstance) + g_MaterialManagerOffset, 0x0),
                nullptr, static_cast<int*>(materialProperties));
            GfxObjectProps_ReleaseMaterialReference(
                CSF_AT(void*, static_cast<uint8_t*>(g_pRwEngineInstance) + g_MaterialManagerOffset, 0x0),
                nullptr, reinterpret_cast<int*>(static_cast<uint8_t*>(materialProperties) + 4));
            Mem_Free(materialProperties);
            CSF_AT(void*, destination, 0x10) = nullptr;
        }
    }

    if (CSF_AT(void*, source, 0x14))
    {
        if (!CSF_AT(void*, destination, 0x14))
        {
            void* storage = Mem_Alloc(60);
            uint32_t* plane = storage ? InitS00402350Object(storage, nullptr) : nullptr;
            CSF_AT(void*, destination, 0x14) = plane;
        }

        void* sourcePlane = CSF_AT(void*, source, 0x14);
        void* destinationPlane = CSF_AT(void*, destination, 0x14);
        CSF_AT(uint32_t, destinationPlane, 0x0) = CSF_AT(uint32_t, sourcePlane, 0x0);
        CSF_AT(uint32_t, destinationPlane, 0x4) = CSF_AT(uint32_t, sourcePlane, 0x4);
        CSF_AT(uint32_t, destinationPlane, 0x8) = CSF_AT(uint32_t, sourcePlane, 0x8);
        CSF_AT(uint32_t, destinationPlane, 0xC) = CSF_AT(uint32_t, sourcePlane, 0xC);
        CSF_AT(uint32_t, destinationPlane, 0x10) = CSF_AT(uint32_t, sourcePlane, 0x10);
        CSF_AT(uint32_t, destinationPlane, 0x14) = CSF_AT(uint32_t, sourcePlane, 0x14);
    }
    else
    {
        void* plane = CSF_AT(void*, destination, 0x14);
        if (plane)
        {
            Mem_Free(plane);
            CSF_AT(void*, destination, 0x14) = nullptr;
        }
    }

    return destinationObject;
}

HOOK(0x006BB030, GfxObjectProps_CopyObjectProperties, "auto");
