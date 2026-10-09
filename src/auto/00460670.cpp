#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_4FBDE0, 0x004FBDE0, ());
CSF_FUNC(int, __fastcall, GetFirstElement, 0x004FC1D0, (void* self, void*));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GetProviderStatusCode, 0x00471540, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, GetReferencedObject, 0x006683F0, (void* self, void* edx));

// Converts a 32-bit integer value returned by the game into an object pointer.
static void* IntValueAsPointer(int value)
{
    return reinterpret_cast<void*>(static_cast<uintptr_t>(static_cast<uint32_t>(value)));
}

static int __stdcall QueryProviderStateForRequest(int a1, int a2, int a3)
{
    int globalValue = GetGlobalValue_4FBDE0();
    C_0085e7b8* firstElement = reinterpret_cast<C_0085e7b8*>(
        IntValueAsPointer(GetFirstElement(IntValueAsPointer(globalValue), nullptr)));
    C_0085e760* cachedProvider = reinterpret_cast<C_0085e760*>(
        GetCachedObjectProvider(firstElement, nullptr));

    int result = GetProviderStatusCode(cachedProvider, nullptr);
    if (result == 3)
    {
        void* referencedObject = GetReferencedObject(cachedProvider, nullptr);

        // Vtable slot 4 (offset 0x10) takes no arguments and yields the inner object.
        void** referencedVtbl = *reinterpret_cast<void***>(referencedObject);
        void* innerObject = reinterpret_cast<void*(__fastcall*)(void*, void*)>(referencedVtbl[4])(
            referencedObject, nullptr);

        // Argument: *(*(uint32_t*)a3 + 4) + 4, read after the first virtual call as in the original.
        uint32_t* requestPointer = reinterpret_cast<uint32_t*>(static_cast<uintptr_t>(static_cast<uint32_t>(a3)));
        uint32_t requestInner = *requestPointer;
        uint32_t requestValue = *reinterpret_cast<uint32_t*>(static_cast<uintptr_t>(requestInner + 4u));
        uint32_t valueArg = requestValue + 4u;

        // Vtable slot 84 (offset 0x54) takes one argument and returns the result.
        void** innerVtbl = *reinterpret_cast<void***>(innerObject);
        return reinterpret_cast<int(__fastcall*)(void*, void*, uint32_t)>(innerVtbl[21])(
            innerObject, nullptr, valueArg);
    }
    return result;
}

HOOK(0x00460670, QueryProviderStateForRequest, "auto");
