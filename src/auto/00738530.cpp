#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __fastcall, C_00867318_UpdateRectangleVertices, 0x00738300, (void* self, void* edx));
CSF_GLOBAL(void*, g_pRwEngineInstance, 0x008B68C0);

// RwRenderStateSet through the engine instance (+0x20, cdecl).
static void SetRenderState(int state, int value)
{
    reinterpret_cast<int(__cdecl*)(int, int)>(CSF_AT(void*, g_pRwEngineInstance, 0x20))(state, value);
}

// Draws the rectangle when +0x1C or +0x20 is set: refreshes the state (+4 renderer, +8 == 1) if the renderer is
// not ready, rebuilds the vertices, sets the 2D render states and draws through the renderer's slot 0x24
// (thiscall, receiver in ECX): a 4-vertex fan when +0x34 is set, else a 5-vertex strip (+0xC vertices).
static int __fastcall C_00867318_Render(void* self, void* /*edx*/)
{
    if (!CSF_AT(std::uint32_t, self, 0x1C) && !CSF_AT(std::uint32_t, self, 0x20))
        return 0;

    void* renderer = CSF_AT(void*, self, 0x04);
    if (renderer && CSF_AT(std::uint32_t, self, 0x08) == 1 &&
        !reinterpret_cast<std::uint8_t(__fastcall*)(void*, void*)>(CSF_AT(void**, renderer, 0)[0x10 / 4])(renderer, nullptr))
        reinterpret_cast<void(__fastcall*)(void*, void*, int)>(CSF_AT(void**, self, 0)[0x10 / 4])(self, nullptr, 0);

    C_00867318_UpdateRectangleVertices(self, nullptr);
    SetRenderState(10, 5);
    SetRenderState(11, 6);
    SetRenderState(12, 1);
    SetRenderState(14, 0);
    SetRenderState(1, 0);
    SetRenderState(9, 2);
    SetRenderState(6, 0);
    SetRenderState(8, 0);
    SetRenderState(20, 1);

    renderer = CSF_AT(void*, self, 0x04);
    const std::uint32_t vertices = CSF_AT(std::uint32_t, self, 0x0C);
    auto draw = reinterpret_cast<int(__fastcall*)(void*, void*, int, std::uint32_t, int)>(CSF_AT(void**, renderer, 0)[0x24 / 4]);
    if (CSF_AT(std::uint8_t, self, 0x34))
        return draw(renderer, nullptr, 4, vertices, 4);
    return draw(renderer, nullptr, 2, vertices, 5);
}

HOOK(0x00738530, C_00867318_Render, "auto");
