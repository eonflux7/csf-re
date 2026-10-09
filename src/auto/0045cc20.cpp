#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __fastcall, GetInnerRecordPointer, 0x00476060, (void* self, void* edx));
CSF_FUNC(int, __cdecl, IsCategoryEnabled, 0x00755810, (unsigned int a1));
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

// Resolves the handle stored in the request header, checks the entry's category,
// and copies the first dword of the entry's inner record into the owner's field at +4.
static int __stdcall UpdateEntryStateFromHandle(int a1, int a2, int a3)
{
    (void)a2;

    void* selfPtr = reinterpret_cast<void*>(a1);
    uint8_t* owner = CSF_AT(uint8_t*, selfPtr, 4);
    CSF_AT(int32_t, owner, 4) = -1;

    uint8_t* request = CSF_AT(uint8_t*, reinterpret_cast<void*>(a3), 0);
    uint8_t* entryHeader = CSF_AT(uint8_t*, request, 4);
    int handleIndex = CSF_AT(int32_t, entryHeader, 12);

    int result = static_cast<int>(reinterpret_cast<uintptr_t>(entryHeader + 4));

    if (handleIndex >= 0)
    {
        int resolvedEntry = CHandleTable_Resolve(g_pCurrentScene, nullptr, result);
        result = resolvedEntry;
        if (resolvedEntry)
        {
            uint8_t* entry = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(resolvedEntry));
            result = IsCategoryEnabled(CSF_AT(uint32_t, entry, 80));
            if (static_cast<uint8_t>(result))
            {
                void* component = CSF_AT(void*, entry, 68);
                if (component)
                {
                    // Virtual slot 220 / 4 = 55, __thiscall with no arguments.
                    void** vtbl = CSF_AT(void**, component, 0);
                    result = ((int(__fastcall*)(void*, void*))vtbl[55])(component, nullptr);
                    if (result)
                    {
                        uint8_t* ownerAgain = CSF_AT(uint8_t*, selfPtr, 4);
                        result = GetInnerRecordPointer(reinterpret_cast<void*>(static_cast<uintptr_t>(result)), nullptr);
                        uint8_t* record = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(result));
                        CSF_AT(int32_t, ownerAgain, 4) = CSF_AT(int32_t, record, 0);
                    }
                }
            }
        }
    }

    return result;
}

HOOK(0x0045CC20, UpdateEntryStateFromHandle, "auto");
