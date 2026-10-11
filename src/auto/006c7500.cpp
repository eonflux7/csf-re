#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __cdecl, GfxSkin_SortEntries, 0x007AEAE0,
         (void* base, std::size_t count, std::size_t size,
          int (__cdecl* compare)(const void*, const void*)));
CSF_FUNC(int, __cdecl, GfxSkin_CompareRenderEntries, 0x006C5AC0,
         (const void* left, const void* right));
CSF_FUNC(int, __fastcall, GfxSkin_CombineTransform, 0x0048E060,
         (void* self, void* edx, int a2, uint32_t* a3));
CSF_FUNC(void, __cdecl, SceneLoader_NoOp, 0x00506AC0, ());
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(std::uint32_t*, __cdecl, RwMatrixCreate, 0x00624D40, ());
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(uint32_t*, __fastcall, GfxSkin_ConstructLookupKey, 0x006B27D0,
         (void* self, void* edx, uint32_t* a2));
CSF_FUNC(unsigned int, __fastcall, GfxSkin_RenderObject, 0x006B3900,
         (void* self, void* edx));
CSF_FUNC(char, __fastcall, GfxSkin_InitializeMatrixBackups, 0x006C7220,
         (void* self, void* edx, unsigned int a2, int a3, char a4));
CSF_FUNC(char, __fastcall, GlobalHud_Commit_6C7C30, 0x006C7C30,
         (void* self, void* edx));
