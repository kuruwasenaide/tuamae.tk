#include "../features.hpp"

void visuals::glow() {

	if (!cfg.visuals.glow)
		return;

	if (!interfaces::engine->is_connected() || !interfaces::engine->is_in_game())
		return;

	if (!csgo::local_player)
		return;

	for (int i = 0; i < interfaces::glow_manager->size; ++i)
	{
		if (interfaces::glow_manager->objects[i].unused() || !interfaces::glow_manager->objects[i].entity)
			continue;

		auto& glowEnt = interfaces::glow_manager->objects[i];
		auto pCSPlayer = reinterpret_cast<player_t*>(glowEnt.entity);
		auto clientclass = reinterpret_cast<c_client_class*>(pCSPlayer->client_class());

		if (!pCSPlayer)
			continue;

		if (pCSPlayer == csgo::local_player)
			continue;

		if (clientclass->class_id == ccsplayer)
		{
			if (pCSPlayer->dormant())
				continue;
			if (!(pCSPlayer->is_alive() && pCSPlayer->health() > 0))
				continue;

			if (pCSPlayer->team() == csgo::local_player->team()) continue;

			glowEnt.set(cfg.visuals.glowcol[0], cfg.visuals.glowcol[1], cfg.visuals.glowcol[2], cfg.visuals.glowcol[3]);
		}
	}
}