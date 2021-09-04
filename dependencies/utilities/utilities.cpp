#include "utilities.hpp"
#include "../utilities/csgo.hpp"
#include <psapi.h>

std::uint8_t* utilities::pattern_scan(const char* module_name, const char* signature) noexcept {
    const auto module_handle = GetModuleHandleA(module_name);

    if (!module_handle)
        return nullptr;

    static auto pattern_to_byte = [](const char* pattern) {
        auto bytes = std::vector<int>{};
        auto start = const_cast<char*>(pattern);
        auto end = const_cast<char*>(pattern) + std::strlen(pattern);

        for (auto current = start; current < end; ++current) {
            if (*current == '?') {
                ++current;

                if (*current == '?')
                    ++current;

                bytes.push_back(-1);
            }
            else {
                bytes.push_back(std::strtoul(current, &current, 16));
            }
        }
        return bytes;
    };

    auto dos_header = reinterpret_cast<PIMAGE_DOS_HEADER>(module_handle);
    auto nt_headers =
        reinterpret_cast<PIMAGE_NT_HEADERS>(reinterpret_cast<std::uint8_t*>(module_handle) + dos_header->e_lfanew);

    auto size_of_image = nt_headers->OptionalHeader.SizeOfImage;
    auto pattern_bytes = pattern_to_byte(signature);
    auto scan_bytes = reinterpret_cast<std::uint8_t*>(module_handle);

    auto s = pattern_bytes.size();
    auto d = pattern_bytes.data();

    for (auto i = 0ul; i < size_of_image - s; ++i) {
        bool found = true;

        for (auto j = 0ul; j < s; ++j) {
            if (scan_bytes[i + j] != d[j] && d[j] != -1) {
                found = false;
                break;
            }
        }
        if (found)
            return &scan_bytes[i];
    }

    throw std::runtime_error(std::string("Wrong signature: ") + signature);
}


template<class T>
static T* utilities::find_hud_element(const char* name) {
    static auto fn = *reinterpret_cast<DWORD**>(utilities::pattern_scan("client.dll", "B9 ? ? ? ? E8 ? ? ? ? 8B 5D 08") + 1);

    static auto find_hud_element = reinterpret_cast<DWORD(__thiscall*)(void*, const char*)>(utilities::pattern_scan("client.dll", "55 8B EC 53 8B 5D 08 56 57 8B F9 33 F6 39 77 28"));
    return (T*)find_hud_element(fn, name);
}


void utilities::force_update() {
    static auto fn = utilities::pattern_scan("client.dll", "55 8B EC 51 53 56 8B 75 08 8B D9 57 6B FE 2C 89 5D FC");
    static auto full_update = utilities::pattern_scan("engine.dll", "A1 ? ? ? ? B9 ? ? ? ? 56 FF 50 14 8B 34 85");

    if (full_update)
    {
        static auto clear_hud_icon = fn;

        if (clear_hud_icon)
        {
            static auto clear_hud_weapon_icon_fn = reinterpret_cast<std::int32_t(__thiscall*)(void*, std::int32_t)>(clear_hud_icon);
            auto element = utilities::find_hud_element<std::uint32_t*>(("CCSGO_HudWeaponSelection"));

            if (element && clear_hud_weapon_icon_fn)
            {
                auto hud_weapons = reinterpret_cast<hud_weapons_t*>(std::uint32_t(element) - 0xA0);
                if (hud_weapons == nullptr)
                    return;

                if (!*hud_weapons->get_weapon_count())
                    return;

                for (std::int32_t i = 0; i < *hud_weapons->get_weapon_count(); i++)
                    i = clear_hud_weapon_icon_fn(hud_weapons, i);

                typedef void(*cl_full_update_t) (void);
                cl_full_update_t cl_fullupdate = (cl_full_update_t)(full_update);

                cl_fullupdate();
            }
        }
    }
}