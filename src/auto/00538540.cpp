#include <cstdint>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Original calls, bound by address.
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(void*, __fastcall, MenuControls_ResolveObject, 0x006B1390, (int globalIndex, void* edx));
CSF_FUNC(void*, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

// Sets the loader object from the global index, then configures it with
// four values taken from the global value block (+0xB34..+0xB40).
static int __fastcall CMenuLoading_ConfigureLoader(CMenuLoading* self, void* /*edx*/)
{
    self->field_004c = 0;

    int globalIndex = GetGlobalInt();
    void* indexedObject = MenuControls_ResolveObject(globalIndex, nullptr);

    // Virtual slot 0x44 / 4 = 17 on indexedObject; result becomes the loader.
    int loaderHandle = ((int(__fastcall*)(void*, void*))CSF_AT(void**, indexedObject, 0)[17])(indexedObject, nullptr);
    self->field_004c = static_cast<uint32_t>(loaderHandle);

    // Parameter block, laid out in ascending address order.
    int32_t loadParams[4];
    loadParams[0] = 0;
    loadParams[1] = 0;
    loadParams[2] = 0;
    loadParams[3] = 0x3F800000; // float 1.0f

    loadParams[0] = CSF_AT(int32_t, GetGlobalValue_8BC1E0(), 0xB34);
    loadParams[1] = CSF_AT(int32_t, GetGlobalValue_8BC1E0(), 0xB38);
    loadParams[2] = CSF_AT(int32_t, GetGlobalValue_8BC1E0(), 0xB3C);
    loadParams[3] = CSF_AT(int32_t, GetGlobalValue_8BC1E0(), 0xB40);

    // Reload the loader after the calls above; virtual slot 0x2C / 4 = 11.
    void* loaderObject = reinterpret_cast<void*>(static_cast<uintptr_t>(self->field_004c));
    return ((int(__fastcall*)(void*, void*, int32_t*))CSF_AT(void**, loaderObject, 0)[11])(loaderObject, nullptr, loadParams);
}

HOOK(0x538540, CMenuLoading_ConfigureLoader, "auto");
