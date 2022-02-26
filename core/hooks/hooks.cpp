#include "../../dependencies/utilities/csgo.hpp"
#include "../features/features.hpp"
#include "../features/misc/engine_prediction.hpp"
#include "../menu/menu.hpp"

hooks::create_move::fn create_move_original = nullptr;
hooks::paint_traverse::fn paint_traverse_original = nullptr;
hooks::do_effect::fn do_effect_original = nullptr;
hooks::emit_sound::fn emit_sound_original = nullptr;
hooks::game_event::fn game_event_original = nullptr;
hooks::frame_stage_notify::fn frame_stage_notify_original = nullptr;
hooks::lock_cursor::fn lock_cursor_original = nullptr;
hooks::present::fn present_original = nullptr;
hooks::reset::fn reset_original = nullptr;
WNDPROC hooks::wndproc_original = NULL;
uint8_t* present_address;
uint8_t* reset_address;
HWND hooks::window;

bool hooks::initialize() {
	const auto create_move_target = reinterpret_cast<void*>(get_virtual(interfaces::clientmode, 24));
	const auto paint_traverse_target = reinterpret_cast<void*>(get_virtual(interfaces::panel, 41));
	const auto lock_cursor_target = reinterpret_cast<void*>(get_virtual(interfaces::surface, 67));
	const auto do_effect_target = reinterpret_cast<void*>(get_virtual(interfaces::clientmode, 44));
	const auto emit_sound_target = reinterpret_cast<void*>(get_virtual(interfaces::engine_sound, 5));
	const auto game_event_target = reinterpret_cast<void*>(get_virtual(interfaces::event_manager, 9));
	const auto frame_stage_notify_target = reinterpret_cast<void*>(get_virtual(interfaces::client, 37));
	present_address = utilities::pattern_scan("gameoverlayrenderer.dll", "FF 15 ? ? ? ? 8B F0 EB 1B") + 0x02;
	reset_address = utilities::pattern_scan("gameoverlayrenderer.dll", "C7 45 ? ? ? ? ? FF 15 ? ? ? ? 8B D8") + 0x09;

	if (MH_Initialize() != MH_OK)
		throw std::runtime_error("failed to initialize MH_Initialize.");

	if (MH_CreateHook(create_move_target, &create_move::hook, reinterpret_cast<void**>(&create_move_original)) != MH_OK)
		throw std::runtime_error("failed to initialize create_move. (outdated index?)");

	if (MH_CreateHook(paint_traverse_target, &paint_traverse::hook, reinterpret_cast<void**>(&paint_traverse_original)) != MH_OK)
		throw std::runtime_error("failed to initialize paint_traverse. (outdated index?)");

	if (MH_CreateHook(lock_cursor_target, &lock_cursor::hook, reinterpret_cast<void**>(&lock_cursor_original)) != MH_OK)
		throw std::runtime_error("failed to initialize lock_cursor. (outdated index?)");

	if (MH_CreateHook(do_effect_target, &do_effect::hook, reinterpret_cast<void**>(&do_effect_original)) != MH_OK)
		throw std::runtime_error("failed to initialize do_effect. (outdated index?)");

	if (MH_CreateHook(emit_sound_target, &emit_sound::hook, reinterpret_cast<void**>(&emit_sound_original)) != MH_OK)
		throw std::runtime_error("failed to initialize emit_sound. (outdated index?)");

	if (MH_CreateHook(game_event_target, &game_event::hook, reinterpret_cast<void**>(&game_event_original)) != MH_OK)
		throw std::runtime_error("failed to initialize game_event. (outdated index?)");

	if (MH_CreateHook(frame_stage_notify_target, &frame_stage_notify::hook, reinterpret_cast<void**>(&frame_stage_notify_original)) != MH_OK)
		throw std::runtime_error("failed to initialize frame_stage_notify. (outdated index?)");

	if (MH_EnableHook(MH_ALL_HOOKS) != MH_OK)
		throw std::runtime_error("failed to enable hooks.");

	present_original = **reinterpret_cast<present::fn**>(present_address);
	reset_original = **reinterpret_cast<reset::fn**>(reset_address);

	**reinterpret_cast<void***>(present_address) = reinterpret_cast<void*>(&present::hook);
	**reinterpret_cast<void***>(reset_address) = reinterpret_cast<void*>(&reset::hook);

	window = FindWindow("Valve001", NULL);
	wndproc_original = reinterpret_cast<WNDPROC>(SetWindowLongW(window, GWL_WNDPROC, reinterpret_cast<LONG>(hooks::wndproc::hook)));

	console::log("[setup] hooks initialized!\n");
	return true;
}

