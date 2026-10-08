#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, MenuExtras_ResolveGlobalObject, 0x006B1390,
         (void* self, void* edx));
CSF_FUNC(int, __fastcall, MenuExtras_UpdateThumbnailObject, 0x004F2930,
         (int* self, void* edx));

static __declspec(naked) int __fastcall CMenuExtras_TailDispatch(
    void* object, void* /*edx*/)
{
    __asm {
        mov eax, ecx
        mov edx, dword ptr [eax]
        mov ecx, eax
        jmp dword ptr [edx + 0x78]
    }
}

static int __fastcall CMenuExtras_ReleaseThumbnailAndDispatch(
    CMenuExtras* self, void* /*edx*/)
{
    int globalHandle = GetGlobalInt();
    void* object = reinterpret_cast<void*>(
        MenuExtras_ResolveGlobalObject(
            reinterpret_cast<void*>(static_cast<std::uintptr_t>(globalHandle)),
            nullptr));

    int result =
        reinterpret_cast<int (__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, object, 0)[0x74 / 4])(object, nullptr, 0);

    if (static_cast<std::uint8_t>(result) == 0)
        return result;

    int* thumbnailObject = reinterpret_cast<int*>(self->field_0048);
    if (thumbnailObject)
        MenuExtras_UpdateThumbnailObject(thumbnailObject, nullptr);

    globalHandle = GetGlobalInt();
    object = reinterpret_cast<void*>(
        MenuExtras_ResolveGlobalObject(
            reinterpret_cast<void*>(static_cast<std::uintptr_t>(globalHandle)),
            nullptr));

    return CMenuExtras_TailDispatch(object, nullptr);
}

HOOK(0x00545EC0, CMenuExtras_ReleaseThumbnailAndDispatch, "auto");
