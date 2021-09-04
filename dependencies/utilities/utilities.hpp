#pragma once
#include <cstdint>
#include "../../source-sdk/math/vector3d.hpp"

namespace utilities {
	template< typename t >
	__forceinline static t call_virtual( void *name, int offset ) {
		int *table = *( int ** ) name;
		int address = table[ offset ];
		return ( t ) ( address );
	}

	struct hud_weapons_t {
		std::int32_t* get_weapon_count() {
			return reinterpret_cast<std::int32_t*>(std::uint32_t(this) + 0x80);
		}
	};

	std::uint8_t* pattern_scan(const char* module_name, const char* signature) noexcept;
	template<class T>
	static T* find_hud_element(const char* name);
	void force_update();
}