void hooks::release() {
	MH_Uninitialize();

	MH_DisableHook(MH_ALL_HOOKS);

	**reinterpret_cast<void***>(present_address) = reinterpret_cast<void*>(present_original);
	**reinterpret_cast<void***>(reset_address) = reinterpret_cast<void*>(present_original);
	SetWindowLongW(FindWindowW(L"Valve001", NULL), GWL_WNDPROC, reinterpret_cast<LONG>(wndproc_original));
}

extern LRESULT ImGui_ImplDX9_WndProcHandler(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);

static bool initialized = false;
long __stdcall hooks::present::hook(IDirect3DDevice9* device, RECT* source_rect, RECT* dest_rect, HWND dest_window_override, RGNDATA* dirty_region) {
	if (!initialized) {
		menu.apply_fonts();
		menu.setup_resent(device);
		initialized = true;
	}
	if (initialized) {
		menu.pre_render(device);
		menu.post_render();

		menu.run();
		if (interfaces::engine->is_connected() && interfaces::engine->is_in_game()) {
			misc::speclist();
			misc::velind();
			misc::stamind();
		}
		misc::watermark();
		menu.end_present(device);
	}

	return present_original(device, source_rect, dest_rect, dest_window_override, dirty_region);
}

bool __stdcall hooks::create_move::hook(float input_sample_frametime, c_usercmd* cmd) {
	create_move_original(input_sample_frametime, cmd);

	if (!cmd || !cmd->command_number)
		return false;

	csgo::local_player = static_cast<player_t*>(interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player()));

	uintptr_t* frame_pointer;
	__asm mov frame_pointer, ebp;
	bool& send_packet = *reinterpret_cast<bool*>(*frame_pointer - 0x34C);

	auto old_viewangles = cmd->viewangles;
	auto old_forwardmove = cmd->forwardmove;
	auto old_sidemove = cmd->sidemove;

	if (interfaces::engine->is_connected() && interfaces::engine->is_in_game()) {

		if (cfg.aimbot.backtrack && csgo::local_player->is_alive())
		{
			if (cmd->command_number)
			{
				aimbot::regtick(cmd);
			}
			aimbot::begin(cmd);
			aimbot::end();
		}

		const int unpredicted_flags = csgo::local_player->flags();
		const float unpredicted_velocity = std::floor(csgo::local_player->velocity().z);

		misc::bunnyhop(cmd);

		prediction::run(cmd, [&]() {
			aimbot::aimbot(cmd);
			});

		const int predicted_flags = csgo::local_player->flags();
		const int predicted_velocity = std::floor(csgo::local_player->velocity().z);

		misc::ebsound(unpredicted_velocity);
		misc::jumpbug(cmd, unpredicted_flags);
		misc::edgebug(cmd, unpredicted_flags, predicted_flags, unpredicted_velocity);
		misc::edgejump(cmd, unpredicted_flags);
		misc::longjump(cmd, unpredicted_flags);
		misc::minijump(cmd, unpredicted_flags);

		misc::rankreveal();

		math::correct_movement(old_viewangles, cmd, old_forwardmove, old_sidemove);

		cmd->forwardmove = std::clamp(cmd->forwardmove, -450.0f, 450.0f);
		cmd->sidemove = std::clamp(cmd->sidemove, -450.0f, 450.0f);
		cmd->upmove = std::clamp(cmd->upmove, -320.0f, 320.0f);

		cmd->viewangles.normalize();
		cmd->viewangles.x = std::clamp(cmd->viewangles.x, -89.0f, 89.0f);
		cmd->viewangles.y = std::clamp(cmd->viewangles.y, -180.0f, 180.0f);
		cmd->viewangles.z = 0.0f;
	}

	return false;
}

