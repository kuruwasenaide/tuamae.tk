#include "../features.hpp"
#define INVALID_EHANDLE_INDEX 0xFFFFFFFF

bool knife_model(attributable_item_t* weapon, const char* model) noexcept {
	auto local_player = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player()));
	if (!local_player)
		return false;

	auto viewmodel = reinterpret_cast<base_view_model_t*>(interfaces::entity_list->get_client_entity_handle(local_player->view_model()));
	if (!viewmodel)
		return false;

	auto h_view_model_weapon = viewmodel->weapon();
	if (!h_view_model_weapon)
		return false;

	auto view_model_weapon = reinterpret_cast<attributable_item_t*>(interfaces::entity_list->get_client_entity_handle(h_view_model_weapon));
	if (view_model_weapon != weapon)
		return false;

	viewmodel->model_index() = interfaces::model_info->get_model_index(model);

	return true;
}

bool knife_skin_id(attributable_item_t* weapon, int item_definition_index, int paint_kit, int model_index, int entity_quality, float fallback_wear) noexcept {
	weapon->item_definition_index() = item_definition_index;
	weapon->fallback_paint_kit() = paint_kit;
	weapon->model_index() = model_index;
	weapon->entity_quality() = entity_quality;
	weapon->fallback_wear() = fallback_wear;

	return true;
}

static auto get_wearable_create_fn() -> create_client_class_fn {
	auto client_class = interfaces::client->get_client_classes();

	for (client_class = interfaces::client->get_client_classes();
		client_class; client_class = client_class->next_ptr) {

		if (client_class->class_id == (int)class_ids::ceconwearable) {
			return client_class->create_fn;
		}
	}
}

static auto make_glove(int entry, int serial) -> attributable_item_t* {
	static auto create_wearable_fn = get_wearable_create_fn();
	create_wearable_fn(entry, serial);

	const auto glove = static_cast<attributable_item_t*>(interfaces::entity_list->get_client_entity(entry));
	assert(glove); {
		static auto set_abs_origin_addr = utilities::pattern_scan("client.dll", "55 8B EC 83 E4 F8 51 53 56 57 8B F1 E8");
		const auto set_abs_origin_fn = reinterpret_cast<void(__thiscall*)(void*, const std::array<float, 3>&)>(set_abs_origin_addr);
		static constexpr std::array<float, 3> new_pos = { 10000.f, 10000.f, 10000.f };
		set_abs_origin_fn(glove, new_pos);
	}
	return glove;
}

bool apply_glove_model(attributable_item_t* glove, const char* model) noexcept {
	player_info_t info;
	interfaces::engine->get_player_info(interfaces::engine->get_local_player(), &info);
	glove->acc_id() = info.xuidlow;
	*reinterpret_cast<int*>(uintptr_t(glove) + 0x64) = -1;

	return true;
}

bool apply_glove_skin(attributable_item_t* glove, int item_definition_index, int paint_kit, int model_index, int entity_quality, float fallback_wear) noexcept {
	glove->item_definition_index() = item_definition_index;
	glove->fallback_paint_kit() = paint_kit;
	glove->set_model_index(model_index);
	glove->entity_quality() = entity_quality;
	glove->fallback_wear() = fallback_wear;

	return true;
}

bool apply_agent_model(attributable_item_t* agent,int model_index) noexcept {
	agent->set_model_index(model_index);

	return true;
}

