#include "../features.hpp"

void smooth(vec3_t& viewangles, vec3_t& angles, int amount)
{
	angles = (viewangles + (angles - viewangles).clamped() / amount).clamped();
}

bool aimbot::aimbot(c_usercmd* cmd)
{
	if (!cfg.aimbot.aimbot)
		return false;

	if (!interfaces::inputsystem->is_button_down(button_code_t::MOUSE_LEFT))
		return false;

	if (csgo::local_player->active_weapon()->is_reloading())

	if (csgo::local_player->next_attack() > interfaces::globals->cur_time)
		return false;

	if (!csgo::local_player->is_alive())
		return false;

	if (!csgo::local_player)
		return false;

	int fov2{};
	int smoothness{};
	int rcs{};
	int hitbox{};
	bool silent = cfg.aimbot.silent;

	auto weapon = csgo::local_player->active_weapon();

	if (weapon->get_weapon_data()->weapon_type == WEAPONTYPE_PISTOL) {
		fov2 = cfg.aimbot.fovpistol;
		smoothness = cfg.aimbot.smoothpistol;
		rcs = cfg.aimbot.rcspistol;
		switch (cfg.aimbot.bonepistol) {
		case 0:
			hitbox = hitbox_head;
			break;
		case 1:
			hitbox = hitbox_neck;
			break;
		case 2:
			hitbox = hitbox_chest;
			break;
		case 3:
			hitbox = hitbox_stomach;
			break;
		case 4:
			hitbox = hitbox_pelvis;
			break;
		}
	}
	if (weapon->get_weapon_data()->weapon_type == WEAPONTYPE_MACHINEGUN || weapon->get_weapon_data()->weapon_type == WEAPONTYPE_SHOTGUN) {
		fov2 = cfg.aimbot.fovheavy;
		smoothness = cfg.aimbot.smoothheavy;
		rcs = cfg.aimbot.rcsheavy;
		switch (cfg.aimbot.boneheavy) {
		case 0:
			hitbox = hitbox_head;
			break;
		case 1:
			hitbox = hitbox_neck;
			break;
		case 2:
			hitbox = hitbox_chest;
			break;
		case 3:
			hitbox = hitbox_stomach;
			break;
		case 4:
			hitbox = hitbox_pelvis;
			break;
		}
	}
	if (weapon->get_weapon_data()->weapon_type == WEAPONTYPE_SUBMACHINEGUN) {
		fov2 = cfg.aimbot.fovsmg;
		smoothness = cfg.aimbot.smoothsmg;
		rcs = cfg.aimbot.rcssmg;
		switch (cfg.aimbot.bonesmg) {
		case 0:
			hitbox = hitbox_head;
			break;
		case 1:
			hitbox = hitbox_neck;
			break;
		case 2:
			hitbox = hitbox_chest;
			break;
		case 3:
			hitbox = hitbox_stomach;
			break;
		case 4:
			hitbox = hitbox_pelvis;
			break;
		}
	}
	if (weapon->get_weapon_data()->weapon_type == WEAPONTYPE_RIFLE) {
		fov2 = cfg.aimbot.fovrifle;
		smoothness = cfg.aimbot.smoothrifle;
		rcs = cfg.aimbot.rcsrifle;
		switch (cfg.aimbot.bonerifle) {
		case 0:
			hitbox = hitbox_head;
			break;
		case 1:
			hitbox = hitbox_neck;
			break;
		case 2:
			hitbox = hitbox_chest;
			break;
		case 3:
			hitbox = hitbox_stomach;
			break;
		case 4:
			hitbox = hitbox_pelvis;
			break;
		}
	}
	if (weapon->get_weapon_data()->weapon_type == WEAPONTYPE_SNIPER_RIFLE) {
		fov2 = cfg.aimbot.fovsniper;
		smoothness = cfg.aimbot.smoothsniper;
		rcs = cfg.aimbot.rcssniper;
		switch (cfg.aimbot.bonesniper) {
		case 0:
			hitbox = hitbox_head;
			break;
		case 1:
			hitbox = hitbox_neck;
			break;
		case 2:
			hitbox = hitbox_chest;
			break;
		case 3:
			hitbox = hitbox_stomach;
			break;
		case 4:
			hitbox = hitbox_pelvis;
			break;
		}
	}

	auto local = csgo::local_player;

	if (!local || !local->is_alive() || local->is_defusing())
		return false;

	struct data {
		float fov = 180;
		int target = -1;
	} data;

	vec3_t final_angles, viewangles;
	interfaces::engine->get_view_angles(viewangles);

	auto recoil_scale = interfaces::console->get_convar("weapon_recoil_scale");
	auto aim_punch = local->aim_punch_angle() * recoil_scale->float_value;
	aim_punch.x *= rcs;
	aim_punch.y *= rcs;

	for (int i = 0; i < interfaces::globals->max_clients; i++)
	{
		auto ent = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(i));

		if (!ent || !ent->is_alive() || ent->dormant() || ent->has_gun_game_immunity())
			continue;

		const bool is_enemy = ent->team() != csgo::local_player->team();

		if (!is_enemy)
			continue;

		const vec3_t eye_pos = csgo::local_player->get_eye_pos();
		vec3_t eye_pos_nonconst = csgo::local_player->get_eye_pos();

		if (hitbox <= 5) {
			for (int x = 0; x < 5; x++)
			{
				const vec3_t hitbox_pos = ent->get_hitbox_position(hitbox);
				vec3_t hitbox_pos_nonconst = ent->get_hitbox_position(hitbox);

				const vec3_t punch = local->aim_punch_angle();

				vec3_t angles = math::calculate_angle(eye_pos_nonconst, hitbox_pos_nonconst) - punch * (rcs / 50);
				const vec3_t fov_angles = math::calculate_angle(eye_pos, hitbox_pos, viewangles);

				if (!local->can_see_player_pos(ent, hitbox_pos))
					continue;

				angles.normalize();

				const float fov = fov_angles.length_2d();

				if (data.fov > fov && fov < fov2)
				{
					data.fov = fov;
					data.target = i;
					final_angles = angles;
				}
			}
		}
		else if (hitbox == 6) {
			for (int x = 0; x < 5; x++)
			{
				const vec3_t hitbox_pos = ent->get_hitbox_position(x);
				vec3_t hitbox_pos_nonconst = ent->get_hitbox_position(x);

				const vec3_t punch = local->aim_punch_angle();

				vec3_t angles = math::calculate_angle(eye_pos_nonconst, hitbox_pos_nonconst) - punch * (rcs / 50);
				const vec3_t fov_angles = math::calculate_angle(eye_pos, hitbox_pos, viewangles);

				if (!local->can_see_player_pos(ent, hitbox_pos))
					continue;

				angles.normalize();

				const float fov = fov_angles.length_2d();

				if (data.fov > fov && fov < fov2)
				{
					data.fov = fov;
					data.target = i;
					final_angles = angles;
				}
			}
		}
	}

	if (data.target == -1)
		return false;

	smooth(viewangles, final_angles, smoothness);

	if (!silent)
		interfaces::engine->set_view_angles(final_angles);

	cmd->viewangles = final_angles;

	return true;
}