#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __fastcall, C_008672e4_UpdateVertices, 0x00737F40, (void* self, void* edx));
CSF_GLOBAL(void*, g_pRwEngineInstance, 0x008B68C0);

// RwRenderStateSet through the engine instance (+0x20, cdecl).
static void SetRenderState(int state, int value)
{
    reinterpret_cast<int(__cdecl*)(int, int)>(CSF_AT(void*, g_pRwEngineInstance, 0x20))(state, value);
}

// Same shape as 0x738530 without the work check: refresh the state if the renderer (+4, +8 == 1) is not ready,
// rebuild the vertices, set the 2D render states and draw 2 vertices (+0xC) as primitive 1 through the renderer's
// slot 0x24 (thiscall, receiver in ECX).
static int __fastcall C_008672e4_Render(void* self, void* /*edx*/)
{
    void* renderer = CSF_AT(void*, self, 0x04);
    if (renderer && CSF_AT(std::uint32_t, self, 0x08) == 1 &&
        !reinterpret_cast<std::uint8_t(__fastcall*)(void*, void*)>(CSF_AT(void**, renderer, 0)[0x10 / 4])(renderer, nullptr))
        reinterpret_cast<void(__fastcall*)(void*, void*, int)>(CSF_AT(void**, self, 0)[0x10 / 4])(self, nullptr, 0);

    C_008672e4_UpdateVertices(self, nullptr);
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
    return reinterpret_cast<int(__fastcall*)(void*, void*, int, std::uint32_t, int)>(CSF_AT(void**, renderer, 0)[0x24 / 4])(
        renderer, nullptr, 1, CSF_AT(std::uint32_t, self, 0x0C), 2);
}

HOOK(0x00738070, C_008672e4_Render, "auto");