inline const char* agent_model[] =
{
		"",
		"models/player/custom_player/legacy/ctm_fbi_variantb.mdl",
		"models/player/custom_player/legacy/ctm_fbi_variantf.mdl",
		"models/player/custom_player/legacy/ctm_fbi_variantg.mdl",
		"models/player/custom_player/legacy/ctm_fbi_varianth.mdl",
		"models/player/custom_player/legacy/ctm_sas_variantf.mdl",
		"models/player/custom_player/legacy/ctm_st6_variante.mdl",
		"models/player/custom_player/legacy/ctm_st6_variantg.mdl",
		"models/player/custom_player/legacy/ctm_st6_varianti.mdl",
		"models/player/custom_player/legacy/ctm_st6_variantk.mdl",
		"models/player/custom_player/legacy/ctm_st6_variantm.mdl",
		"models/player/custom_player/legacy/tm_balkan_variantf.mdl",
		"models/player/custom_player/legacy/tm_balkan_variantg.mdl",
		"models/player/custom_player/legacy/tm_balkan_varianth.mdl",
		"models/player/custom_player/legacy/tm_balkan_varianti.mdl",
		"models/player/custom_player/legacy/tm_balkan_variantj.mdl",
		"models/player/custom_player/legacy/tm_leet_variantf.mdl",
		"models/player/custom_player/legacy/tm_leet_variantg.mdl",
		"models/player/custom_player/legacy/tm_leet_varianth.mdl",
		"models/player/custom_player/legacy/tm_leet_varianti.mdl",
		"models/player/custom_player/legacy/tm_phoenix_variantf.mdl",
		"models/player/custom_player/legacy/tm_phoenix_variantg.mdl",
		"models/player/custom_player/legacy/tm_phoenix_varianth.mdl",
		"models/player/custom_player/legacy/tm_phoenix_varianti.mdl",
		"models/player/custom_player/legacy/ctm_st6_variantj.mdl",
		"models/player/custom_player/legacy/ctm_st6_variantl.mdl",
		"models/player/custom_player/legacy/tm_balkan_variantk.mdl",
		"models/player/custom_player/legacy/tm_balkan_variantl.mdl",
		"models/player/custom_player/legacy/ctm_swat_variante.mdl",
		"models/player/custom_player/legacy/ctm_swat_variantf.mdl",
		"models/player/custom_player/legacy/ctm_swat_variantg.mdl",
		"models/player/custom_player/legacy/ctm_swat_varianth.mdl",
		"models/player/custom_player/legacy/ctm_swat_varianti.mdl",
		"models/player/custom_player/legacy/ctm_swat_variantj.mdl",
		"models/player/custom_player/legacy/tm_professional_varf.mdl",
		"models/player/custom_player/legacy/tm_professional_varf1.mdl",
		"models/player/custom_player/legacy/tm_professional_varf2.mdl",
		"models/player/custom_player/legacy/tm_professional_varf3.mdl",
		"models/player/custom_player/legacy/tm_professional_varf4.mdl",
		"models/player/custom_player/legacy/tm_professional_varg.mdl",
		"models/player/custom_player/legacy/tm_professional_varh.mdl",
		"models/player/custom_player/legacy/tm_professional_vari.mdl",
		"models/player/custom_player/legacy/tm_professional_varj.mdl"
};

