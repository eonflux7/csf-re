#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RpHAnimHierarchyDestroy, 0x0056CD30, (uint32_t* a1));
CSF_FUNC(int, __cdecl, RpHAnimHierarchyDetach, 0x0056CF50, (int a1));
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(void, __fastcall, C_00860170_Construct, 0x006B2830, (C_00860170* self, void*));
CSF_FUNC(int, __cdecl, GetValue8BBB28, 0x006F7820, ());
CSF_FUNC(char, __fastcall, GfxHAnim_ReleaseResourceReference, 0x006F7A70, (void* self, void* edx, void** a2));
CSF_FUNC(void, __fastcall, C_00863360_Destroy, 0x00707550, (void* self, void* edx));

CSF_GLOBAL(std::uint32_t*, g_HAnimSharedMatrixA, 0x008BBABC);
CSF_GLOBAL(std::uint32_t*, g_HAnimSharedMatrixB, 0x008BBAC0);
CSF_GLOBAL(std::uint32_t*, g_HAnimSharedMatrixC, 0x008BBAC4);
CSF_GLOBAL(int, g_HAnimSharedMatrixUsers, 0x008BBAC8);

static void __fastcall C_00861e08_Destroy(C_00861e08* self, void* /*edx*/)
{
    const int users = g_HAnimSharedMatrixUsers;
    CSF_AT(void**, self, 0) = CSF_VTBL(0x00861E98);

    if (users > 0)
    {
        g_HAnimSharedMatrixUsers = users - 1;
        if (users == 1 && g_HAnimSharedMatrixA)
        {
            RwMatrixDestroy(g_HAnimSharedMatrixC);
            RwMatrixDestroy(g_HAnimSharedMatrixB);
            RwMatrixDestroy(g_HAnimSharedMatrixA);
            g_HAnimSharedMatrixA = nullptr;
            g_HAnimSharedMatrixB = nullptr;
            g_HAnimSharedMatrixC = nullptr;
        }
    }

    RwMatrixDestroy(CSF_AT(std::uint32_t*, self, 0x48));
    CSF_AT(std::uint32_t*, self, 0x48) = nullptr;

    for (int i = 0; i < 8; ++i)
    {
        if (CSF_AT(void*, self, 0x60 + 4 * i))
        {
            Mem_Free(CSF_AT(void*, self, 0x60 + 4 * i));
            CSF_AT(void*, self, 0x60 + 4 * i) = nullptr;
        }
        if (CSF_AT(void*, self, 0x80 + 4 * i))
        {
            Mem_Free(CSF_AT(void*, self, 0x80 + 4 * i));
            CSF_AT(void*, self, 0x80 + 4 * i) = nullptr;
        }
    }

    if (CSF_AT(std::uint32_t, self, 0x1C))
    {
        if (CSF_AT(void*, self, 0x50))
        {
            for (int i = 0;
                 i < CSF_AT(int, CSF_AT(void*, self, 0x14), 4);
                 ++i)
            {
                auto* matrix = CSF_AT(std::uint32_t**, self, 0x50)[i];
                if (matrix)
                {
                    RwMatrixDestroy(matrix);
                    CSF_AT(std::uint32_t**, self, 0x50)[i] = nullptr;
                }
            }
            if (CSF_AT(void*, self, 0x50))
            {
                Mem_Free(CSF_AT(void*, self, 0x50));
                CSF_AT(void*, self, 0x50) = nullptr;
            }
        }

        if (CSF_AT(void*, self, 0x4C))
        {
            for (int i = 0;
                 i < CSF_AT(int, CSF_AT(void*, self, 0x14), 4);
                 ++i)
            {
                void* animation = CSF_AT(void**, self, 0x4C)[i];
                if (animation)
                {
                    void* data = CSF_AT(void*, animation, 0x24);
                    if (data)
                    {
                        Mem_Free(data);
                        CSF_AT(void*, CSF_AT(void**, self, 0x4C)[i], 0x24) = nullptr;
                    }

                    animation = CSF_AT(void**, self, 0x4C)[i];
                    if (animation)
                    {
                        C_00863360_Destroy(animation, nullptr);
                        Mem_Free(animation);
                        CSF_AT(void**, self, 0x4C)[i] = nullptr;
                    }
                }
            }
            if (CSF_AT(void*, self, 0x4C))
            {
                Mem_Free(CSF_AT(void*, self, 0x4C));
                CSF_AT(void*, self, 0x4C) = nullptr;
            }
        }

        int index = static_cast<std::int32_t>(
            CSF_AT(std::uint32_t, self, 0x0C) - 1);
        while (index >= 0)
        {
            auto* entry = CSF_AT(std::uint8_t*, self, 0x04) + 36 * index;
            if (index)
                RpHAnimHierarchyDestroy(CSF_AT(std::uint32_t*, entry, 0));
            else
                RpHAnimHierarchyDetach(CSF_AT(int, entry, 0));

            if (CSF_AT(void*, entry, 4))
            {
                void** reference = reinterpret_cast<void**>(entry + 4);
                void* manager = reinterpret_cast<void*>(GetValue8BBB28());
                GfxHAnim_ReleaseResourceReference(manager, nullptr, reference);
                CSF_AT(void*, entry, 4) = nullptr;
            }
            --index;
            CSF_AT(std::uint32_t, entry, 8) = 0;
            CSF_AT(std::uint32_t, entry, 0x0C) = 0;
        }
    }

    if (CSF_AT(void*, self, 0x04))
    {
        Mem_Free(CSF_AT(void*, self, 0x04));
        CSF_AT(void*, self, 0x04) = nullptr;
    }

    CSF_AT(void**, self, 0) = CSF_VTBL(0x00861E08);
    C_00860170_Construct(reinterpret_cast<C_00860170*>(self), nullptr);
}

HOOK(0x006EA230, C_00861e08_Destroy, "auto");
