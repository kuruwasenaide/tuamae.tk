#include "../features.hpp"
#include "engine_prediction.hpp"
#include "../../menu/menu.hpp"

void misc::bunnyhop(c_usercmd* cmd) {
	if (!cfg.misc.bunnyhop)
		return;

	if (cfg.misc.jumpbug &&
		GetAsyncKeyState(cfg.misc.jumpbugkey))
		return;

	const int move_type = csgo::local_player->move_type();

	if (move_type == movetype_ladder || move_type == movetype_noclip || move_type == movetype_observer)
		return;

	static auto wasLastTimeOnGround{ csgo::local_player->flags() & 1 };

	if (!(csgo::local_player->flags() & 1) && !wasLastTimeOnGround)
		cmd->buttons &= ~in_jump;

	wasLastTimeOnGround = csgo::local_player->flags() & 1;
};

void misc::jumpbug(c_usercmd* cmd, int unpredicted_flags) {
	if (!cfg.misc.jumpbug)
		return;
	if (!GetAsyncKeyState(cfg.misc.jumpbugkey))
		return;

	const int move_type = csgo::local_player->move_type();

	if (move_type == movetype_ladder || move_type == movetype_noclip || move_type == movetype_observer)
		return;

	if ((csgo::local_player->flags() & fl_onground) && !(unpredicted_flags & fl_onground)) {
		cmd->buttons |= in_duck;
	}

	if ((csgo::local_player->flags() & fl_onground))
		cmd->buttons &= ~in_jump;
};

void misc::edgejump(c_usercmd* cmd, int unpredicted_flags) {
	if (!cfg.misc.edgejump)
	return;
	if (!GetAsyncKeyState(cfg.misc.edgejumpkey))
		return;

	const int move_type = csgo::local_player->move_type();

	if (move_type == movetype_ladder || move_type == movetype_noclip || move_type == movetype_observer)
		return;

	if (!(csgo::local_player->flags() & fl_onground) && (unpredicted_flags & fl_onground)) {
		cmd->buttons |= in_jump;
		if (cfg.misc.edgejumpkeyrelease) {
			interfaces::engine->execute_cmd("-forward");
			interfaces::engine->execute_cmd("-back");
		}
	}
};

void misc::longjump(c_usercmd* cmd, int unpredicted_flags) {
	if (!cfg.misc.longjump)
		return;
	if (!GetAsyncKeyState(cfg.misc.longjumpkey))
		return;

	const int move_type = csgo::local_player->move_type();

	if (move_type == movetype_ladder || move_type == movetype_noclip || move_type == movetype_observer)
		return;

	if (cfg.misc.longjumpej) {
		if (!(csgo::local_player->flags() & fl_onground) && (unpredicted_flags & fl_onground))
			cmd->buttons |= in_jump;

		if (!(csgo::local_player->flags() & fl_onground))
			cmd->buttons |= in_duck;
	}
	else {
		cmd->buttons |= in_jump;
		cmd->buttons |= in_duck;
	}

	if (!(csgo::local_player->flags() & fl_onground) && (unpredicted_flags & fl_onground)) {
		if (cfg.misc.longjumpkeyrelease) {
			interfaces::engine->execute_cmd("-forward");
			interfaces::engine->execute_cmd("-back");
		}
	}
};

void misc::minijump(c_usercmd* cmd, int unpredicted_flags) {
	if (!cfg.misc.minijump)
		return;
	if (!GetAsyncKeyState(cfg.misc.minijumpkey))
		return;

	const int move_type = csgo::local_player->move_type();

	if (move_type == movetype_ladder || move_type == movetype_noclip || move_type == movetype_observer)
		return;

	if (!(csgo::local_player->flags() & fl_onground) && (unpredicted_flags & fl_onground)) {
		cmd->buttons |= in_jump;
		cmd->buttons |= in_duck;
	}
};

