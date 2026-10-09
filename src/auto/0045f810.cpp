#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

// Resolves the handle stored in a link record. If the handle maps to an object with a
// non-null sub-object at +0x34, calls that sub-object's virtual slot 0xA8 with a
// 4-int descriptor {25, a4[1] value, a4[2] value, -1}, then copies two dwords from the
// link record into self (+0x60, +0x64) and clears the byte at self+0x82.
static int __fastcall SceneLink_ResolveAndSync(void* self, void* /*edx*/, int /*a2*/, int /*a3*/, uint32_t* a4)
{
    uint32_t* objectRef = reinterpret_cast<uint32_t*>(a4[0]);
    uintptr_t linkBase = objectRef[1];
    int result = *reinterpret_cast<int*>(linkBase + 12);
    uintptr_t handleRecord = linkBase + 4;

    if (result >= 0)
    {
        result = CHandleTable_Resolve(g_pCurrentScene, nullptr, static_cast<int>(handleRecord));
        if (result != 0)
        {
            uintptr_t resolvedObject = static_cast<uintptr_t>(static_cast<uint32_t>(result));
            uint32_t subObject = *reinterpret_cast<uint32_t*>(resolvedObject + 52);
            if (subObject != 0)
            {
                int descriptor[4] = { 25, -1, 0, -1 };

                uint32_t* firstLink = reinterpret_cast<uint32_t*>(a4[1]);
                uint32_t* firstTarget = reinterpret_cast<uint32_t*>(firstLink[1]);
                descriptor[1] = static_cast<int>(firstTarget[1]);

                uint32_t* secondLink = reinterpret_cast<uint32_t*>(a4[2]);
                uint32_t* secondTarget = reinterpret_cast<uint32_t*>(secondLink[1]);
                descriptor[2] = static_cast<int>(secondTarget[1]);

                void* subObjectPtr = reinterpret_cast<void*>(subObject);
                result = ((int(__fastcall*)(void*, void*, int*))CSF_AT(void**, subObjectPtr, 0)[42])(
                    subObjectPtr, nullptr, descriptor);

                CSF_AT(uint32_t, self, 0x60) = reinterpret_cast<uint32_t*>(handleRecord)[1];
                CSF_AT(uint32_t, self, 0x64) = reinterpret_cast<uint32_t*>(handleRecord)[2];
                CSF_AT(uint8_t, self, 0x82) = 0;
            }
        }
    }
    return result;
}

HOOK(0x0045F810, SceneLink_ResolveAndSync, "auto");
