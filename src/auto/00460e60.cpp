#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __fastcall, ApplyScaledValueToResolvedObject, 0x0048E3F0, (void* self, void* edx, int a2, int a3));

// Stdcall, 3 stack args (ret 12). argsBlock is the third argument: a pointer to
// an array of three record pointers. Each record has its payload pointer at +4.
static int __stdcall DispatchResolvedHandleWithScaledFloat(int /*unused1*/, int /*unused2*/, uint32_t* argsBlock)
{
    // First record: v3 = *(argsBlock[0] + 4)
    const uint32_t firstRecord = argsBlock[0];
    const uint32_t handleOwner = *reinterpret_cast<uint32_t*>(firstRecord + 4);

    // v4 = *(v3 + 12), compared as signed.
    const int32_t ownerFlags = *reinterpret_cast<int32_t*>(handleOwner + 12);
    const uint32_t handleValue = handleOwner + 4;

    int result = static_cast<int>(handleValue);
    if (ownerFlags >= 0)
    {
        result = CHandleTable_Resolve(g_pCurrentScene, nullptr, static_cast<int>(handleValue));
        if (result != 0)
        {
            void* resolvedObject = reinterpret_cast<void*>(static_cast<uintptr_t>(result));

            // Third argument: float source at *(*(argsBlock[2] + 4) + 4)
            const uint32_t thirdRecord = argsBlock[2];
            const uint32_t thirdPayload = *reinterpret_cast<uint32_t*>(thirdRecord + 4);
            const float scaleSource = *reinterpret_cast<float*>(thirdPayload + 4);

            // (unsigned __int64)float; the callee receives the low dword.
            const uint64_t scaledValue = static_cast<uint64_t>(static_cast<int64_t>(scaleSource));
            const int scaledLow = static_cast<int>(static_cast<uint32_t>(scaledValue));

            // Second record: *(*(argsBlock[1] + 4) + 4)
            const uint32_t secondRecord = argsBlock[1];
            const uint32_t secondPayload = *reinterpret_cast<uint32_t*>(secondRecord + 4);
            const int secondValue = static_cast<int>(*reinterpret_cast<uint32_t*>(secondPayload + 4));

            return ApplyScaledValueToResolvedObject(resolvedObject, nullptr, secondValue, scaledLow);
        }
    }
    return result;
}

HOOK(0x00460E60, DispatchResolvedHandleWithScaledFloat, "auto");