int takeoff, lastvel, lasttick = 0;
bool takeoff2 = false;
int delay;
void misc::velind() {
	if (!cfg.misc.velind)
		return;

	if (!csgo::local_player->is_alive())
		return;

	auto local = csgo::local_player;
	int velocity = std::round(prediction::velocity.length_2d());
	std::string velocity_string = std::to_string(velocity);
	int screenW, screenH;
	interfaces::engine->get_screen_size(screenW, screenH);

	auto delta = velocity - lastvel;

	if (cfg.misc.takeoff) {
		if (!(prediction::flags & fl_onground)) {
			if (!takeoff2)
			{
				takeoff = velocity;
				takeoff2 = true;
			}
			velocity_string = std::to_string(velocity) + " (" + std::to_string(takeoff) + ")";
		}
		else {
			if (takeoff2)
			{
				takeoff2 = false;
			}
				velocity_string = std::to_string(velocity);
		}
	}
	else velocity_string = std::to_string(velocity);

	ImGui::GetStyle().Colors[ImGuiCol_WindowBg] = ImVec4(0, 0, 0, 0);
	ImGui::Begin("velind", &cfg.misc.speclist, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus); {
		ImGui::SetWindowPos(ImVec2(screenW / 2 - (ImGui::GetWindowSize().x / 2), screenH / 2 + 200 + cfg.misc.velindoffset));
		ImGui::PushFont(menu.font_big);
		ImGui::SetWindowSize(ImVec2(ImGui::CalcTextSize(velocity_string.c_str()).x + 5, ImGui::CalcTextSize(velocity_string.c_str()).y + 5));
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (ImGui::GetWindowSize().x / 2) - (ImGui::CalcTextSize(velocity_string.c_str()).x / 2));
		if (delta > 0) {
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(cfg.misc.good[0], cfg.misc.good[1], cfg.misc.good[2], 255));
			ImGui::Text(velocity_string.c_str());
			ImGui::PopStyleColor();
		}

		else if (delta == 0) {
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(cfg.misc.normal[0], cfg.misc.normal[1], cfg.misc.normal[2], 255));
			ImGui::Text(velocity_string.c_str());
			ImGui::PopStyleColor();
		}

		else if (delta < 0) {
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(cfg.misc.bad[0], cfg.misc.bad[1], cfg.misc.bad[2], 255));
			ImGui::Text(velocity_string.c_str());
			ImGui::PopStyleColor();
		}
		ImGui::PopFont();
		ImGui::GetStyle().Colors[ImGuiCol_WindowBg] = ImVec4(.065, .065, .065, 1);
	} ImGui::End();

	if (GetTickCount() > lasttick + 150)
	{
		lasttick = GetTickCount();
		lastvel = velocity;
	}
}

float takeoff3;
bool takeoff4 = false;
void misc::stamind() {
	if (!cfg.misc.stamind)
		return;

	if (!csgo::local_player->is_alive())
		return;

	auto local = csgo::local_player;
	float stamina = prediction::stamina.length_2d();
	std::string stamina_string;
	std::stringstream stamina_stream;
	int screenW, screenH;
	interfaces::engine->get_screen_size(screenW, screenH);

	if (cfg.misc.takeoff) {
		if (!(prediction::flags & fl_onground)) {
			if (!takeoff4)
			{
				takeoff3 = stamina;
				takeoff4 = true;
			}
			stamina_stream << std::fixed << std::setprecision(1) << stamina << " (" << takeoff3 << ")";
		}
		else {
			if (takeoff4)
			{
				takeoff4 = false;
			}

			stamina_stream << std::fixed << std::setprecision(1) << stamina;
		}
	}
	else stamina_stream << std::fixed << std::setprecision(1) << stamina;

	stamina_string = stamina_stream.str();

	color col(200, 200, 200, 255);

	ImGui::GetStyle().Colors[ImGuiCol_WindowBg] = ImVec4(0, 0, 0, 0);
	ImGui::Begin("stamind", &cfg.misc.speclist, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus); {
		ImGui::SetWindowPos(ImVec2(screenW / 2 - (ImGui::GetWindowSize().x / 2), screenH / 2 + 227 + cfg.misc.stamindoffset));
		ImGui::PushFont(menu.font_big);
		ImGui::SetWindowSize(ImVec2(ImGui::CalcTextSize(stamina_string.c_str()).x + 5, ImGui::CalcTextSize(stamina_string.c_str()).y + 5));
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (ImGui::GetWindowSize().x / 2) - (ImGui::CalcTextSize(stamina_string.c_str()).x / 2));
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(cfg.misc.stamcol[0], cfg.misc.stamcol[1], cfg.misc.stamcol[2], 255));
		ImGui::Text(stamina_string.c_str());
		ImGui::PopStyleColor();
		ImGui::PopFont();
		ImGui::GetStyle().Colors[ImGuiCol_WindowBg] = ImVec4(.065, .065, .065, 1);
	} ImGui::End();
}

