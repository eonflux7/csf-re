#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Callees, bound by address.
CSF_FUNC(int, __fastcall, LookupSceneSlot, 0x00468E20, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, GetGlobalValue_4FBDE0, 0x004FBDE0, ());
CSF_FUNC(int, __fastcall, GetGlobalTableEntry, 0x004FC140, (void* self, void* edx, unsigned int a2));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));
CSF_FUNC(int, __fastcall, InvokeMemberVirtualSlot8, 0x004F5170, (void* self, void* edx, int a2));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

// Looks up a scene slot from a float parameter chain, resolves the global table
// entry for it, and invokes virtual slot 8 on the cached provider.
// stdcall, 3 args, ret 12.
static C_0085e7b8* __stdcall ResolveSceneSlotObject(int /*unusedA1*/, int /*unusedA2*/, uint8_t* holder)
{
    // Slot index: holder -> node1 -> node2 -> float at +4 (truncated to integer).
    uint8_t* firstNode = *reinterpret_cast<uint8_t**>(holder);
    uint8_t* secondNode = *reinterpret_cast<uint8_t**>(firstNode + 4);
    float slotValue = *reinterpret_cast<float*>(secondNode + 4);
    int32_t slotIndex = static_cast<int32_t>(static_cast<int64_t>(slotValue));

    // Scene sub-object at g_pCurrentScene->+0xBC, then embedded member at +0x94.
    uint8_t* sceneChild = CSF_AT(uint8_t*, g_pCurrentScene, 0xBC);
    int slotResult = LookupSceneSlot(sceneChild + 0x94, nullptr, slotIndex);
    if (slotResult == -1)
        return reinterpret_cast<C_0085e7b8*>(slotResult);

    // The original leaves slotResult pushed across this cdecl call; the
    // table lookup below consumes it as its stack argument.
    int tableResult = GetGlobalValue_4FBDE0();
    int entry = GetGlobalTableEntry(reinterpret_cast<void*>(static_cast<uintptr_t>(tableResult)),
                                    nullptr, static_cast<unsigned int>(slotResult));
    if (entry == 0)
        return nullptr;

    // Provider from the resolved entry, then invoke slot 8 with a value taken
    // from a second chain rooted at holder+4 (re-read from holder after the calls).
    void* provider = GetCachedObjectProvider(reinterpret_cast<void*>(static_cast<uintptr_t>(entry)), nullptr);
    uint8_t* holderNext = *reinterpret_cast<uint8_t**>(holder + 4);
    uint8_t* holderNext2 = *reinterpret_cast<uint8_t**>(holderNext + 4);
    uint32_t providerArg = *reinterpret_cast<uint32_t*>(holderNext2 + 4);
    int invokeResult = InvokeMemberVirtualSlot8(provider, nullptr, static_cast<int>(providerArg));
    return reinterpret_cast<C_0085e7b8*>(invokeResult);
}

HOOK(0x45DF80, ResolveSceneSlotObject, "auto");