void skins::knife() {
	if (!interfaces::engine->is_connected() || !interfaces::engine->is_in_game())
		return;

	auto local_player = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player()));
	if (!local_player)
		return;

	if (cfg.config.knife_model == 0)
		return;

	if (!local_player->is_alive())
		return;

	auto viewmodel = reinterpret_cast<base_view_model_t*>(interfaces::entity_list->get_client_entity_handle(local_player->view_model()));
	if (!viewmodel)
		return;

	auto h_view_model_weapon = viewmodel->weapon();
	if (!h_view_model_weapon)
		return;

	auto model_bayonet = "models/weapons/v_knife_bayonet.mdl";
	auto model_m9 = "models/weapons/v_knife_m9_bay.mdl";
	auto model_karambit = "models/weapons/v_knife_karam.mdl";
	auto model_bowie = "models/weapons/v_knife_survival_bowie.mdl";
	auto model_butterfly = "models/weapons/v_knife_butterfly.mdl";
	auto model_falchion = "models/weapons/v_knife_falchion_advanced.mdl";
	auto model_flip = "models/weapons/v_knife_flip.mdl";
	auto model_gut = "models/weapons/v_knife_gut.mdl";
	auto model_huntsman = "models/weapons/v_knife_tactical.mdl";
	auto model_daggers = "models/weapons/v_knife_push.mdl";
	auto model_navaja = "models/weapons/v_knife_gypsy_jackknife.mdl";
	auto model_stiletto = "models/weapons/v_knife_stiletto.mdl";
	auto model_talon = "models/weapons/v_knife_widowmaker.mdl";
	auto model_ursus = "models/weapons/v_knife_ursus.mdl";
	auto model_canis = "models/weapons/v_knife_canis.mdl";
	auto model_outdoor = "models/weapons/v_knife_outdoor.mdl";
	auto model_cord = "models/weapons/v_knife_cord.mdl";
	auto model_skeleton = "models/weapons/v_knife_skeleton.mdl";
	auto model_css = "models/weapons/v_knife_css.mdl";
	auto model_tr = "models/weapons/v_knife_default_t.mdl";
	auto model_ct = "models/weapons/v_knife_default_ct.mdl";

	auto index_bayonet = interfaces::model_info->get_model_index("models/weapons/v_knife_bayonet.mdl");
	auto index_m9 = interfaces::model_info->get_model_index("models/weapons/v_knife_m9_bay.mdl");
	auto index_karambit = interfaces::model_info->get_model_index("models/weapons/v_knife_karam.mdl");
	auto index_bowie = interfaces::model_info->get_model_index("models/weapons/v_knife_survival_bowie.mdl");
	auto index_butterfly = interfaces::model_info->get_model_index("models/weapons/v_knife_butterfly.mdl");
	auto index_falchion = interfaces::model_info->get_model_index("models/weapons/v_knife_falchion_advanced.mdl");
	auto index_flip = interfaces::model_info->get_model_index("models/weapons/v_knife_flip.mdl");
	auto index_gut = interfaces::model_info->get_model_index("models/weapons/v_knife_gut.mdl");
	auto index_huntsman = interfaces::model_info->get_model_index("models/weapons/v_knife_tactical.mdl");
	auto index_daggers = interfaces::model_info->get_model_index("models/weapons/v_knife_push.mdl");
	auto index_navaja = interfaces::model_info->get_model_index("models/weapons/v_knife_gypsy_jackknife.mdl");
	auto index_stiletto = interfaces::model_info->get_model_index("models/weapons/v_knife_stiletto.mdl");
	auto index_talon = interfaces::model_info->get_model_index("models/weapons/v_knife_widowmaker.mdl");
	auto index_ursus = interfaces::model_info->get_model_index("models/weapons/v_knife_ursus.mdl");
	auto index_canis = interfaces::model_info->get_model_index("models/weapons/v_knife_canis.mdl");
	auto index_outdoor = interfaces::model_info->get_model_index("models/weapons/v_knife_outdoor.mdl");
	auto index_cord = interfaces::model_info->get_model_index("models/weapons/v_knife_cord.mdl");
	auto index_skeleton = interfaces::model_info->get_model_index("models/weapons/v_knife_skeleton.mdl");
	auto index_css = interfaces::model_info->get_model_index("models/weapons/v_knife_css.mdl");
	auto index_tr = interfaces::model_info->get_model_index("models/weapons/v_knife_default_t.mdl");
	auto index_ct = interfaces::model_info->get_model_index("models/weapons/v_knife_default_ct.mdl");


	auto active_weapon = local_player->active_weapon();
	if (!active_weapon)
		return;

	auto my_weapons = local_player->get_weapons();
	for (size_t i = 0; my_weapons[i] != INVALID_EHANDLE_INDEX; i++) {
		auto weapon = reinterpret_cast<attributable_item_t*>(interfaces::entity_list->get_client_entity_handle(my_weapons[i]));

		if (!weapon)
			return;

		//knife conditions
		float wear = 0.f;
		switch (cfg.config.knife_wear) {
		case 0:
			wear = 0.0000001f;
			break;
		case 1:
			wear = 0.07f;
			break;
		case 2:
			wear = 0.15f;
			break;
		case 3:
			wear = 0.38f;
			break;
		case 4:
			wear = 0.45f;
			break;
		}

		//apply knife model
		if (active_weapon->client_class()->class_id == class_ids::cknife) {
			switch (cfg.config.knife_model) {
			case 0:
				break;
			case 1:
				knife_model(weapon, model_bayonet);
				break;
			case 2:
				knife_model(weapon, model_m9);
				break;
			case 3:
				knife_model(weapon, model_karambit);
				break;
			case 4:
				knife_model(weapon, model_bowie);
				break;
			case 5:
				knife_model(weapon, model_butterfly);
				break;
			case 6:
				knife_model(weapon, model_falchion);
				break;
			case 7:
				knife_model(weapon, model_flip);
				break;
			case 8:
				knife_model(weapon, model_gut);
				break;
			case 9:
				knife_model(weapon, model_huntsman);
				break;
			case 10:
				knife_model(weapon, model_daggers);
				break;
			case 11:
				knife_model(weapon, model_navaja);
				break;
			case 12:
				knife_model(weapon, model_stiletto);
				break;
			case 13:
				knife_model(weapon, model_talon);
				break;
			case 14:
				knife_model(weapon, model_ursus);
				break;
			case 15:
				knife_model(weapon, model_canis);
				break;
			case 16:
				knife_model(weapon, model_outdoor);
				break;
			case 17:
				knife_model(weapon, model_cord);
				break;
			case 18:
				knife_model(weapon, model_skeleton);
				break;
			case 19:
				knife_model(weapon, model_css);
				break;
			case 20:
				knife_model(weapon, model_tr);
				break;
			case 21:
				knife_model(weapon, model_ct);
				break;
			}
		}

		if (weapon->client_class()->class_id == class_ids::cknife) {
			switch (cfg.config.knife_model) {
			case 0:
				break;
			case 1:
				knife_skin_id(weapon, WEAPON_KNIFE_BAYONET, cfg.config.knife_skin_id, index_bayonet, 3, wear);
				break;
			case 2:
				knife_skin_id(weapon, WEAPON_KNIFE_M9_BAYONET, cfg.config.knife_skin_id, index_m9, 3, wear);
				break;
			case 3:
				knife_skin_id(weapon, WEAPON_KNIFE_KARAMBIT, cfg.config.knife_skin_id, index_karambit, 3, wear);
				break;
			case 4:
				knife_skin_id(weapon, WEAPON_KNIFE_SURVIVAL_BOWIE, cfg.config.knife_skin_id, index_bowie, 3, wear);
				break;
			case 5:
				knife_skin_id(weapon, WEAPON_KNIFE_BUTTERFLY, cfg.config.knife_skin_id, index_butterfly, 3, wear);
				break;
			case 6:
				knife_skin_id(weapon, WEAPON_KNIFE_FALCHION, cfg.config.knife_skin_id, index_falchion, 3, wear);
				break;
			case 7:
				knife_skin_id(weapon, WEAPON_KNIFE_FLIP, cfg.config.knife_skin_id, index_flip, 3, wear);
				break;
			case 8:
				knife_skin_id(weapon, WEAPON_KNIFE_GUT, cfg.config.knife_skin_id, index_gut, 3, wear);
				break;
			case 9:
				knife_skin_id(weapon, WEAPON_KNIFE_TACTICAL, cfg.config.knife_skin_id, index_huntsman, 3, wear);
				break;
			case 10:
				knife_skin_id(weapon, WEAPON_KNIFE_PUSH, cfg.config.knife_skin_id, index_daggers, 3, wear);
				break;
			case 11:
				knife_skin_id(weapon, WEAPON_KNIFE_GYPSY_JACKKNIFE, cfg.config.knife_skin_id, index_navaja, 3, wear);
				break;
			case 12:
				knife_skin_id(weapon, WEAPON_KNIFE_STILETTO, cfg.config.knife_skin_id, index_stiletto, 3, wear);
				break;
			case 13:
				knife_skin_id(weapon, WEAPON_KNIFE_WIDOWMAKER, cfg.config.knife_skin_id, index_talon, 3, wear);
				break;
			case 14:
				knife_skin_id(weapon, WEAPON_KNIFE_URSUS, cfg.config.knife_skin_id, index_ursus, 3, wear);
				break;
			case 15:
				knife_skin_id(weapon, WEAPON_KNIFE_CANIS, cfg.config.knife_skin_id, index_canis, 3, wear);
				break;
			case 16:
				knife_skin_id(weapon, WEAPON_KNIFE_OUTDOOR, cfg.config.knife_skin_id, index_outdoor, 3, wear);
				break;
			case 17:
				knife_skin_id(weapon, WEAPON_KNIFE_CORD, cfg.config.knife_skin_id, index_cord, 3, wear);
				break;
			case 18:
				knife_skin_id(weapon, WEAPON_KNIFE_SKELETON, cfg.config.knife_skin_id, index_skeleton, 3, wear);
				break;
			case 19:
				knife_skin_id(weapon, WEAPON_KNIFE_CSS, cfg.config.knife_skin_id, index_css, 3, wear);
			case 20:
				knife_skin_id(weapon, WEAPON_KNIFE_T, cfg.config.knife_skin_id, index_tr, 3, wear);
				break;
			case 21:
				knife_skin_id(weapon, WEAPON_KNIFE, cfg.config.knife_skin_id, index_ct, 3, wear);
			}
		}
		weapon->original_owner_xuid_low() = 0;
		weapon->original_owner_xuid_high() = 0;
		weapon->fallback_seed() = 661;
		weapon->item_id_high() = -1;
	}
}