void misc::velgraph()
{
	if (!cfg.misc.velgraph)
		return;

	if (!csgo::local_player->is_alive())
		return;

	int screenWidth, screenHeight;
	interfaces::engine->get_screen_size(screenWidth, screenHeight);

	auto local = csgo::local_player;

	static std::vector<float> velData(300, 0);

	vec3_t vecVelocity = local->velocity();
	float currentVelocity = sqrt(vecVelocity.x * vecVelocity.x + vecVelocity.y * vecVelocity.y);

	velData.erase(velData.begin());
	velData.push_back(currentVelocity);

	for (auto i = 0; i < velData.size() - 1; i++)
	{
		int cur = velData.at(i);
		int next = velData.at(i + 1);

		float max_val = (int)velData.size() - 1;
		float val = (int)i;

		render::draw_line(
			screenWidth / 2 - (velData.size() * .7 / 2) + (i - 1) * .7,
			screenHeight / 2 - (cur * .15) + 175 + cfg.misc.graphoffset,
			screenWidth / 2 - (velData.size() * .7 / 2) + i * .7,
			screenHeight / 2 - (next * .15) + 175 + cfg.misc.graphoffset,
			color(cfg.misc.velgraphcol[0] * 255, cfg.misc.velgraphcol[1] * 255, cfg.misc.velgraphcol[2] * 255, fabs(max_val - fabs(val - max_val / 2) * 2) * .7)
		);
	}
}

void misc::stamgraph()
{
	if (!cfg.misc.stamgraph)
		return;

	if (!csgo::local_player->is_alive())
		return;

	int screenWidth, screenHeight;
	interfaces::engine->get_screen_size(screenWidth, screenHeight);

	auto local = csgo::local_player;

	static std::vector<float> stamData(300, 0);

	vec3_t vecStamina = local->stamina();
	float currentStamina = sqrt(vecStamina.x * vecStamina.x + vecStamina.y * vecStamina.y);

	stamData.erase(stamData.begin());
	stamData.push_back(currentStamina);

	for (auto i = 0; i < stamData.size() - 1; i++)
	{
		int cur = stamData.at(i);
		int next = stamData.at(i + 1);

		float max_val = (int)stamData.size() - 1;
		float val = (int)i;

		render::draw_line(
			screenWidth / 2 - (stamData.size() * .7 / 2) + (i - 1) * .7,
			screenHeight / 2 - (cur * .15) + 175 + cfg.misc.graphoffset,
			screenWidth / 2 - (stamData.size() * .7 / 2) + i * .7,
			screenHeight / 2 - (next * .15) + 175 + cfg.misc.graphoffset,
			color(cfg.misc.stamgraphcol[0] * 255, cfg.misc.stamgraphcol[1] * 255, cfg.misc.stamgraphcol[2] * 255, fabs(max_val - fabs(val - max_val / 2) * 2) * .7)
		);
	}
}

void misc::ebsound(float unpredicted_velocity) {
	if (!cfg.misc.ebsound)
		return;

	if (!csgo::local_player->is_alive())
		return;

	auto local = csgo::local_player;
	auto velocity = csgo::local_player->velocity();
	auto move_type = csgo::local_player->move_type();
	bool caneb = !(unpredicted_velocity >= -7 || floor(local->velocity().z) != -7 || local->flags() & fl_onground);

	if (move_type == movetype_ladder || move_type == movetype_noclip)
		return;

	if (caneb) {
		interfaces::surface->play_sound("buttons\\arena_switch_press_02.wav");
	}
}

