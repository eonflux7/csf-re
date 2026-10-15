#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RpPTankAtomicLock, 0x005821D0, (int a1, uint32_t* a2, unsigned int a3, int a4));
CSF_FUNC(int, __cdecl, RpPTankAtomicUnlock, 0x005823E0, (int a1));
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(std::uint32_t*, __cdecl, RwMatrixCreate, 0x00624D40, ());
CSF_FUNC(int, __fastcall, GfxMatrix_ForwardSplitBlocks, 0x006ADA90, (S_006ada10* self, void* , uint32_t* a2));
CSF_FUNC(void, __fastcall, S_006add20_AlignAxesToDirection, 0x006AE040, (S_006add20* self, void* , int fixedAxis, int alignedAxis, float* point, float* origin));
CSF_FUNC(int, __cdecl, GetGlobalValue_6C5760, 0x006C5760, ());

CSF_GLOBAL(std::int32_t, g_PTankPluginOffset, 0x008B09EC);
CSF_GLOBAL(void*, g_pRwEngineInstance, 0x008B68C0);

static int __fastcall GfxPTank_CommitOverlayBuffer(void* self, void* /*edx*/)
{
    alignas(8) std::uint8_t lockBytes[0x8];
    alignas(8) std::uint8_t matrixBytes[0x8];
    alignas(8) std::uint8_t stateBytes[0x4];
    auto& matrix = *reinterpret_cast<S_006ada10*>(matrixBytes);
    auto& savedState = *reinterpret_cast<int*>(stateBytes);
    savedState = 0;

    void* transform = static_cast<std::uint8_t*>(self) + 0x8;
    const std::uint32_t flags = CSF_AT(std::uint32_t, self, 0x30);

    if ((flags & 0x20) != 0)
    {
        auto* createdMatrix = RwMatrixCreate();
        std::memcpy(&matrix.field_0000, &createdMatrix, sizeof(createdMatrix));
        CSF_AT(std::uint8_t, &matrix, 0x4) = 1;

        GfxMatrix_ForwardSplitBlocks(
            &matrix, nullptr,
            reinterpret_cast<std::uint32_t*>(
                static_cast<std::uint8_t*>(
                    CSF_AT(void*, CSF_AT(void*, self, 0x4), g_PTankPluginOffset))
                + 0xF8));

        const int globalValue = GetGlobalValue_6C5760();
        float* origin =
            reinterpret_cast<float* (__fastcall*)(void*, void*)>(
                CSF_AT(void**, transform, 0)[0x10 / 4])(transform, nullptr);

        S_006add20_AlignAxesToDirection(
            reinterpret_cast<S_006add20*>(&matrix), nullptr, 1, 3,
            reinterpret_cast<float*>(globalValue + 0x124), origin);

        std::memcpy(
            static_cast<std::uint8_t*>(
                CSF_AT(void*, CSF_AT(void*, self, 0x4), g_PTankPluginOffset))
                + 0xF8,
            CSF_AT(void*, &matrix, 0), 0x40);

        CSF_AT(std::uint32_t,
               CSF_AT(void*, CSF_AT(void*, self, 0x4), g_PTankPluginOffset),
               0x40) |= 0x8000;

        if (CSF_AT(std::uint8_t, &matrix, 0x4))
            RwMatrixDestroy(CSF_AT(std::uint32_t*, &matrix, 0));
    }
    else if ((flags & 0x10) != 0)
    {
        S_006ada10* suppliedMatrix =
            reinterpret_cast<S_006ada10* (__fastcall*)(void*, void*, S_006ada10*)>(
                CSF_AT(void**, transform, 0)[0x8C / 4])(
                    transform, nullptr, &matrix);

        std::memcpy(
            static_cast<std::uint8_t*>(
                CSF_AT(void*, CSF_AT(void*, self, 0x4), g_PTankPluginOffset))
                + 0xF8,
            CSF_AT(void*, suppliedMatrix, 0), 0x40);

        if (CSF_AT(std::uint8_t, &matrix, 0x4))
            RwMatrixDestroy(CSF_AT(std::uint32_t*, &matrix, 0));

        CSF_AT(std::uint32_t,
               CSF_AT(void*, CSF_AT(void*, self, 0x4), g_PTankPluginOffset),
               0x40) |= 0x8000;
    }

    if (RpPTankAtomicLock(CSF_AT(int, self, 0x4),
                         reinterpret_cast<uint32_t*>(lockBytes),
                         1, 0x40000000))
    {
        auto* position =
            reinterpret_cast<std::uint32_t* (__fastcall*)(void*, void*)>(
                CSF_AT(void**, transform, 0)[0x10 / 4])(transform, nullptr);

        auto* destination = CSF_AT(std::uint32_t*, lockBytes, 0);
        destination[0] = position[0];
        destination[1] = position[1];
        destination[2] = position[2];

        RpPTankAtomicUnlock(CSF_AT(int, self, 0x4));
    }

    reinterpret_cast<void (__cdecl*)(int, int*)>(
        static_cast<void**>(g_pRwEngineInstance)[0x24 / 4])(14, &savedState);

    if ((CSF_AT(std::uint8_t, self, 0x30) & 8) != 0)
    {
        reinterpret_cast<int (__cdecl*)(int, int)>(
            static_cast<void**>(g_pRwEngineInstance)[0x20 / 4])(14, 0);
    }

    using AtomicRenderCallback = void (__cdecl*)(void*);
    void* atomic = CSF_AT(void*, self, 0x4);
    CSF_AT(AtomicRenderCallback, atomic, 0x48)(atomic);

    return reinterpret_cast<int (__cdecl*)(int, int)>(
        static_cast<void**>(g_pRwEngineInstance)[0x20 / 4])(14, savedState);
}

HOOK(0x006EF050, GfxPTank_CommitOverlayBuffer, "auto");
