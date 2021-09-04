#include "../features.hpp"

#define clamp(val, min, max) (((val) > (max)) ? (max) : (((val) < (min)) ? (min) : (val)))

bool visuals::get_playerbox(player_t* ent, box& in) {
	vec3_t origin, min, max, flb, brt, blb, frt, frb, brb, blt, flt;
	int left, top, right, bottom;

	origin = ent->abs_origin();
	min = ent->collideable()->mins() + origin;
	max = ent->collideable()->maxs() + origin;

	vec3_t points[] = {
		vec3_t(min.x, min.y, min.z),
		vec3_t(min.x, max.y, min.z),
		vec3_t(max.x, max.y, min.z),
		vec3_t(max.x, min.y, min.z),
		vec3_t(max.x, max.y, max.z),
		vec3_t(min.x, max.y, max.z),
		vec3_t(min.x, min.y, max.z),
		vec3_t(max.x, min.y, max.z)
	};
	if (!interfaces::debug_overlay->world_to_screen(points[3], flb) || !interfaces::debug_overlay->world_to_screen(points[5], brt)
		|| !interfaces::debug_overlay->world_to_screen(points[0], blb) || !interfaces::debug_overlay->world_to_screen(points[4], frt)
		|| !interfaces::debug_overlay->world_to_screen(points[2], frb) || !interfaces::debug_overlay->world_to_screen(points[1], brb)
		|| !interfaces::debug_overlay->world_to_screen(points[6], blt) || !interfaces::debug_overlay->world_to_screen(points[7], flt))
		return false;

	vec3_t arr[] = { flb, brt, blb, frt, frb, brb, blt, flt };

	left = flb.x;
	top = flb.y;
	right = flb.x;
	bottom = flb.y;

	for (int i = 1; i < 8; i++) {
		if (left > arr[i].x)
			left = arr[i].x;
		if (bottom < arr[i].y)
			bottom = arr[i].y;
		if (right < arr[i].x)
			right = arr[i].x;
		if (top > arr[i].y)
			top = arr[i].y;
	}
	in.x = (int)left;
	in.y = (int)top;
	in.w = int(right - left );
	in.h = int(bottom - top);


	return true;
}

void visuals::boxes(player_t* entity, visuals::box bbox) {

	if (!cfg.visuals.box)
		return;

	int width = bbox.w / 4;

	switch (cfg.visuals.boxtype) {
	case 0:
		render::draw_rect(bbox.x, bbox.y, bbox.w, bbox.h, color(cfg.visuals.boxcol[0] * 255, cfg.visuals.boxcol[1] * 255, cfg.visuals.boxcol[2] * 255, 255));
		render::draw_rect(bbox.x - 1, bbox.y - 1, bbox.w + 2, bbox.h + 2, color(0, 0, 0, 255));
		render::draw_rect(bbox.x + 1, bbox.y + 1, bbox.w - 2, bbox.h - 2, color(0, 0, 0, 255));
		break;
	case 1:
		// Inline
		render::draw_line(bbox.x, bbox.y, bbox.x + width, bbox.y, color(cfg.visuals.boxcol[0] * 255, cfg.visuals.boxcol[1] * 255, cfg.visuals.boxcol[2] * 255, 255));
		render::draw_line(bbox.x, bbox.y, bbox.x, bbox.y + width, color(cfg.visuals.boxcol[0] * 255, cfg.visuals.boxcol[1] * 255, cfg.visuals.boxcol[2] * 255, 255));

		render::draw_line(bbox.x + bbox.w, bbox.y, bbox.x + bbox.w - width, bbox.y, color(cfg.visuals.boxcol[0] * 255, cfg.visuals.boxcol[1] * 255, cfg.visuals.boxcol[2] * 255, 255));
		render::draw_line(bbox.x + bbox.w, bbox.y, bbox.x + bbox.w, bbox.y + width, color(cfg.visuals.boxcol[0] * 255, cfg.visuals.boxcol[1] * 255, cfg.visuals.boxcol[2] * 255, 255));

		render::draw_line(bbox.x, bbox.y + bbox.h, bbox.x + width, bbox.y + bbox.h, color(cfg.visuals.boxcol[0] * 255, cfg.visuals.boxcol[1] * 255, cfg.visuals.boxcol[2] * 255, 255));
		render::draw_line(bbox.x, bbox.y + bbox.h, bbox.x, bbox.y + bbox.h - width, color(cfg.visuals.boxcol[0] * 255, cfg.visuals.boxcol[1] * 255, cfg.visuals.boxcol[2] * 255, 255));

		render::draw_line(bbox.x + bbox.w, bbox.y + bbox.h, bbox.x + bbox.w - width, bbox.y + bbox.h, color(cfg.visuals.boxcol[0] * 255, cfg.visuals.boxcol[1] * 255, cfg.visuals.boxcol[2] * 255, 255));
		render::draw_line(bbox.x + bbox.w, bbox.y + bbox.h, bbox.x + bbox.w, bbox.y + bbox.h - width, color(cfg.visuals.boxcol[0] * 255, cfg.visuals.boxcol[1] * 255, cfg.visuals.boxcol[2] * 255, 255));

		//Outline
		render::draw_line(bbox.x - 1, bbox.y - 1, bbox.x + width, bbox.y - 1, color(0, 0, 0, 255));
		render::draw_line(bbox.x - 1, bbox.y - 1, bbox.x - 1, bbox.y + width, color(0, 0, 0, 255));

		render::draw_line(bbox.x + bbox.w - 1, bbox.y - 1, bbox.x + bbox.w - width, bbox.y - 1, color(0, 0, 0, 255));
		render::draw_line(bbox.x + bbox.w + 1, bbox.y - 1, bbox.x + bbox.w + 1, bbox.y + width, color(0, 0, 0, 255));

		render::draw_line(bbox.x - 1, bbox.y + bbox.h + 1, bbox.x + width, bbox.y + bbox.h + 1, color(0, 0, 0, 255));
		render::draw_line(bbox.x - 1, bbox.y + bbox.h + 1, bbox.x - 1, bbox.y + bbox.h - width, color(0, 0, 0, 255));

		render::draw_line(bbox.x + bbox.w + 1, bbox.y + bbox.h + 1, bbox.x + bbox.w - width, bbox.y + bbox.h + 1, color(0, 0, 0, 255));
		render::draw_line(bbox.x + bbox.w + 1, bbox.y + bbox.h + 1, bbox.x + bbox.w + 1, bbox.y + bbox.h - width, color(0, 0, 0, 255));
		break;
	}
}

