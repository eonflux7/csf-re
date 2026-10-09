#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Callees, bound by address (signatures per packet).
CSF_FUNC(int, __fastcall, sub_468E20, 0x00468E20, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, sub_471540, 0x00471540, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalValue_4FBDE0, 0x004FBDE0, ());
CSF_FUNC(int, __fastcall, sub_4FC140, 0x004FC140, (void* self, void* edx, unsigned int a2));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static C_0085e760* __stdcall Scene_ResolveAndInvokeSlotMethod(int /*unusedArg1*/, int /*unusedArg2*/, uint32_t* refArgs)
{
    // Float value read through refArgs[0] -> +4 -> +4.
    uint8_t* refRecord = *reinterpret_cast<uint8_t**>(refArgs);
    uint8_t* refInner = *reinterpret_cast<uint8_t**>(refRecord + 4);
    float slotValue = *reinterpret_cast<float*>(refInner + 4);

    // Scene sub-object lives at g_pCurrentScene->[0xBC] + 0x94.
    uint8_t* sceneObject = CSF_AT(uint8_t*, g_pCurrentScene, 0xBC);
    void* sceneSubObject = reinterpret_cast<void*>(sceneObject + 0x94);

    int slotIndex = sub_468E20(sceneSubObject, nullptr,
                               static_cast<int>(static_cast<uint64_t>(slotValue)));

    C_0085e760* result = reinterpret_cast<C_0085e760*>(static_cast<intptr_t>(slotIndex));
    if (result != reinterpret_cast<C_0085e760*>(static_cast<intptr_t>(-1)))
    {
        uint32_t resolvedSlot = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(result));
        int globalObj = GetGlobalValue_4FBDE0();
        result = reinterpret_cast<C_0085e760*>(static_cast<intptr_t>(
            sub_4FC140(reinterpret_cast<void*>(static_cast<intptr_t>(globalObj)), nullptr, resolvedSlot)));
        if (result)
        {
            int methodOwner = sub_471540(reinterpret_cast<void*>(result), nullptr);
            void* owner = reinterpret_cast<void*>(static_cast<intptr_t>(methodOwner));
            uint8_t flagByte = *(reinterpret_cast<uint8_t*>(
                *reinterpret_cast<uint8_t**>(*reinterpret_cast<uint8_t**>(refArgs + 1) + 4) + 1));
            using VirtualMethodFn = C_0085e760* (__fastcall*)(void*, void*, int);
            VirtualMethodFn method = reinterpret_cast<VirtualMethodFn>(CSF_AT(void**, owner, 0)[17]);
            result = method(owner, nullptr, static_cast<int>(flagByte));
        }
    }
    return result;
}

HOOK(0x0045DFE0, Scene_ResolveAndInvokeSlotMethod, "auto");
