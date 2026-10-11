#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static void __fastcall CharacterModel_SyncCachedTransform(void* self, void* /*edx*/)
{
    void* parent = CSF_AT(void*, self, 0x04);
    void* transform =
        ((void* (__fastcall*)(void*, void*))CSF_AT(void**, parent, 0)[0x10 / 4])(
            parent, nullptr);
    float firstValue =
        ((float (__fastcall*)(void*, void*))CSF_AT(void**, transform, 0)[0x24 / 4])(
            transform, nullptr);

    if (CSF_AT(int32_t, self, 0x64) == 2139095039) {
        CSF_AT(float, self, 0x64) = firstValue;
    } else {
        parent = CSF_AT(void*, self, 0x04);
        if (CSF_AT(float, self, 0x6C) == firstValue) {
            transform =
                ((void* (__fastcall*)(void*, void*))CSF_AT(void**, parent, 0)[0x10 / 4])(
                    parent, nullptr);
            ((void (__fastcall*)(void*, void*, float))CSF_AT(void**, transform, 0)[0x30 / 4])(
                transform, nullptr, CSF_AT(float, self, 0x64));
        } else {
            transform =
                ((void* (__fastcall*)(void*, void*))CSF_AT(void**, parent, 0)[0x10 / 4])(
                    parent, nullptr);
            firstValue =
                ((float (__fastcall*)(void*, void*))CSF_AT(void**, transform, 0)[0x24 / 4])(
                    transform, nullptr);
            CSF_AT(float, self, 0x64) = firstValue;
        }
    }

    parent = CSF_AT(void*, self, 0x04);
    transform =
        ((void* (__fastcall*)(void*, void*))CSF_AT(void**, parent, 0)[0x10 / 4])(
            parent, nullptr);
    float secondValue =
        ((float (__fastcall*)(void*, void*))CSF_AT(void**, transform, 0)[0x28 / 4])(
            transform, nullptr);

    if (CSF_AT(int32_t, self, 0x68) == 2139095039) {
        CSF_AT(float, self, 0x68) = secondValue;
    } else {
        parent = CSF_AT(void*, self, 0x04);
        if (CSF_AT(float, self, 0x70) == secondValue) {
            transform =
                ((void* (__fastcall*)(void*, void*))CSF_AT(void**, parent, 0)[0x10 / 4])(
                    parent, nullptr);
            ((void (__fastcall*)(void*, void*, float))CSF_AT(void**, transform, 0)[0x34 / 4])(
                transform, nullptr, CSF_AT(float, self, 0x68));
            return;
        }

        transform =
            ((void* (__fastcall*)(void*, void*))CSF_AT(void**, parent, 0)[0x10 / 4])(
                parent, nullptr);
        secondValue =
            ((float (__fastcall*)(void*, void*))CSF_AT(void**, transform, 0)[0x28 / 4])(
                transform, nullptr);
        CSF_AT(float, self, 0x68) = secondValue;
    }
}

HOOK(0x004F5850, CharacterModel_SyncCachedTransform, "auto");
