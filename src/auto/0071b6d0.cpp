#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, ConstructStringFromText, 0x00434150,
         (void* self, void* edx, const char* text));
CSF_FUNC(void*, __fastcall, AssignStringRange, 0x004109E0,
         (void* self, void* edx, void* source, uint32_t start, uint32_t count));
CSF_FUNC(int, __fastcall, DestroyString, 0x00544310,
         (void* self, void* edx));

static bool TextEquals(const char* left, const char* right)
{
    for (;;)
    {
        const unsigned char value = static_cast<unsigned char>(*left);
        if (value != static_cast<unsigned char>(*right))
            return false;
        if (value == 0)
            return true;
        ++left;
        ++right;
    }
}

static int __fastcall C_00864508_ActivateEntry(
    C_00864508* self, void* /*edx*/, int index)
{
    auto* entry = reinterpret_cast<uint8_t*>(
        self->field_0014 + (static_cast<uint32_t>(index) << 6));

    void* controller = CSF_AT(void*, entry, 0x38);
    const bool alreadyActive =
        ((int(__fastcall*)(void*, void*))
            CSF_AT(void**, controller, 0)[0x20 / 4])(controller, nullptr) != 0;

    controller = CSF_AT(void*, entry, 0x38);
    void* activatedObject =
        ((void*(__fastcall*)(void*, void*, C_00864508*, uint32_t))
            CSF_AT(void**, controller, 0)[0x08 / 4])(
                controller, nullptr, self, CSF_AT(uint32_t, entry, 0x3C));

    controller = CSF_AT(void*, entry, 0x38);
    int result =
        ((int(__fastcall*)(void*, void*))
            CSF_AT(void**, controller, 0)[0x20 / 4])(controller, nullptr);

    if (result != 0)
    {
        controller = CSF_AT(void*, entry, 0x38);
        void* activeObject =
            ((void*(__fastcall*)(void*, void*))
                CSF_AT(void**, controller, 0)[0x20 / 4])(controller, nullptr);
        result =
            ((int(__fastcall*)(void*, void*))
                CSF_AT(void**, activeObject, 0)[0x54 / 4])(activeObject, nullptr);
    }

    if (activatedObject == nullptr)
        return result;

    if (!alreadyActive)
    {
        ((void(__fastcall*)(void*, void*))
            CSF_AT(void**, activatedObject, 0)[0x1A4 / 4])(
                activatedObject, nullptr);

        const char* objectName =
            ((const char*(__fastcall*)(void*, void*))
                CSF_AT(void**, activatedObject, 0)[0x190 / 4])(
                    activatedObject, nullptr);

        const char* previousName = self->field_0094 < 0x10u
            ? reinterpret_cast<const char*>(&self->field_0080)
            : reinterpret_cast<const char*>(self->field_0080);

        const char* emptyMarker = CSF_STR(0x0082E65C, "");
        if (TextEquals(objectName, previousName) ||
            objectName[0] == emptyMarker[0])
        {
            float initialValue;
            std::memcpy(&initialValue, &self->field_0070, sizeof(initialValue));
            self->field_0078 = initialValue;
        }
        else
        {
            self->field_0078 = self->field_0074;
        }

        void* storedName = reinterpret_cast<uint8_t*>(self) + 0x7C;
        uint32_t temporaryString[7];
        void* source;
        const bool constructTemporary = objectName[0] != emptyMarker[0];
        if (constructTemporary)
            source = ConstructStringFromText(temporaryString, nullptr, objectName);
        else
            source = storedName;

        AssignStringRange(storedName, nullptr, source, 0, UINT32_MAX);
        if (constructTemporary)
            DestroyString(temporaryString, nullptr);
    }

    ((void(__fastcall*)(void*, void*, uint32_t))
        CSF_AT(void**, activatedObject, 0)[0x1B4 / 4])(
            activatedObject, nullptr, self->field_00a1);

    controller = CSF_AT(void*, entry, 0x38);
    ((void(__fastcall*)(C_00864508*, void*, void*))
        CSF_AT(void**, self, 0)[0xB8 / 4])(self, nullptr, controller);

    controller = CSF_AT(void*, entry, 0x38);
    result =
        ((int(__fastcall*)(void*, void*))
            CSF_AT(void**, controller, 0)[0x28 / 4])(controller, nullptr);

    if (static_cast<uint8_t>(result) != 0)
    {
        controller = CSF_AT(void*, entry, 0x38);
        result =
            ((int(__fastcall*)(C_00864508*, void*, void*, int))
                CSF_AT(void**, self, 0)[0x7C / 4])(
                    self, nullptr, controller, 1);
    }
    return result;
}

HOOK(0x0071B6D0, C_00864508_ActivateEntry, "auto");
