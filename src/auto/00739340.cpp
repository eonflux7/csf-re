#include <cstdint>
#include <cstddef>
#include "hook.h"
#include "core/memory.h"
#include "globals.h"

CSF_FUNC(int, __cdecl, AllocateAlignedBuffer, 0x00690EB0,
         (std::size_t size, int alignment, int option1, int option2));

CSF_GLOBAL(void*, g_QuadVertexBuffer, 0x008BBF8C);
CSF_GLOBAL(void*, g_QuadIndexBuffer, 0x008BBF90);

static int __cdecl InitializeQuadGeometryBuffers()
{
    int result = static_cast<int>(
        reinterpret_cast<std::uintptr_t>(g_QuadVertexBuffer));

    if (!g_QuadVertexBuffer)
    {
        g_QuadVertexBuffer = reinterpret_cast<void*>(
            AllocateAlignedBuffer(0x5780u, 64, 0, 0));

        for (result = 0; static_cast<std::uint32_t>(result) < 0x5780u;
             result += 28)
        {
            CSF_AT(std::uint32_t, g_QuadVertexBuffer, result + 8) = 0;
            CSF_AT(std::uint32_t, g_QuadVertexBuffer, result + 12) =
                0x3F800000u;
        }
    }

    if (!g_QuadIndexBuffer)
    {
        g_QuadIndexBuffer = reinterpret_cast<void*>(
            AllocateAlignedBuffer(0x960u, 64, 0, 0));

        int firstVertex = 0;
        for (result = 6; result < 2406; result += 12)
        {
            CSF_AT(std::uint16_t, g_QuadIndexBuffer, result - 6) =
                static_cast<std::uint16_t>(firstVertex);
            CSF_AT(std::uint16_t, g_QuadIndexBuffer, result - 4) =
                static_cast<std::uint16_t>(firstVertex + 1);
            CSF_AT(std::uint16_t, g_QuadIndexBuffer, result - 2) =
                static_cast<std::uint16_t>(firstVertex + 2);
            CSF_AT(std::uint16_t, g_QuadIndexBuffer, result) =
                static_cast<std::uint16_t>(firstVertex + 2);
            CSF_AT(std::uint16_t, g_QuadIndexBuffer, result + 2) =
                static_cast<std::uint16_t>(firstVertex + 1);
            CSF_AT(std::uint16_t, g_QuadIndexBuffer, result + 4) =
                static_cast<std::uint16_t>(firstVertex + 3);
            firstVertex += 4;
        }
    }

    return result;
}

HOOK(0x00739340, InitializeQuadGeometryBuffers, "auto");
