#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int*, __fastcall, NetSession_ConstructLookupContext, 0x0073DA70,
         (void* self, void* edx));
CSF_FUNC(void, __fastcall, NetSession_PrepareLookupContext, 0x0064B880,
         (void* self, void* edx));
CSF_FUNC(void, __fastcall, NetSession_SetLookupMode, 0x0064B900,
         (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, NetSession_InitializeObjectReference, 0x00651A60,
         (void* self, void* edx, int a2));
CSF_FUNC(void, __fastcall, NetSession_DestroyObjectReference, 0x00651B40,
         (void* self, void* edx));
CSF_FUNC(int, __fastcall, NetSession_AssignObjectReference, 0x00651C70,
         (void* self, void* edx, uint32_t* a2));
CSF_FUNC(int, __fastcall, NetSession_CopyObjectReference, 0x00651D30,
         (void* self, void* edx, int* a2));
CSF_FUNC(void, __fastcall, NetSession_ClearLookupContext, 0x0065E130,
         (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, NetSession_IsSpawnReferenceValid, 0x0073CA40,
         (void* self, void* edx));

static auto __cdecl NetSession_ResolveSpawnVtableSlotPointer()
    -> uint32_t* (__stdcall **)(uint32_t*)
{
    // Preserve the frame adjacency of the context and its reference storage.
    alignas(8) uint8_t frameBytes[0x40];
    void* context = frameBytes;
    void* selectedReference = frameBytes + 0x04;
    void* temporaryReference = frameBytes + 0x24;
    C_00873b14& resolvedReference =
        *reinterpret_cast<C_00873b14*>(frameBytes + 0x30);
    uint32_t* emptyReference =
        reinterpret_cast<uint32_t*>(frameBytes + 0x3C);

    uint32_t* (__stdcall **slotPointer)(uint32_t*) = nullptr;

    NetSession_ConstructLookupContext(context, nullptr);
    if (CSF_AT(uint32_t, context, 0) != 0)
    {
        NetSession_PrepareLookupContext(context, nullptr);
        NetSession_SetLookupMode(context, nullptr, 1);
    }
    else
    {
        *emptyReference = 0;
        NetSession_AssignObjectReference(selectedReference, nullptr,
                                        emptyReference);
    }

    if (CSF_AT(uint32_t, selectedReference, 0x04) != 0)
    {
        NetSession_InitializeObjectReference(
            temporaryReference, nullptr,
            CSF_AT(int, selectedReference, 0));
        NetSession_CopyObjectReference(
            &resolvedReference, nullptr,
            reinterpret_cast<int*>(temporaryReference));
        NetSession_DestroyObjectReference(temporaryReference, nullptr);

        if (NetSession_IsSpawnReferenceValid(&resolvedReference, nullptr) &&
            resolvedReference.vtbl != nullptr)
        {
            slotPointer = reinterpret_cast<uint32_t* (__stdcall **)(uint32_t*)>(
                reinterpret_cast<uint8_t*>(resolvedReference.vtbl) + 0xD8);
        }
        else
        {
            slotPointer = nullptr;
        }

        NetSession_DestroyObjectReference(&resolvedReference, nullptr);
    }

    NetSession_ClearLookupContext(context, nullptr, 0);
    *emptyReference = 0;
    NetSession_AssignObjectReference(selectedReference, nullptr, emptyReference);
    NetSession_DestroyObjectReference(selectedReference, nullptr);

    return slotPointer;
}

HOOK(0x0073E6B0, NetSession_ResolveSpawnVtableSlotPointer, "auto");