void misc::autoaccept(const char* sound_entry) {
	if (!cfg.misc.autoaccept)
		return;

	auto window = FindWindowW(L"Valve001", NULL);
	if (!strcmp(sound_entry, "UIPanorama.popup_accept_match_beep"))
	{
		static auto fn_accept = reinterpret_cast<bool(__stdcall*)(const char*)>(utilities::pattern_scan("client.dll", "55 8B EC 83 E4 F8 8B 4D 08 BA ? ? ? ? E8 ? ? ? ? 85 C0 75 12"));

		if (fn_accept)
		{
			fn_accept("");
			FLASHWINFO fi;
			fi.cbSize = sizeof(FLASHWINFO);
			fi.hwnd = window;
			fi.dwFlags = FLASHW_ALL | FLASHW_TIMERNOFG;
			fi.uCount = 0;
			fi.dwTimeout = 0;
			FlashWindowEx(&fi);
		}
	}
}

void misc::speclist() {
	if (!cfg.misc.speclist)
		return;

	if (!csgo::local_player)
		return;

	if (!interfaces::engine->is_connected())
		return;

	if (!interfaces::engine->is_in_game())
		return;

	int specs = 0;
	std::string spect = "";

	for (int i = 0; i < interfaces::globals->max_clients; i++)
	{
		player_t* entity = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(i));

		if (!entity)
			continue;
		if (entity->health() > 0)
			continue;
		if (entity->is_alive())
			continue;
		if (entity == csgo::local_player)
			continue;
		if (reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity_handle(entity->observer_target())) != csgo::local_player)
			continue;

		player_info_t info;
		interfaces::engine->get_player_info(entity->index(), &info);

		if (info.ishltv)
			continue;

		spect += info.name;
		spect += "\n";
		specs++;
	}

	if (specs == 0)
		return;

	if (ImGui::Begin("spectators", &cfg.misc.speclist, ImVec2(0, 0), .5, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar))
	{
		ImVec2 size = ImGui::CalcTextSize(spect.c_str());
		ImGui::SetWindowSize(ImVec2(150, size.y + 2.5));
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 2.5);
		ImGui::Text(spect.c_str());
	}
	ImGui::End();
}

void misc::rankreveal() {
	if (!cfg.misc.rankreveal)
		return;

	if (GetAsyncKeyState(VK_TAB))
		interfaces::client->dispatch_user_message(cs_um_serverrankrevealall, 0, 0, nullptr);
}

void misc::hitsound(i_game_event* event) {
	if (cfg.misc.hitsound == 0)
		return;


	auto local = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player()));
	if (!local)
		return;

	auto attacker = interfaces::entity_list->get_client_entity(interfaces::engine->get_player_for_user_id(event->get_int("attacker")));
	if (!attacker)
		return;

	if (attacker == local) {
		switch (cfg.misc.hitsound) {
		case 0:
			break;
		case 1:
			interfaces::surface->play_sound("buttons\\arena_switch_press_02.wav");
			break;
		case 2:
			interfaces::surface->play_sound("survival\\money_collect_01.wav");
			break;
		}
	}
}

