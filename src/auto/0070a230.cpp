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

// Visitor callback type for RpClumpForAllAtomics: RpAtomic* (*)(RpAtomic*, void* data).
// Declared locally because the project headers do not provide it.
using AtomicVisitorFn = void* (__cdecl*)(void*, void*);

CSF_FUNC(uint32_t*, __cdecl, RxPipelineCreate, 0x00633710, ());
CSF_FUNC(int, __cdecl, RxPipelineLock, 0x00638210, (uint32_t* a1));
CSF_FUNC(void, __cdecl, RxPipelineDestroy, 0x00633780, (uint32_t* a1));
CSF_FUNC(void**, __cdecl, RwOsGetFileInterface, 0x005935D0, ());
CSF_FUNC(uint32_t*, __cdecl, RxLockedPipeAddFragment, 0x006383C0, (uint32_t* a1, uint32_t* a2, int a3, int a4));
CSF_FUNC(uint32_t*, __cdecl, RxLockedPipeUnlock, 0x00637E30, (uint32_t* a1, int a2, int a3, int a4, int a5));
CSF_FUNC(void**, __cdecl, RxPipelineFindNodeByName, 0x00638320, (int a1, int a2, void** a3, uint32_t* a4));
CSF_FUNC(int, __cdecl, RxD3D9AllInOneSetRenderCallBack, 0x00592690, (int a1, int a2, int a3, int a4, int a5, int a6));
CSF_FUNC(int, __cdecl, RpClumpForAllAtomics, 0x0058EF00, (int clump, AtomicVisitorFn visitor, int data));

static char __fastcall Water_InitRenderPipeline(void* self, void* /*edx*/)
{
    // Create a render pipeline and, if it locks, add the file-interface fragment.
    uint32_t* pipeline = RxPipelineCreate();
    if (pipeline)
    {
        int lockResult = RxPipelineLock(pipeline);
        if (lockResult)
        {
            int fileInterface = (int)(uintptr_t)RwOsGetFileInterface();
            // variadic: (pipeline, firstIndex = NULL, node definition, NULL terminator): four pushes
            uint32_t* locked = RxLockedPipeAddFragment(
                reinterpret_cast<uint32_t*>(lockResult), nullptr, fileInterface, 0);
            // original leaves the AddFragment stack args in place: unlock sees (locked, lockResult, 0, fileInterface, 0)
            RxLockedPipeUnlock(locked, lockResult, 0, fileInterface, 0);
        }
        else
        {
            RxPipelineDestroy(pipeline);
            pipeline = nullptr;
        }
    }

    CSF_AT(uint32_t*, self, 0xF0) = pipeline;

    // Find the named node and install the render callback on it.
    void** fileObject = RwOsGetFileInterface();
    int fileObjectValue = (int)(uintptr_t)*fileObject;
    int pipelineValue = (int)(uintptr_t)CSF_AT(uint32_t*, self, 0xF0);
    void** nodeByName = RxPipelineFindNodeByName(pipelineValue, fileObjectValue, nullptr, nullptr);

    // The original leaves the four FindNodeByName stack arguments in place and the
    // render-callback call consumes them as its trailing arguments.
    RxD3D9AllInOneSetRenderCallBack(
        (int)(uintptr_t)nodeByName,
        (int)(CSF_ADDR_FN(0x0070A070)),
        pipelineValue,
        fileObjectValue,
        0,
        0);

    // Visit every atomic of the clump.
    int clump = CSF_AT(int, self, 0x8);
    RpClumpForAllAtomics(clump, (AtomicVisitorFn)(CSF_ADDR_FN(0x00709900)), (int)(uintptr_t)self);

    return 1;
}

HOOK(0x0070A230, Water_InitRenderPipeline, "auto");