void visuals::name(player_t* entity, visuals::box bbox) {

	if (!cfg.visuals.name)
		return;

	player_info_t info;

	interfaces::engine->get_player_info(entity->index(), &info);

	render::text(bbox.x + (bbox.w / 2), bbox.y - 13, render::fonts::name, info.name, true, color(cfg.visuals.namecol[0] * 255, cfg.visuals.namecol[1] * 255, cfg.visuals.namecol[2] * 255, 255));
}

void visuals::health(player_t* entity, visuals::box bbox) {
	int entity_health = entity->health();
	if (entity_health <= 0)
		return;

	int hp_bar = bbox.h * (entity_health / 100.f);

	color hp_color = color(0, 0, 0, 255);
	if (cfg.visuals.healthoverride) {
		hp_color = color(cfg.visuals.healthcol[0] * 255, cfg.visuals.healthcol[1] * 255, cfg.visuals.healthcol[2] * 255, 255);
	}
	else {
		hp_color.r -= entity_health * 2.55;
		hp_color.g += entity_health * 2.55;
		hp_color.b += entity_health * 1.25;
	}

	if (cfg.visuals.healthbar) {
		render::draw_filled_rect(bbox.x - 4, bbox.y - 1, 3, bbox.h + 2, color(0, 0, 0, 255));
		render::draw_filled_rect(bbox.x - 3, bbox.y + bbox.h - hp_bar, 1, hp_bar, hp_color);
	}

	if (cfg.visuals.healthtext) {
		if (cfg.visuals.healthbar)
			render::text(bbox.x - 9 - render::get_text_size(render::fonts::flags, std::to_string(entity->health())).x, bbox.y, render::fonts::flags, std::to_string(entity->health()), false, color(255, 255, 255, 255));
		else
			render::text(bbox.x - 6 - render::get_text_size(render::fonts::flags, std::to_string(entity->health())).x, bbox.y, render::fonts::flags, std::to_string(entity->health()), false, color(255, 255, 255, 255));
	}
}

void visuals::weapon(player_t* entity, visuals::box bbox) {
	if (!cfg.visuals.weapon)
		return;

	weapon_t* weapon = reinterpret_cast<weapon_t*>(entity->active_weapon());

	if (!weapon)
		return;

	std::string weapon_name = weapon->get_weapon_data()->weapon_name;

	weapon_name.erase(0, 7);
	std::transform(weapon_name.begin(), weapon_name.end(), weapon_name.begin(), ::tolower);

	std::string ammo = "[" + std::to_string(weapon->clip1_count()) + "/" + std::to_string(weapon->get_weapon_data()->weapon_max_clip) + "]";

	if (cfg.visuals.ammo) {
		render::text(bbox.x + (bbox.w / 2), bbox.y + (bbox.h + 6), render::fonts::weapon, weapon_name, true, color(cfg.visuals.weaponcol[0] * 255, cfg.visuals.weaponcol[1] * 255, cfg.visuals.weaponcol[2] * 255, 255));
	}
	else if (!cfg.visuals.ammo || weapon->get_weapon_data()->weapon_type == WEAPONTYPE_C4 || weapon->get_weapon_data()->weapon_type == WEAPONTYPE_KNIFE || weapon->get_weapon_data()->weapon_type == WEAPONTYPE_GRENADE){
		render::text(bbox.x + (bbox.w / 2), bbox.y + (bbox.h + 1), render::fonts::weapon, weapon_name, true, color(cfg.visuals.weaponcol[0] * 255, cfg.visuals.weaponcol[1] * 255, cfg.visuals.weaponcol[2] * 255, 255));
	}
}

