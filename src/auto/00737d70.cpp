#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, C_00867260_UpdateQuadVertexData, 0x00737C90, (C_00867260* self, void*));
CSF_GLOBAL(void*, g_pRwEngineInstance, 0x008B68C0);

// Address of the g_pRwEngineInstance slot (0x008B68C0), read by the thunks below.
static const uintptr_t g_rwEngineSlotAddr = CSF_ADDR_DATA(0x008B68C0);

// Engine call with ecx = g_pRwEngineInstance: the original loads `mov ecx, [0x8B68C0]` and then
// `call [ecx+0x20]` with cdecl stack arguments. The thunk tail-jumps, so the callee returns straight
// to the C++ caller, which pops the arguments.
static __declspec(naked) int RwEngineCall_EcxEngine()
{
    __asm {
        mov eax, dword ptr [g_rwEngineSlotAddr]
        mov ecx, dword ptr [eax]
        mov eax, dword ptr [ecx + 0x20]
        jmp eax
    }
}

// Engine call with ecx = the second stack argument (the raster value): the original does
// `mov ecx, [eax]` (eax = self->field_0018) and then `push ecx; push 1; call [engine+0x20]`.
static __declspec(naked) int RwEngineCall_EcxArg2()
{
    __asm {
        mov ecx, dword ptr [esp + 8]
        mov eax, dword ptr [g_rwEngineSlotAddr]
        mov eax, dword ptr [eax]
        mov eax, dword ptr [eax + 0x20]
        jmp eax
    }
}

static int __fastcall C_00867260_Render(C_00867260* self, void* /*edx*/)
{
    if (CSF_AT(int, self, 0x10) == 1)
    {
        void* renderer = CSF_AT(void*, self, 0x0C);
        if (!((bool(__fastcall*)(void*, void*))
                  CSF_AT(void**, renderer, 0)[0x10 / 4])(renderer, nullptr))
        {
            ((void(__fastcall*)(C_00867260*, void*, int))
                 self->vtbl[0x10 / 4])(self, nullptr, 0);
        }
    }

    if (CSF_AT(uint32_t, self, 0x18) == 0)
        return 0;

    C_00867260_UpdateQuadVertexData(self, nullptr);

    using SetRenderState = int(__cdecl*)(int, uint32_t);
    const SetRenderState setEcxEngine = reinterpret_cast<SetRenderState>(&RwEngineCall_EcxEngine);
    const SetRenderState setEcxArg2 = reinterpret_cast<SetRenderState>(&RwEngineCall_EcxArg2);

    reinterpret_cast<SetRenderState>(
        reinterpret_cast<void**>(g_pRwEngineInstance)[8])(12, 1);
    setEcxEngine(10, 5);
    setEcxEngine(14, 0);

    uint32_t raster = CSF_AT(uint32_t, CSF_AT(void*, self, 0x18), 0);
    setEcxArg2(1, raster);

    reinterpret_cast<SetRenderState>(
        reinterpret_cast<void**>(g_pRwEngineInstance)[8])(9, 2);
    setEcxEngine(6, 0);
    setEcxEngine(8, 0);
    setEcxEngine(20, 1);

    if (CSF_AT(int, self, 0x78) == 1)
    {
        setEcxEngine(11, 2);
    }
    else
    {
        setEcxEngine(11, 6);
    }

    uint32_t vertices = CSF_AT(uint32_t, self, 0x14);
    void* renderer = CSF_AT(void*, self, 0x0C);
    return ((int(__fastcall*)(void*, void*, int, uint32_t, int))
                CSF_AT(void**, renderer, 0)[0x24 / 4])(
                    renderer, nullptr, 4, vertices, 4);
}

HOOK(0x00737D70, C_00867260_Render, "auto");
