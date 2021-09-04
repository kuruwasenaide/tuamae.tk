#pragma once

inline bool b_predicting;

namespace hooks {
	extern WNDPROC wndproc_original;
	extern HWND window;
	bool initialize();
	void release();

	inline unsigned int get_virtual(void* _class, unsigned int index) { return static_cast<unsigned int>((*static_cast<int**>(_class))[index]); }

	namespace create_move {
		using fn = bool(__stdcall*)(float, c_usercmd*);
		bool __stdcall hook(float input_sample_frametime, c_usercmd* cmd);
	};

	namespace paint_traverse {
		using fn = void(__thiscall*)(i_panel*, unsigned int, bool, bool);
		void __stdcall hook(unsigned int panel, bool force_repaint, bool allow_force);
	}

	namespace do_effect {
		using fn = int(__fastcall*)(void* ecx, void* edx, int a1);
		int __fastcall hook(void* ecx, void* edx, int a1);
	}

	namespace emit_sound {
		using fn = void(__thiscall*)(void*, void*, int, int, const char*, unsigned int, const char*, float, int, int, int, int, const vec3_t&, const vec3_t&, void*, bool, float, int, void*);
		void __stdcall hook(void* filter, int entity_index, int channel, const char* sound_entry, unsigned int sound_entry_hash, const char* sample, float volume, int seed, int sound_level, int flags, int pitch, const vec3_t& origin, const vec3_t& direction, void* vec_origins, bool update_pos, float sound_time, int speaker_entity, void* sound_params);
	}

	namespace game_event {
		using fn = bool(__thiscall*)(i_game_event_manager2* manager, i_game_event* event);
		bool __stdcall hook(i_game_event* event);
	}

	namespace frame_stage_notify {
		using fn = void(__thiscall*)(i_base_client_dll*, int);
		void __stdcall hook(int frame_stage);
	}

	namespace wndproc {
		LRESULT __stdcall hook(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);
	}

	namespace lock_cursor {
		using fn = void(__thiscall*)(void*);
		void __stdcall hook(c_usercmd* cmd);
	}

	namespace present	 {
		using fn = long(__stdcall*)(IDirect3DDevice9*, RECT*, RECT*, HWND, RGNDATA*);
		long __stdcall hook(IDirect3DDevice9* device, RECT* source_rect, RECT* dest_rect, HWND dest_window_override, RGNDATA* dirty_region);
	}

	namespace reset {
		using fn = long(__stdcall*)(IDirect3DDevice9*, D3DPRESENT_PARAMETERS*);
		long __stdcall hook(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* present_parameters);
	}
}
