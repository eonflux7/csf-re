#include <cstdint>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "globals.h"

CSF_FUNC(unsigned int, __fastcall, AppendObjectPointer, 0x0050CDF0,
         (void* self, void* edx, void** value));
CSF_FUNC(unsigned int, __fastcall, InsertObjectPointers, 0x0050CAA0,
         (void* self, void* edx, void* position, unsigned int count, void** value));

static void* GetLinkedObject(void* object)
{
    return ((void* (__fastcall*)(void*, void*))
        CSF_AT(void**, object, 0)[0x20 / 4])(object, nullptr);
}

static unsigned char HasOrderingFlag(void* object)
{
    return ((unsigned char (__fastcall*)(void*, void*))
        CSF_AT(void**, object, 0)[0xCC / 4])(object, nullptr);
}

static unsigned int __fastcall C_00864508_RegisterAndInsertObject(
    C_00864508* self, void* /*edx*/, void* object)
{
    void* linkedObject = GetLinkedObject(object);
    void* manager = reinterpret_cast<void*>(self->field_0040);

    ((void (__fastcall*)(void*, void*, void*))
        CSF_AT(void**, manager, 0)[0xF8 / 4])(manager, nullptr, linkedObject);

    ((void (__fastcall*)(void*, void*, void*))
        CSF_AT(void**, linkedObject, 0)[0xEC / 4])(
            linkedObject, nullptr, object);

    void* list = reinterpret_cast<uint8_t*>(self) + 0x44;
    if (GetLinkedObject(object))
    {
        if (HasOrderingFlag(GetLinkedObject(object)))
            return AppendObjectPointer(list, nullptr, &object);
    }

    const uint32_t begin = self->field_0048;
    uint32_t position = self->field_004c;

    if (begin &&
        static_cast<uint32_t>(
            static_cast<int32_t>(self->field_004c - begin) >> 2) != 0)
    {
        uint32_t currentBegin = self->field_0048;
        position -= 4;
        while (position != currentBegin)
        {
            if (GetLinkedObject(CSF_AT(void*, reinterpret_cast<void*>(position), 0)))
            {
                void* currentLinked = GetLinkedObject(
                    CSF_AT(void*, reinterpret_cast<void*>(position), 0));
                if (!HasOrderingFlag(currentLinked))
                    break;
            }

            currentBegin = self->field_0048;
            position -= 4;
        }

        if (GetLinkedObject(CSF_AT(void*, reinterpret_cast<void*>(position), 0)))
        {
            void* currentLinked = GetLinkedObject(
                CSF_AT(void*, reinterpret_cast<void*>(position), 0));
            if (!HasOrderingFlag(currentLinked))
                position += 4;
        }
    }

    return InsertObjectPointers(
        list, nullptr, reinterpret_cast<void*>(position), 1u, &object);
}

HOOK(0x0071D390, C_00864508_RegisterAndInsertObject, "auto");