CSF_FUNC(int, __fastcall, GfxSkin_SetRenderStage, 0x006C8390,
         (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, C_008655e8_EndCameraUpdate, 0x006D2BE0,
         (C_008655e8* self, void*));
CSF_FUNC(bool, __fastcall, GfxSkin_BeginCameraPass, 0x006D3010,
         (void* self, void* edx, int arg0, float a3, float a2, float a5, int a6));
CSF_FUNC(int, __fastcall, GfxSkin_UpdateAdditionalObject, 0x006D8360,
         (void* self, void* edx));

CSF_GLOBAL(float, g_ColorBlack, 0x008A7DA0);
CSF_GLOBAL(float, g_ColorBlack_G, 0x008A7DA4);
CSF_GLOBAL(float, g_ColorBlack_B, 0x008A7DA8);
CSF_GLOBAL(float, g_ColorBlack_A, 0x008A7DAC);

static bool __fastcall S_006c5cd0_RenderSkinPass(S_006c5cd0* self, void* /*edx*/)
{
    if (!self->field_00c8)
        return true;

    const std::uint32_t sortedCount = self->field_00d8;
    if (sortedCount)
    {
        GfxSkin_SortEntries(CSF_AT(void*, self, 0xD0), sortedCount, 0xC,
                           GfxSkin_CompareRenderEntries);
    }

    alignas(8) std::uint8_t cameraMatrixBytes[0x8];
    alignas(8) std::uint8_t adjustedMatrixBytes[0x8];
    alignas(8) std::uint8_t objectMatrixBytes[0xC];
    alignas(8) std::uint8_t cameraOrLookupBytes[0x4];
    alignas(8) std::uint8_t alternateLookupBytes[0x4];
    alignas(8) std::uint8_t objectLookupBytes[0x4];
    alignas(8) std::uint8_t firstKeyBytes[0x4];
    alignas(8) std::uint8_t secondKeyBytes[0x4];
    alignas(8) std::uint8_t secondaryLookupBytes[0x8];

    auto* camera = reinterpret_cast<C_008655e8*>(self->field_00f0);
    CSF_AT(void*, cameraOrLookupBytes, 0) =
        reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            camera->vtbl[0x10 / 4])(camera, nullptr);

    C_008655e8_EndCameraUpdate(
        reinterpret_cast<C_008655e8*>(self->field_00f0), nullptr);
    reinterpret_cast<C_008655e8*>(self->field_00f0)
        ->vCreateResourceWrapper(1065353216);
    reinterpret_cast<C_008655e8*>(self->field_00f0)
        ->vCreateTextureWrapper(1148846080);

    const float red = g_ColorBlack;
    const float green = g_ColorBlack_G;
    const float blue = g_ColorBlack_B;
    int alpha;
    std::memcpy(&alpha, &g_ColorBlack_A, sizeof(alpha));
    const bool passStarted = GfxSkin_BeginCameraPass(
        reinterpret_cast<void*>(self->field_00f0), nullptr,
        2, red, green, blue, alpha);
    if (!passStarted)
        return passStarted;

    const std::uint32_t backupCount = self->field_00c8;
    auto* initialMatrix = RwMatrixCreate();
    auto* backupContainer = reinterpret_cast<std::uint8_t*>(self) + 0x138;
    GfxSkin_InitializeMatrixBackups(
        backupContainer, nullptr, backupCount,
        reinterpret_cast<int>(initialMatrix), 1);

    for (std::uint32_t index = 0; index < self->field_00c8; ++index)
    {
        void* entry = CSF_AT(void*, CSF_AT(void*, self, 0xC0), 4 * index);
        GfxSkin_ConstructLookupKey(
            secondKeyBytes, nullptr,
            reinterpret_cast<uint32_t*>(CSF_ADDR_DATA(0x008BB464)));
        const bool found =
            reinterpret_cast<bool (__fastcall*)(void*, void*, void*, void*)>(
                CSF_AT(void**, entry, 0)[0xC / 4])(
                    entry, nullptr, secondKeyBytes, objectLookupBytes);
        SceneLoader_NoOp();

        if (found)
        {
            void* object =
                CSF_AT(void*, CSF_AT(void*, objectLookupBytes, 0), 4);
            if (!reinterpret_cast<bool (__fastcall*)(void*, void*)>(
                    CSF_AT(void**, object, 0)[0xA8 / 4])(object, nullptr))
            {
                void* transform =
                    reinterpret_cast<void* (__fastcall*)(void*, void*)>(
                        CSF_AT(void**, object, 0)[0x18 / 4])(object, nullptr);

                auto* backupSlot =
                    static_cast<std::uint8_t*>(CSF_AT(void*, self, 0x13C))
                    + 8 * index;
                auto* matrixReference =
                    reinterpret_cast<uint32_t* (__fastcall*)(void*, void*, void*)>(
                        CSF_AT(void**, transform, 0)[0x8C / 4])(
                            transform, nullptr, objectMatrixBytes);
                const void* sourceMatrix = CSF_AT(void*, matrixReference, 0);
                void* destinationMatrix = CSF_AT(void*, backupSlot, 0);
                std::memcpy(destinationMatrix, sourceMatrix, 0x40);

                if (CSF_AT(std::uint8_t, objectMatrixBytes, 4))
                    RwMatrixDestroy(CSF_AT(std::uint32_t*, objectMatrixBytes, 0));

                transform =
                    reinterpret_cast<void* (__fastcall*)(void*, void*)>(
                        CSF_AT(void**, object, 0)[0x18 / 4])(object, nullptr);
                void** transformVtable = CSF_AT(void**, transform, 0);
                void* cameraTransform = CSF_AT(void*, cameraOrLookupBytes, 0);
                auto* cameraReference =
                    reinterpret_cast<uint32_t* (__fastcall*)(void*, void*, void*)>(
                        CSF_AT(void**, cameraTransform, 0)[0x8C / 4])(
                            cameraTransform, nullptr, cameraMatrixBytes);
                const int adjustedReference = GfxSkin_CombineTransform(
                    reinterpret_cast<std::uint8_t*>(self) + 0x14, nullptr,
                    reinterpret_cast<int>(adjustedMatrixBytes), cameraReference);
                reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
                    transformVtable[0x94 / 4])(
                        transform, nullptr, adjustedReference);

                if (CSF_AT(std::uint8_t, adjustedMatrixBytes, 4))
                    RwMatrixDestroy(CSF_AT(std::uint32_t*, adjustedMatrixBytes, 0));
                if (CSF_AT(std::uint8_t, cameraMatrixBytes, 4))
                    RwMatrixDestroy(CSF_AT(std::uint32_t*, cameraMatrixBytes, 0));

                if (reinterpret_cast<void* (__fastcall*)(void*, void*)>(
                        CSF_AT(void**, object, 0)[0x68 / 4])(object, nullptr))
                {
                    void* child =
                        reinterpret_cast<void* (__fastcall*)(void*, void*)>(
                            CSF_AT(void**, object, 0)[0x68 / 4])(object, nullptr);
                    reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
                        CSF_AT(void**, child, 0)[0x24 / 4])(child, nullptr, 0);
                }
            }
        }
        else
        {
            CSF_AT(void*, alternateLookupBytes, 0) = nullptr;
            entry = CSF_AT(void*, CSF_AT(void*, self, 0xC0), 4 * index);
            GfxSkin_ConstructLookupKey(
                firstKeyBytes, nullptr,
                reinterpret_cast<uint32_t*>(CSF_ADDR_DATA(0x008BB4C0)));
            reinterpret_cast<bool (__fastcall*)(void*, void*, void*, void*)>(
                CSF_AT(void**, entry, 0)[0xC / 4])(
                    entry, nullptr, firstKeyBytes, alternateLookupBytes);
            SceneLoader_NoOp();
        }
    }

    GfxSkin_SetRenderStage(reinterpret_cast<void*>(self->field_0004), nullptr, 5);
    for (std::uint32_t index = 0; index < self->field_00c8; ++index)
    {
        void* entry = CSF_AT(void*, CSF_AT(void*, self, 0xC0), 4 * index);
        GfxSkin_ConstructLookupKey(
            firstKeyBytes, nullptr,
            reinterpret_cast<uint32_t*>(CSF_ADDR_DATA(0x008BB464)));
        const bool found =
            reinterpret_cast<bool (__fastcall*)(void*, void*, void*, void*)>(
                CSF_AT(void**, entry, 0)[0xC / 4])(
                    entry, nullptr, firstKeyBytes, cameraOrLookupBytes);
        SceneLoader_NoOp();
        if (found)
        {
            GfxSkin_RenderObject(
                CSF_AT(void*, CSF_AT(void*, cameraOrLookupBytes, 0), 4), nullptr);
        }
        else
        {
            CSF_AT(std::uint32_t, secondaryLookupBytes, 0) = 0;
            entry = CSF_AT(void*, CSF_AT(void*, self, 0xC0), 4 * index);
            GfxSkin_ConstructLookupKey(
                secondKeyBytes, nullptr,
                reinterpret_cast<uint32_t*>(CSF_ADDR_DATA(0x008BB4C0)));
            const bool alternateFound =
                reinterpret_cast<bool (__fastcall*)(void*, void*, void*, void*)>(
                    CSF_AT(void**, entry, 0)[0xC / 4])(
                        entry, nullptr, secondKeyBytes, secondaryLookupBytes);
            SceneLoader_NoOp();
            if (alternateFound)
            {
                void* object =
                    CSF_AT(void*, CSF_AT(void*, secondaryLookupBytes, 0), 4);
                reinterpret_cast<void (__fastcall*)(void*, void*)>(
                    CSF_AT(void**, object, 0)[0x7C / 4])(object, nullptr);
            }
        }
    }

    GfxSkin_SetRenderStage(reinterpret_cast<void*>(self->field_0004), nullptr, 7);
    for (std::uint32_t index = 0; index < self->field_00d8; ++index)
    {
        void* object = CSF_AT(void*, CSF_AT(void*, self, 0xD0), 12 * index);
        reinterpret_cast<void (__fastcall*)(void*, void*)>(
            CSF_AT(void**, object, 0)[0x14 / 4])(object, nullptr);
    }

    if (self->field_0008 & 2)
    {
        for (std::uint32_t index = 0; index < self->field_00e8; ++index)
        {
            GfxSkin_UpdateAdditionalObject(
                CSF_AT(void*, reinterpret_cast<void*>(self->field_00e0), 4 * index),
                nullptr);
        }
    }

    GlobalHud_Commit_6C7C30(reinterpret_cast<void*>(self->field_0004), nullptr);
    GlobalHud_Commit_6C7C30(reinterpret_cast<void*>(self->field_0004), nullptr);

    for (std::uint32_t index = 0; index < self->field_00c8; ++index)
    {
        void* entry = CSF_AT(void*, CSF_AT(void*, self, 0xC0), 4 * index);
        GfxSkin_ConstructLookupKey(
            firstKeyBytes, nullptr,
            reinterpret_cast<uint32_t*>(CSF_ADDR_DATA(0x008BB464)));
        const bool found =
            reinterpret_cast<bool (__fastcall*)(void*, void*, void*, void*)>(
                CSF_AT(void**, entry, 0)[0xC / 4])(
                    entry, nullptr, firstKeyBytes, cameraOrLookupBytes);
        SceneLoader_NoOp();
        if (found)
        {
            void* object =
                CSF_AT(void*, CSF_AT(void*, cameraOrLookupBytes, 0), 4);
            if (!reinterpret_cast<bool (__fastcall*)(void*, void*)>(
                    CSF_AT(void**, object, 0)[0xA8 / 4])(object, nullptr))
            {
                void* transform =
                    reinterpret_cast<void* (__fastcall*)(void*, void*)>(
                        CSF_AT(void**, object, 0)[0x18 / 4])(object, nullptr);
                reinterpret_cast<void (__fastcall*)(void*, void*, void*)>(
                    CSF_AT(void**, transform, 0)[0x94 / 4])(
                        transform, nullptr,
                        static_cast<std::uint8_t*>(CSF_AT(void*, self, 0x13C))
                            + 8 * index);
            }
        }
        else
        {
            CSF_AT(void*, alternateLookupBytes, 0) = nullptr;
            entry = CSF_AT(void*, CSF_AT(void*, self, 0xC0), 4 * index);
            GfxSkin_ConstructLookupKey(
                secondKeyBytes, nullptr,
                reinterpret_cast<uint32_t*>(CSF_ADDR_DATA(0x008BB4C0)));
            reinterpret_cast<bool (__fastcall*)(void*, void*, void*, void*)>(
                CSF_AT(void**, entry, 0)[0xC / 4])(
                    entry, nullptr, secondKeyBytes, alternateLookupBytes);
            SceneLoader_NoOp();
        }
    }

    auto* backup = CSF_AT(std::uint8_t*, backupContainer, 4);
    if (backup)
    {
        auto* end = CSF_AT(std::uint8_t*, backupContainer, 8);
        for (; backup != end; backup += 8)
        {
            if (CSF_AT(std::uint8_t, backup, 4))
                RwMatrixDestroy(CSF_AT(std::uint32_t*, backup, 0));
        }
        if (CSF_AT(void*, backupContainer, 4))
            Mem_FreeDebug(CSF_AT(void*, backupContainer, 4));
    }
    CSF_AT(void*, backupContainer, 4) = nullptr;
    CSF_AT(void*, backupContainer, 8) = nullptr;
    CSF_AT(void*, backupContainer, 12) = nullptr;
    return passStarted;
}

HOOK(0x006C7500, S_006c5cd0_RenderSkinPass, "auto");
