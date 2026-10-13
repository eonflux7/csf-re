#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, S_0041e5f0_GrowArrayCapacity, 0x0041E5F0, (S_0041e5f0* self, void* , int additionalCapacity));
CSF_FUNC(void, __cdecl, SceneLoader_NoOp, 0x00506AC0, ());
CSF_FUNC(int, __cdecl, RpWorldAddClump, 0x0058E280, (int a1, uint32_t* a2));
CSF_FUNC(int, __fastcall, SceneLoader_GetSceneController, 0x00652010, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, GetReferencedObject, 0x006683F0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(void*, __fastcall, MenuControls_ResolveObject, 0x006B1390, (void* self, void* edx));
CSF_FUNC(int, __fastcall, AttachmentRecord_Resolve, 0x006B8230, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GfxObjectProps_GetObjectAttachment, 0x006B87A0, (int objectIndex));
CSF_FUNC(int, __fastcall, C_0085ec18_GetEntryByIndex, 0x006BCA10, (C_0085ec18* self, void* , int index));
CSF_FUNC(int, __cdecl, GfxFrames_GetProviderFrameObject, 0x006C0500, (int clump));
CSF_FUNC(int, __cdecl, GetGlobalValue_6C5760, 0x006C5760, ());
CSF_FUNC(int, __fastcall, C_00860288_AddEntryAndRegister, 0x006C6D40, (C_00860288* self, void* , int a1));
CSF_FUNC(S_006fe0c0*, __cdecl, GfxClump_CreateWaterEffect, 0x006FE1D0, (C_0085ec18* a2, int arg4));

// Address marker listed in the packet; the instructions actually call this routine.
[[maybe_unused]] static const auto g_ArrayCapacityVtableMarker =
    CSF_VTBL(0x0041E5F0);

static unsigned int __fastcall C_0085e7b8_RefreshControllerState(
    C_0085e7b8* self, void* /*edx*/)
{
    const int globalObject = GetGlobalInt();
    void* controller = reinterpret_cast<void*>(
        SceneLoader_GetSceneController(reinterpret_cast<void*>(globalObject), nullptr));

    C_0085ec18* entries =
        reinterpret_cast<C_0085ec18* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, controller, 0)[0x40 / 4])(controller, nullptr);

    unsigned int index = 0;
    unsigned int count = static_cast<unsigned int>(
        reinterpret_cast<std::uintptr_t>(MenuControls_ResolveObject(entries, nullptr)));

    if (count == 0)
        return count;

    do
    {
        const int entryAddress =
            C_0085ec18_GetEntryByIndex(entries, nullptr, static_cast<int>(index));
        auto* entry = reinterpret_cast<C_0085ec18*>(entryAddress);
        const int providerFrame = GfxFrames_GetProviderFrameObject(entryAddress);
        const int attachment = GfxObjectProps_GetObjectAttachment(providerFrame);

        SceneLoader_NoOp();

        const int attachmentType =
            AttachmentRecord_Resolve(reinterpret_cast<void*>(attachment), nullptr);

        if (attachmentType != 0 && attachmentType != 1)
        {
            S_006fe0c0* waterEffect =
                GfxClump_CreateWaterEffect(entry, reinterpret_cast<int>(self));

            if (self->field_000c >= self->field_0008)
            {
                S_0041e5f0_GrowArrayCapacity(
                    reinterpret_cast<S_0041e5f0*>(
                        reinterpret_cast<std::uint8_t*>(self) + 4),
                    nullptr, CSF_AT(int, self, 0x10));
            }

            const std::uint32_t size = self->field_000c;
            auto* destination =
                reinterpret_cast<S_006fe0c0**>(self->field_0004) + size;
            self->field_000c = size + 1;
            *destination = waterEffect;
        }
        else
        {
            void* world = GetReferencedObject(entries, nullptr);
            RpWorldAddClump(reinterpret_cast<int>(world),
                            reinterpret_cast<uint32_t*>(entry));

            const int registry = GetGlobalValue_6C5760();
            C_00860288_AddEntryAndRegister(
                reinterpret_cast<C_00860288*>(registry), nullptr, providerFrame);

            auto* array = reinterpret_cast<S_0041e5f0*>(
                reinterpret_cast<std::uint8_t*>(self) +
                (attachmentType == 1 ? 0x24 : 0x14));

            if (array->field_0008 >= array->field_0004)
            {
                S_0041e5f0_GrowArrayCapacity(
                    array, nullptr, array->field_000c);
            }

            const std::uint32_t size = array->field_0008;
            auto* destination =
                reinterpret_cast<C_0085ec18**>(array->field_0000) + size;
            array->field_0008 = size + 1;
            *destination = entry;
        }

        ++index;
        count = static_cast<unsigned int>(
            reinterpret_cast<std::uintptr_t>(
                MenuControls_ResolveObject(entries, nullptr)));
    } while (index < count);

    return count;
}

HOOK(0x006D5E20, C_0085e7b8_RefreshControllerState, "auto");
