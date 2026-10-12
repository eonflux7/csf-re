#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_4FBDE0, 0x004FBDE0, ());
CSF_FUNC(int, __fastcall, GetFirstElement, 0x004FC1D0, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, GetReferencedObject, 0x006683F0, (void* self, void* edx));

// Resolves the referenced object chain and forwards the call to its vtable slot 4
// (byte offset 0x10). The caller's stack arguments are not used by this function.
static hook::Tail __fastcall GetContextTransform(void* /*self*/, const uint32_t* /*args*/)
{
    int globalValue = GetGlobalValue_4FBDE0();
    int firstElement = GetFirstElement(reinterpret_cast<void*>(globalValue), nullptr);
    void* cachedObjectProvider = GetCachedObjectProvider(reinterpret_cast<void*>(firstElement), nullptr);
    void* referencedObject = GetReferencedObject(cachedObjectProvider, nullptr);
    return hook::tail_jump(referencedObject, (*reinterpret_cast<void***>(referencedObject))[0x10 / 4]);
}

HOOK_TAIL(0x495890, GetContextTransform, "auto");