void misc::edgebug(c_usercmd* cmd, int unpredicted_flags, int post_flags, float unpredicted_velocity) {
	if (!cfg.misc.edgebug)
		return;
	if (!GetAsyncKeyState(cfg.misc.edgebugkey))
		return;

	auto local = csgo::local_player;
	auto velocity = csgo::local_player->velocity();
	bool willeb = !(unpredicted_velocity >= -7 || floor(local->velocity().z) != -7 || (local->flags() & fl_onground));
	bool doeb = !(unpredicted_flags & fl_onground) && post_flags & fl_onground;

	switch (cfg.misc.edgebugtype) {
	case 0:
		if (doeb && willeb) cmd->buttons &= ~in_duck;

		else if (doeb && !willeb) cmd->buttons |= in_duck;

		break;
	case 1:
		if (local->flags() & fl_onground) {
			float max_radias = M_PI * 2;
			float step = max_radias / 128;
			float xThick = 23;
			bool unduck = cmd->buttons &= ~in_duck;
			if (unduck) {
				cmd->buttons &= ~in_duck; // duck
				unduck = false;
			}
			vec3_t pos = local->abs_origin();
			for (float a = 0.f; a < max_radias; a += step) {
				vec3_t pt;
				pt.x = (xThick * cos(a)) + pos.x;
				pt.y = (xThick * sin(a)) + pos.y;
				pt.z = pos.z;


				vec3_t pt2 = pt;
				pt2.z -= 8192;

				trace_t fag;

				ray_t ray;
				ray.initialize(pt, pt2);

				trace_filter flt;
				flt.skip = local;
				interfaces::trace_ray->trace_ray(ray, MASK_PLAYERSOLID, &flt, &fag);

				if (fag.flFraction != 1.f && fag.flFraction != 0.f) {
					cmd->buttons |= in_duck; // duck
					unduck = true;
				}
			}
			for (float a = 0.f; a < max_radias; a += step) {
				vec3_t pt;
				pt.x = ((xThick - 2.f) * cos(a)) + pos.x;
				pt.y = ((xThick - 2.f) * sin(a)) + pos.y;
				pt.z = pos.z;

				vec3_t pt2 = pt;
				pt2.z -= 8192;

				trace_t fag;

				ray_t ray;
				ray.initialize(pt, pt2);

				trace_filter flt;
				flt.skip = local;
				interfaces::trace_ray->trace_ray(ray, MASK_PLAYERSOLID, &flt, &fag);

				if (fag.flFraction != 1.f && fag.flFraction != 0.f) {
					cmd->buttons |= in_duck; // duck
					unduck = true;
				}
			}
			for (float a = 0.f; a < max_radias; a += step) {
				vec3_t pt;
				pt.x = ((xThick - 20.f) * cos(a)) + pos.x;
				pt.y = ((xThick - 20.f) * sin(a)) + pos.y;
				pt.z = pos.z;

				vec3_t pt2 = pt;
				pt2.z -= 8192;

				trace_t fag;

				ray_t ray;
				ray.initialize(pt, pt2);

				trace_filter flt;
				flt.skip = local;
				interfaces::trace_ray->trace_ray(ray, MASK_PLAYERSOLID, &flt, &fag);

				if (fag.flFraction != 1.f && fag.flFraction != 0.f) {
					cmd->buttons |= in_duck; // duck
					unduck = true;
				}
			}
		}
		break;
	case 2:
		bool dideb = !(unpredicted_velocity >= -7 || floor(local->velocity().z) != -7 || (local->flags() & fl_onground));
		bool willeb = false;
		if (dideb || unpredicted_flags & fl_onground || !GetAsyncKeyState(cfg.misc.edgebugkey)) {
			interfaces::inputsystem->enable_input(true);
		}
		for (int i = 0; i < cfg.misc.edgebugradius; i++) {
			prediction::run(cmd, [&]() {
				bool caneb = !(unpredicted_velocity >= -7 || floor(local->velocity().z) != -7 || (local->flags() & fl_onground));
				bool duck = !(unpredicted_flags & fl_onground) && post_flags & fl_onground;

				if (duck && caneb) cmd->buttons &= ~in_duck;

				else if (duck && !caneb) cmd->buttons |= in_duck;

				if (caneb) {
					interfaces::inputsystem->enable_input(false);
				}
			} );
		}
		break;
	}
}

void misc::watermark() {
	if (!cfg.config.watermark)
		return;

	std::string watermark = "tuamae.tk | beta user";

	ImGui::GetStyle().Colors[ImGuiCol_WindowBg] = ImVec4(0, 0, 0, 0);
	ImGui::Begin("watermark", &cfg.misc.speclist, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus); {
		ImGui::SetWindowPos(ImVec2(0, 0));
		ImGui::PushFont(menu.font_small);
		ImGui::SetWindowSize(ImVec2(ImGui::CalcTextSize(watermark.c_str()).x + 5, ImGui::CalcTextSize(watermark.c_str()).y + 5));
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (ImGui::GetWindowSize().x / 2) - (ImGui::CalcTextSize(watermark.c_str()).x / 2));
		ImGui::Text(watermark.c_str());
		ImGui::PopFont();
		ImGui::GetStyle().Colors[ImGuiCol_WindowBg] = ImVec4(.065, .065, .065, 1);
	} ImGui::End();
}