void skins::gloves() {
	if (!interfaces::engine->is_connected() && !interfaces::engine->is_in_game())
		return;

	auto local_player = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player()));
	if (!local_player)
		return;

	if (cfg.config.gloves_model == 0)
		return;


	if (!local_player->is_alive())
		return;

	auto model_blood = "models/weapons/v_models/arms/glove_bloodhound/v_glove_bloodhound.mdl";
	auto model_sport = "models/weapons/v_models/arms/glove_sporty/v_glove_sporty.mdl";
	auto model_slick = "models/weapons/v_models/arms/glove_slick/v_glove_slick.mdl";
	auto model_leath = "models/weapons/v_models/arms/glove_handwrap_leathery/v_glove_handwrap_leathery.mdl";
	auto model_moto = "models/weapons/v_models/arms/glove_motorcycle/v_glove_motorcycle.mdl";
	auto model_speci = "models/weapons/v_models/arms/glove_specialist/v_glove_specialist.mdl";
	auto model_hydra = "models/weapons/v_models/arms/glove_bloodhound/v_glove_bloodhound_hydra.mdl";

	auto index_blood = interfaces::model_info->get_model_index(("models/weapons/v_models/arms/glove_bloodhound/v_glove_bloodhound.mdl"));
	auto index_sport = interfaces::model_info->get_model_index(("models/weapons/v_models/arms/glove_sporty/v_glove_sporty.mdl"));
	auto index_slick = interfaces::model_info->get_model_index(("models/weapons/v_models/arms/glove_slick/v_glove_slick.mdl"));
	auto index_leath = interfaces::model_info->get_model_index(("models/weapons/v_models/arms/glove_handwrap_leathery/v_glove_handwrap_leathery.mdl"));
	auto index_moto = interfaces::model_info->get_model_index(("models/weapons/v_models/arms/glove_motorcycle/v_glove_motorcycle.mdl"));
	auto index_speci = interfaces::model_info->get_model_index(("models/weapons/v_models/arms/glove_specialist/v_glove_specialist.mdl"));
	auto index_hydra = interfaces::model_info->get_model_index(("models/weapons/v_models/arms/glove_bloodhound/v_glove_bloodhound_hydra.mdl"));

	//credit to namazso for nskinz

	uintptr_t* const wearables = local_player->get_wearables();
	if (!wearables)
		return;

	static uintptr_t glove_handle = uintptr_t(0);

	auto glove = reinterpret_cast<attributable_item_t*>(interfaces::entity_list->get_client_entity_handle(wearables[0]));

	if (!glove) // There is no glove
	{
		const auto our_glove = reinterpret_cast<attributable_item_t*>(interfaces::entity_list->get_client_entity_handle(glove_handle));

		if (our_glove) // Try to get our last created glove
		{
			wearables[0] = glove_handle;
			glove = our_glove;
		}
	}
	if (!local_player || // We are dead but we have a glove, destroy it
		!local_player->is_alive() ||
		!interfaces::engine->is_connected() ||
		!interfaces::engine->is_in_game()
		) {
		if (glove) {
			glove->net_set_destroyed_on_recreate_entities();
			glove->net_release();
		}
		return;
	}
	if (!glove) // We don't have a glove, but we should
	{
		const auto entry = interfaces::entity_list->get_highest_index() + 1;
		const auto serial = rand() % 0x1000;
		glove = make_glove(entry, serial);   // He he
		wearables[0] = entry | serial << 16;
		glove_handle = wearables[0]; // Let's store it in case we somehow lose it.
	}
	if (glove)
	{
		//glove conditions
		float g_wear = 0.f;
		switch (cfg.config.gloves_wear) {
		case 0:
			g_wear = 0.0000001f;
			break;
		case 1:
			g_wear = 0.07f;
			break;
		case 2:
			g_wear = 0.15f;
			break;
		case 3:
			g_wear = 0.38f;
			break;
		case 4:
			g_wear = 0.45f;
			break;
		}

		//apply glove model
		switch (cfg.config.gloves_model) {
		case 0:
			break;
		case 1:
			apply_glove_model(glove, model_blood);
			break;
		case 2:
			apply_glove_model(glove, model_sport);
			break;
		case 3:
			apply_glove_model(glove, model_slick);
			break;
		case 4:
			apply_glove_model(glove, model_leath);
			break;
		case 5:
			apply_glove_model(glove, model_moto);
			break;
		case 6:
			apply_glove_model(glove, model_speci);
			break;
		case 7:
			apply_glove_model(glove, model_hydra);
			break;
		}

		//apply glove skins
		switch (cfg.config.gloves_model) {
		case 0:
			break;
		case 1:
			apply_glove_skin(glove, GLOVE_STUDDED_BLOODHOUND, cfg.config.gloves_skin_id, index_blood, 3, g_wear);
			break;
		case 2:
			apply_glove_skin(glove, GLOVE_SPORTY, cfg.config.gloves_skin_id, index_sport, 3, g_wear);
			break;
		case 3:
			apply_glove_skin(glove, GLOVE_SLICK, cfg.config.gloves_skin_id, index_slick, 3, g_wear);
			break;
		case 4:
			apply_glove_skin(glove, GLOVE_LEATHER_WRAP, cfg.config.gloves_skin_id, index_leath, 3, g_wear);
			break;
		case 5:
			apply_glove_skin(glove, GLOVE_MOTORCYCLE, cfg.config.gloves_skin_id, index_moto, 3, g_wear);
			break;
		case 6:
			apply_glove_skin(glove, GLOVE_SPECIALIST, cfg.config.gloves_skin_id, index_speci, 3, g_wear);
			break;
		case 7:
			apply_glove_skin(glove, GLOVE_HYDRA, cfg.config.gloves_skin_id, index_hydra, 3, g_wear);
			break;
		}

		glove->item_id_high() = -1;
		glove->fallback_seed() = 0;
		glove->fallback_stattrak() = -1;

		glove->net_pre_data_update(DATA_UPDATE_CREATED);
	}
}

void skins::agents(int frame_stage) {
	if (!interfaces::engine->is_connected() || !interfaces::engine->is_in_game())
		return;

	auto local_player = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player()));
	if (!local_player)
		return;

	if (cfg.config.agent_model == 0)
		return;

	if (frame_stage != FRAME_RENDER_START && frame_stage != FRAME_RENDER_END)
		return;

	static int original_index = 0;

	if (!local_player) {
		original_index = 0;
		return;
	}

	if (const auto model = agent_model[cfg.config.agent_model]) {
		const auto idx = frame_stage == FRAME_RENDER_END && original_index ? original_index : interfaces::model_info->get_model_index(model);

		local_player->set_model_index(idx);
	}
}