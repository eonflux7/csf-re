#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __cdecl, SceneLoader_SerializeReference, 0x00442E70, (void* a3, bool* a2));
CSF_FUNC(char, __cdecl, Archive_SerializeObjectRef, 0x004ADF10, (void* a3, bool* a2));
CSF_FUNC(void**, __fastcall, SceneLoader_RegisterArchivedObject, 0x0069ACB0, (void* self, void* edx, int a2));

static int __fastcall C_008316d8_Serialize(
    C_008316d8* self, void* /*edx*/, int archiveAddress)
{
    void* archive = reinterpret_cast<void*>(archiveAddress);
    bool failed = false;
    if (CSF_AT(uint8_t, archive, 0) == 0)
    {
        void* stream = CSF_AT(void*, archive, 0x04);
        failed = !((bool(__fastcall*)(void*, void*, int, void*))
            CSF_AT(void**, stream, 0)[0x80 / 4])(stream, nullptr, 0, archive);
    }

    uint32_t savedContext = CSF_AT(uint32_t, archive, 0x1C);
    CSF_AT(uint8_t, archive, 0) = failed;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (!failed)
    {
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        Archive_SerializeObjectRef(&self->field_0004, reinterpret_cast<bool*>(archive));
    }

    bool ready = CSF_AT(uint8_t, archive, 0) == 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (ready)
    {
        CSF_AT(uint8_t, archive, 0x15) = 1;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        SceneLoader_SerializeReference(&self->field_0008, reinterpret_cast<bool*>(archive));
    }

    ready = CSF_AT(uint8_t, archive, 0) == 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (ready)
    {
        void* stream = CSF_AT(void*, archive, 0x04);
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        CSF_AT(uint8_t, archive, 0) =
            ((int(__fastcall*)(void*, void*, uint32_t*, void*))
                CSF_AT(void**, stream, 0)[0x58 / 4])(
                    stream, nullptr, &self->field_000c, archive) == -1;
    }

    ready = CSF_AT(uint8_t, archive, 0) == 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (ready)
    {
        void* stream = CSF_AT(void*, archive, 0x04);
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        CSF_AT(uint8_t, archive, 0) =
            ((int(__fastcall*)(void*, void*, uint8_t*, void*))
                CSF_AT(void**, stream, 0)[0x44 / 4])(
                    stream, nullptr, &self->field_0014, archive) == -1;
    }

    ready = CSF_AT(uint8_t, archive, 0) == 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (ready)
    {
        void* stream = CSF_AT(void*, archive, 0x04);
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        CSF_AT(uint8_t, archive, 0) =
            ((int(__fastcall*)(void*, void*, float*, void*))
                CSF_AT(void**, stream, 0)[0x54 / 4])(
                    stream, nullptr, &self->field_0010, archive) == -1;
    }

    ready = CSF_AT(uint8_t, archive, 0) == 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (ready)
    {
        void* stream = CSF_AT(void*, archive, 0x04);
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        CSF_AT(uint8_t, archive, 0) =
            ((int(__fastcall*)(void*, void*, uint32_t*, void*))
                CSF_AT(void**, stream, 0)[0x58 / 4])(
                    stream, nullptr, &self->field_001c, archive) == -1;
    }

    ready = CSF_AT(uint8_t, archive, 0) == 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (ready)
    {
        void* stream = CSF_AT(void*, archive, 0x04);
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        CSF_AT(uint8_t, archive, 0) =
            ((int(__fastcall*)(void*, void*, uint32_t*, void*))
                CSF_AT(void**, stream, 0)[0x58 / 4])(
                    stream, nullptr, &self->field_0020, archive) == -1;
    }

    ready = CSF_AT(uint8_t, archive, 0) == 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (ready)
    {
        void* stream = CSF_AT(void*, archive, 0x04);
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        CSF_AT(uint8_t, archive, 0) =
            ((int(__fastcall*)(void*, void*, uint32_t*, void*))
                CSF_AT(void**, stream, 0)[0x58 / 4])(
                    stream, nullptr, &self->field_0024, archive) == -1;
    }

    ready = CSF_AT(uint8_t, archive, 0) == 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (ready)
    {
        void* stream = CSF_AT(void*, archive, 0x04);
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        CSF_AT(uint8_t, archive, 0) =
            ((int(__fastcall*)(void*, void*, uint8_t*, void*))
                CSF_AT(void**, stream, 0)[0x44 / 4])(
                    stream, nullptr, &self->field_0016, archive) == -1;
    }

    ready = CSF_AT(uint8_t, archive, 0) == 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (ready)
    {
        void* stream = CSF_AT(void*, archive, 0x04);
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        CSF_AT(uint8_t, archive, 0) =
            ((int(__fastcall*)(void*, void*, float*, void*))
                CSF_AT(void**, stream, 0)[0x54 / 4])(
                    stream, nullptr, &self->field_0018, archive) == -1;
    }

    ready = CSF_AT(uint8_t, archive, 0) == 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (ready)
    {
        CSF_AT(uint8_t, archive, 0x15) = 1;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        Archive_SerializeObjectRef(&self->field_0028, reinterpret_cast<bool*>(archive));
    }

    ready = CSF_AT(uint8_t, archive, 0) == 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (ready)
    {
        void* stream = CSF_AT(void*, archive, 0x04);
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        CSF_AT(uint8_t, archive, 0) =
            ((int(__fastcall*)(void*, void*, uint32_t*, void*))
                CSF_AT(void**, stream, 0)[0x48 / 4])(
                    stream, nullptr, &self->field_0044, archive) == -1;
    }

    ready = CSF_AT(uint8_t, archive, 0) == 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (ready)
    {
        void* stream = CSF_AT(void*, archive, 0x04);
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        CSF_AT(uint8_t, archive, 0) =
            ((int(__fastcall*)(void*, void*, uint32_t*, void*))
                CSF_AT(void**, stream, 0)[0x48 / 4])(
                    stream, nullptr, &self->field_004c, archive) == -1;
    }

    ready = CSF_AT(uint8_t, archive, 0) == 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (ready)
    {
        void* stream = CSF_AT(void*, archive, 0x04);
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        CSF_AT(uint8_t, archive, 0) =
            ((int(__fastcall*)(void*, void*, uint32_t*, void*))
                CSF_AT(void**, stream, 0)[0x48 / 4])(
                    stream, nullptr, &self->field_0050, archive) == -1;
    }

    uint8_t hasArchivedState = static_cast<uint8_t>(
        static_cast<uint32_t>(archiveAddress) >> 24);
    if (CSF_AT(uint8_t, archive, 0x14) != 0)
    {
        void* entry = reinterpret_cast<void*>(self->vResolveEntryLookupValue());
        bool unavailable = !entry || self->field_0044 == uint32_t(-1);
        if (!unavailable)
        {
            void* state = ((void*(__fastcall*)(void*, void*))
                CSF_AT(void**, entry, 0)[0x68 / 4])(entry, nullptr);
            unavailable = ((bool(__fastcall*)(void*, void*, int))
                CSF_AT(void**, state, 0)[0x40 / 4])(state, nullptr, 0);
        }
        if (unavailable)
        {
            hasArchivedState = 0;
            self->field_0048 = 0.0f;
        }
        else
        {
            hasArchivedState = 1;
            void* state = ((void*(__fastcall*)(void*, void*))
                CSF_AT(void**, entry, 0)[0x68 / 4])(entry, nullptr);
            self->field_0048 = static_cast<float>(
                ((double(__fastcall*)(void*, void*, int))
                    CSF_AT(void**, state, 0)[0x34 / 4])(state, nullptr, 0));
        }
    }

    ready = CSF_AT(uint8_t, archive, 0) == 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (ready)
    {
        void* stream = CSF_AT(void*, archive, 0x04);
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        CSF_AT(uint8_t, archive, 0) =
            ((int(__fastcall*)(void*, void*, uint8_t*, void*))
                CSF_AT(void**, stream, 0)[0x44 / 4])(
                    stream, nullptr, &hasArchivedState, archive) == -1;
    }

    ready = CSF_AT(uint8_t, archive, 0) == 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (ready)
    {
        void* stream = CSF_AT(void*, archive, 0x04);
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        CSF_AT(uint8_t, archive, 0) =
            ((int(__fastcall*)(void*, void*, float*, void*))
                CSF_AT(void**, stream, 0)[0x54 / 4])(
                    stream, nullptr, &self->field_0048, archive) == -1;
    }

    if (CSF_AT(uint8_t, archive, 0x14) == 0 &&
        CSF_AT(uint8_t, archive, 0) == 0)
    {
        if (hasArchivedState)
        {
            SceneLoader_RegisterArchivedObject(
                CSF_AT(void*, archive, 0x10), nullptr,
                reinterpret_cast<int>(self));
        }
        else
        {
            self->field_0044 = uint32_t(-1);
        }
    }

    CSF_AT(uint32_t, archive, 0x1C) = savedContext;
    if (CSF_AT(uint8_t, archive, 0) != 0)
    {
        CSF_AT(uint8_t, archive, 0) = 1;
        return 1;
    }

    void* stream = CSF_AT(void*, archive, 0x04);
    int result = ((int(__fastcall*)(void*, void*, void*))
        CSF_AT(void**, stream, 0)[0x7C / 4])(stream, nullptr, archive);
    if (static_cast<uint8_t>(result) == 0)
    {
        CSF_AT(uint8_t, archive, 0) = 1;
        return 1;
    }

    CSF_AT(uint8_t, archive, 0) = 0;
    return result;
}

HOOK(0x00477850, C_008316d8_Serialize, "auto");