void __stdcall hooks::paint_traverse::hook(unsigned int panel, bool force_repaint, bool allow_force) {
	auto panel_to_draw = fnv::hash(interfaces::panel->get_panel_name(panel));

	switch (panel_to_draw) {
	case fnv::hash("MatSystemTopPanel"):
		if (interfaces::engine->is_connected() && interfaces::engine->is_in_game()) {
			visuals::run();
			misc::stamgraph();
			misc::velgraph();
		}

		break;
	}

	paint_traverse_original(interfaces::panel, panel, force_repaint, allow_force);
}

int __fastcall hooks::do_effect::hook(void* ecx, void* edx, int a1) {
	visuals::glow();

	return do_effect_original(ecx, edx, a1);
}

void __stdcall hooks::emit_sound::hook(void* filter, int entity_index, int channel, const char* sound_entry, unsigned int sound_entry_hash, const char* sample, float volume, int seed, int sound_level, int flags, int pitch, const vec3_t& origin, const vec3_t& direction, void* vec_origins, bool update_pos, float sound_time, int speaker_entity, void* sound_params) {
	misc::autoaccept(sound_entry);

	if (prediction::in_prediction)
		return;

	volume = std::clamp(volume, 0.0f, 1.0f);
	return emit_sound_original(interfaces::client, filter, entity_index, channel, sound_entry, sound_entry_hash, sample, volume, seed, sound_level, flags, pitch, std::cref(origin), std::cref(direction), vec_origins, update_pos, sound_time, speaker_entity, sound_params);
}

bool __stdcall hooks::game_event::hook(i_game_event* event) {
	auto event_name = event->get_name();

	if (!strcmp(event_name, "player_hurt")) {
		misc::hitsound(event);
	}

	return game_event_original(interfaces::event_manager, event);
}

void __stdcall hooks::frame_stage_notify::hook(int frame_stage) {
	if (frame_stage == FRAME_NET_UPDATE_POSTDATAUPDATE_START ) {
		skins::knife();
		skins::gloves();
	}
	else if (frame_stage == FRAME_START) {
		visuals::fog();
	}
	
	skins::agents(frame_stage);

	frame_stage_notify_original(interfaces::client, frame_stage);
}

LRESULT __stdcall hooks::wndproc::hook(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam) {
	static bool pressed = false;

	if (!pressed && GetAsyncKeyState(VK_INSERT)) {
		pressed = true;
	}
	else if (pressed && !GetAsyncKeyState(VK_INSERT)) {
		pressed = false;

		menu.opened = !menu.opened;
	}

	if (menu.opened) {
		interfaces::inputsystem->enable_input(false);

	}
	else if (!menu.opened) {
		interfaces::inputsystem->enable_input(true);
	}

	if (menu.opened && ImGui_ImplDX9_WndProcHandler(hwnd, message, wparam, lparam))
		return true;

	return CallWindowProcA(wndproc_original, hwnd, message, wparam, lparam);
}

void __stdcall hooks::lock_cursor::hook(c_usercmd* cmd) {
	if (menu.opened) {
		interfaces::surface->unlock_cursor();
		return;
	}

	return lock_cursor_original(interfaces::surface);
}

long __stdcall hooks::reset::hook(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* present_parameters) {
	if (!initialized)
		reset_original(device, present_parameters);

	menu.invalidate_objects();
	long hr = reset_original(device, present_parameters);
	menu.create_objects(device);

	return hr;
}