void visuals::ammo(player_t* entity, visuals::box bbox) {
	if (!cfg.visuals.ammo)
		return;

	weapon_t* weapon = reinterpret_cast<weapon_t*>(entity->active_weapon());

	if (!weapon)
		return;

	int weapon_ammo = weapon->clip1_count();

	if (weapon_ammo < 0)
		return;

	int weapon_ammo_bar = weapon_ammo * bbox.w / weapon->get_weapon_data()->weapon_max_clip;

	render::draw_filled_rect(bbox.x - 1, bbox.y + bbox.h + 2, bbox.w + 2, 4, color(0, 0, 0, 255));
	render::draw_filled_rect(bbox.x, bbox.y + bbox.h + 3, weapon_ammo_bar, 2, color(cfg.visuals.ammocol[0] * 255, cfg.visuals.ammocol[1] * 255, cfg.visuals.ammocol[2] * 255, 255));

	if (!(weapon_ammo <= weapon->get_weapon_data()->weapon_max_clip / 4)) return;
}

void visuals::flags(player_t* entity, visuals::box bbox) {
	static int offset;
	offset = 0;

	player_info_t info;
	interfaces::engine->get_player_info(entity->index(), &info);

	if (info.fakeplayer && cfg.visuals.money) {
		render::text(bbox.x + bbox.w + 5, bbox.y + offset, render::fonts::flags, std::string("$").append(std::to_string(entity->money())), false, color(0, 175, 0, 255));
		offset += 10;
	}

	if (info.fakeplayer && cfg.visuals.bot) {
		render::text(bbox.x + bbox.w + 5, bbox.y + offset, render::fonts::flags, "bot", false, color(255, 255, 255, 255));
		offset += 10;
	}

	if (entity->armor() > 0 && !entity->has_helmet() && cfg.visuals.armor) {
		render::text(bbox.x + bbox.w + 5, bbox.y + offset, render::fonts::flags, "k", false, color(255, 255, 255, 255));
		offset += 10;
	}
	else if (entity->armor() > 0 && entity->has_helmet() && cfg.visuals.armor) {
		render::text(bbox.x + bbox.w + 5, bbox.y + offset, render::fonts::flags, "hk", false, color(255, 255, 255, 255));
		offset += 10;
	}

	if (entity->is_scoped() && cfg.visuals.scoped) {
		render::text(bbox.x + bbox.w + 5, bbox.y + offset, render::fonts::flags, "scoped", false, color(255, 255, 255, 255));
		offset += 10;
	}

	if (entity->is_flashed() && cfg.visuals.flashed) {
		render::text(bbox.x + bbox.w + 5, bbox.y + offset, render::fonts::flags, "flashed", false, color(255, 255, 255, 255));
		offset += 10;
	}

	if (entity->is_defusing() && cfg.visuals.defusing) {
		render::text(bbox.x + bbox.w + 5, bbox.y + offset, render::fonts::flags, "defusing", false, color(255, 255, 255, 255));
		offset += 10;
	}
}

void visuals::fog() {
	std::string color = std::to_string(cfg.visuals.fogcolor[0] * 255) + " " + std::to_string(cfg.visuals.fogcolor[1] * 255) + " " + std::to_string(cfg.visuals.fogcolor[2] * 255);
	interfaces::console->get_convar("fog_override")->set_value(cfg.visuals.fogoverride);
	interfaces::console->get_convar("fog_color")->set_value(color.c_str());
	interfaces::console->get_convar("fog_start")->set_value(cfg.visuals.fogmin);
	interfaces::console->get_convar("fog_end")->set_value(cfg.visuals.fogmax);
	interfaces::console->get_convar("fog_maxdensity")->set_value(cfg.visuals.fogcolor[3]);
}

void run_player(player_t* entity) {

	if (!entity
		|| !csgo::local_player
		|| !entity->is_alive()
		|| entity == csgo::local_player
		|| entity->team() == csgo::local_player->team()
		|| entity->health() <= 0
		|| entity->dormant())
		return;

	visuals::box bbox;
	if (!visuals::get_playerbox(entity, bbox))
		return;

	visuals::boxes(entity, bbox);
	visuals::name(entity, bbox);
	visuals::health(entity, bbox);
	visuals::ammo(entity, bbox);
	visuals::weapon(entity, bbox);
	visuals::flags(entity, bbox);
}


void visuals::run()
{
	for (int i = 1; i <= interfaces::globals->max_clients; ++i) {
		player_t* entity = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(i));
		run_player(entity);	
	}
}