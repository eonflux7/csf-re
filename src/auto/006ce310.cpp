#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __stdcall, AssignCollisionShape_6CE2E0, 0x006CE2E0, (int a1, int a2));
CSF_FUNC(unsigned int, __fastcall, DestroySceneObjectOptionsStorage, 0x004018E0, (void* self, void* edx));
CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666,
         (void* exceptionObject, void* throwInfo));
CSF_FUNC(int, __cdecl, ___CxxFrameHandler, 0x007AF226,
         (void* exceptionRecord, void* frame, void* context, void* dispatcher));

// Exception-funclet data referenced by the frame thunk at 0x827C00 (mov eax, 0x8905F0).
CSF_GLOBAL(std::uint8_t, g_CollisionShapeRangeExceptionInfo, 0x008905F0);

static std::uint32_t* __cdecl CopyCollisionShapeRange_6CE310(
    int first,
    int last,
    int destination,
    int context,
    int unused)
{
    (void)&g_CollisionShapeRangeExceptionInfo;
    (void)context;
    (void)unused;

    const int destinationBegin = destination;
    int source = first;

    try
    {
        while (source != last)
        {
            AssignCollisionShape_6CE2E0(destination, source);

            destination += 0x64;
            source += 0x64;
        }
    }
    catch (...)
    {
        int completed = destinationBegin;
        while (completed != destination)
        {
            DestroySceneObjectOptionsStorage(
                reinterpret_cast<void*>(static_cast<std::uintptr_t>(completed)),
                nullptr);
            completed += 0x64;
        }

        RethrowCppException(nullptr, nullptr);
        __assume(0);
    }

    return reinterpret_cast<std::uint32_t*>(static_cast<std::uintptr_t>(destination));
}

HOOK(0x006CE310, CopyCollisionShapeRange_6CE310, "auto");
