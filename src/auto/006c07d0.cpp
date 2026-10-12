#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RpMatFXMaterialGetEffects, 0x005712B0, (int a1));
CSF_FUNC(int, __cdecl, RpMatFXMaterialSetEnvMapFrame, 0x005715D0, (int a1, int a2));
CSF_FUNC(int, __cdecl, RpMatFXMaterialSetEnvMapFrameBufferAlpha, 0x00571610, (int a1, int a2, int a3, int a4));
CSF_FUNC(int, __cdecl, RpMatFXMaterialSetUVTransformMatrices, 0x00571920, (int a1, int a2, int a3));
CSF_FUNC(std::uint32_t*, __cdecl, RwMatrixCreate, 0x00624D40, ());
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, GlobalInt_ResolveAccessor, 0x006B13A0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GfxObjectProps_GetObjectAttachment, 0x006B87A0, (int objectIndex));
CSF_FUNC(int, __fastcall, C_0085e7b8_FindEntryPayloadByKey, 0x006C4870, (C_0085e7b8* self, void* , int key));
CSF_FUNC(int, __fastcall, C_008670f0_SetEntry, 0x006C4B00, (C_008670f0* self, void* , int key, int value));

static int __cdecl GfxFrames_ConfigureMaterialEffects(int object, int attachmentOwner)
{
    void* geometry = CSF_AT(void*, reinterpret_cast<void*>(object), 0x18);

    if (!attachmentOwner)
    {
        int attachment = GfxObjectProps_GetObjectAttachment(object);
        attachmentOwner = attachment
            ? CSF_AT(int, reinterpret_cast<void*>(attachment), 0x8)
            : 0;
    }

    const int materialCount = CSF_AT(int, geometry, 0x24);
    for (int index = 0; index < materialCount; ++index)
    {
        void* materials = CSF_AT(void*, geometry, 0x20);
        int material = CSF_AT(int, materials, index * 4);
        int effect = RpMatFXMaterialGetEffects(material);

        if (effect == 2)
        {
            int global = GetGlobalInt();
            int accessor = GlobalInt_ResolveAccessor(
                reinterpret_cast<void*>(global), nullptr);
            int frame = CSF_AT(int, reinterpret_cast<void*>(accessor), 0x6C);

            RpMatFXMaterialSetEnvMapFrame(material, frame);
            // The preceding call's arguments remain on the original stack.
            RpMatFXMaterialSetEnvMapFrameBufferAlpha(material, 0, material, frame);
        }
        else if (effect == 5 && attachmentOwner)
        {
            void* owner = reinterpret_cast<void*>(attachmentOwner);
            void* entries =
                (reinterpret_cast<void* (__fastcall*)(void*, void*)>(
                    CSF_AT(void**, owner, 0)[7]))(owner, nullptr);

            int* entry = reinterpret_cast<int*>(
                C_0085e7b8_FindEntryPayloadByKey(
                    reinterpret_cast<C_0085e7b8*>(entries), nullptr, 7));
            void* transform = entry ? reinterpret_cast<void*>(*entry) : nullptr;

            if (!transform)
            {
                transform = Mem_Alloc(12);
                if (transform)
                {
                    CSF_AT(int, transform, 0) = material;
                    CSF_AT(std::uint32_t*, transform, 0x4) = RwMatrixCreate();
                    CSF_AT(std::uint8_t, transform, 0x8) = 1;
                }

                entries =
                    (reinterpret_cast<void* (__fastcall*)(void*, void*)>(
                        CSF_AT(void**, owner, 0)[7]))(owner, nullptr);
                C_008670f0_SetEntry(
                    reinterpret_cast<C_008670f0*>(entries), nullptr,
                    7, reinterpret_cast<int>(transform));
            }

            RpMatFXMaterialSetUVTransformMatrices(
                material, CSF_AT(int, transform, 0x4), 0);
        }
    }

    return materialCount;
}

HOOK(0x006C07D0, GfxFrames_ConfigureMaterialEffects, "auto");
