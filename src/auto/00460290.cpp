#include <cstdint>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Original callees, bound by address.
CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __fastcall, IsField1BCNonNegative, 0x00492B10, (void* self, void*));
CSF_FUNC(char*, __fastcall, S_00492b20_GetPtr0x1B4, 0x00492B20, (void* self, void*));
CSF_FUNC(int, __cdecl, IsStateCodeAllowed, 0x00755810, (unsigned int a1));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

// Vtable of the S_00492b20 type that the accessor at 0x492B20 is written against.
[[maybe_unused]] static void* const kHandleEntryVtable = CSF_VTBL(0x00492B20);

// Resolves the handle record referenced through *a3. If the record's child entry is valid, forwards
// request code 33 to the object at +0x34 (vtbl +0xA8). Otherwise calls the entry's default handler (vtbl +0xA4).
static int __stdcall DispatchHandleRequestFromRef([[maybe_unused]] int a1, [[maybe_unused]] int a2, int a3)
{
    // v3 = *(*(int*)a3 + 4): a3 points at a slot holding the record pointer.
    const uint32_t recordAddr = CSF_AT(uint32_t, a3, 0x0);
    const uint32_t entryAddr = CSF_AT(uint32_t, recordAddr, 0x4);
    const int entryFlagValue = CSF_AT(int, entryAddr, 0xC);
    const int handleKey = static_cast<int>(entryAddr + 4);

    if (entryFlagValue < 0)
        return handleKey;

    const int resolvedHandle = CHandleTable_Resolve(g_pCurrentScene, nullptr, handleKey);
    if (resolvedHandle == 0)
        return resolvedHandle;

    void* const resolvedObject = reinterpret_cast<void*>(resolvedHandle);

    // entryChild = resolvedObject->vtbl[+0x28](resolvedObject)
    const int entryChild = reinterpret_cast<int (__fastcall*)(void*, void*)>(
        CSF_AT(void**, resolvedObject, 0)[10])(resolvedObject, nullptr);
    if (entryChild == 0)
        return entryChild;

    void* const entryChildObject = reinterpret_cast<void*>(entryChild);

    // The original tests only AL after both predicate calls (test al, al), so compare the low byte only.
    const bool stateCodeAllowed =
        (static_cast<uint8_t>(IsStateCodeAllowed(CSF_AT(uint32_t, resolvedObject, 0x50))) != 0);
    if (stateCodeAllowed &&
        (static_cast<uint8_t>(IsField1BCNonNegative(resolvedObject, nullptr)) != 0))
    {
        char* const accessor1B4 = S_00492b20_GetPtr0x1B4(resolvedObject, nullptr);
        const int accessorHandle = CHandleTable_Resolve(
            g_pCurrentScene, nullptr, static_cast<int>(reinterpret_cast<uintptr_t>(accessor1B4)));
        if (accessorHandle != 0)
        {
            void* const requestTarget = reinterpret_cast<void*>(CSF_AT(uint32_t, resolvedObject, 0x34));
            // The original overwrites its stack argument slot [ebp+0x10] with 0x21 and passes its address.
            a3 = 33;
            // requestTarget->vtbl[+0xA8](requestTarget, &a3)
            return reinterpret_cast<int (__fastcall*)(void*, void*, int*)>(
                CSF_AT(void**, requestTarget, 0)[42])(requestTarget, nullptr, &a3);
        }
        return accessorHandle;
    }

    // entryChildObject->vtbl[+0xA4](entryChildObject)
    return reinterpret_cast<int (__fastcall*)(void*, void*)>(
        CSF_AT(void**, entryChildObject, 0)[41])(entryChildObject, nullptr);
}

HOOK(0x00460290, DispatchHandleRequestFromRef, "auto");
