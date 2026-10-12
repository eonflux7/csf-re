#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(uint32_t, __fastcall, CGuiControl_GetField012C, 0x0044F630, (CGuiControl* self, void*));
CSF_FUNC(int, __fastcall, Field012C_LookupEntry, 0x004DE860, (void* self, void* edx, int a2));

// Resolves the two indices stored in this[0] and this[1] through the GUI
// field table and copies each resolved 3-dword record into the two out buffers.
static int __fastcall GuiScene_ResolvePairToOutputs(int* self, void* /*edx*/, uint32_t* outFirst, uint32_t* outSecond)
{
    void* fieldTable = reinterpret_cast<void*>(
        CGuiControl_GetField012C(reinterpret_cast<CGuiControl*>(g_pCurrentScene), nullptr));

    uint32_t* firstEntry = reinterpret_cast<uint32_t*>(
        static_cast<uintptr_t>(Field012C_LookupEntry(fieldTable, nullptr, self[0])));
    outFirst[0] = firstEntry[0];
    outFirst[1] = firstEntry[1];
    outFirst[2] = firstEntry[2];

    uint32_t* secondEntry = reinterpret_cast<uint32_t*>(
        static_cast<uintptr_t>(Field012C_LookupEntry(fieldTable, nullptr, self[1])));
    outSecond[0] = secondEntry[0];
    outSecond[1] = secondEntry[1];
    int result = static_cast<int>(secondEntry[2]);
    outSecond[2] = static_cast<uint32_t>(result);
    return result;
}

HOOK(0x004E0B70, GuiScene_ResolvePairToOutputs, "auto");
