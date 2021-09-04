#include "menu.hpp"
#include "config/config.hpp"
#include "../features/features.hpp"

c_menu menu;

#define UNLEN 256
IDirect3DStateBlock9* state_block;
bool save = false;
bool load = false;

void c_menu::run() {
	static int tab = 1;
	static int weapon = 0;

	if (opened) {
		ImGui::SetNextWindowSize(ImVec2(500, 500));
		ImGui::Begin("Menu", &opened, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_ShowBorders | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar); {
			ImVec2 pos = ImGui::GetCursorScreenPos();
			ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(pos.x, pos.y + 25), ImVec2(pos.x + ImGui::GetWindowWidth(), pos.y - 3), ImColor(20, 20, 20));
			ImGui::GetWindowDrawList()->AddRectFilledMultiColor(ImVec2(pos.x, pos.y + 27), ImVec2(pos.x + ImGui::GetWindowWidth(), pos.y + 25), ImColor( 200 / 255.f, 150 / 255.f, 255 / 255.f , 1.f), ImColor( 200 / 255.f, 150 / 255.f, 255 / 255.f , 1.f), ImColor( 200 / 255.f, 150 / 255.f, 255 / 255.f , 1.f), ImColor( 200 / 255.f, 150 / 255.f, 255 / 255.f , 1.f));
			ImGui::PushFont(font_menu);
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 5);
			if (ImGui::ButtonT("Aimbot", ImVec2(46, 25), tab, 1, false, false)) tab = 1; ImGui::SameLine(0, 0);
			if (ImGui::ButtonT("Visuals", ImVec2(45, 25), tab, 2, false, false)) tab = 2; ImGui::SameLine(0, 0);
			if (ImGui::ButtonT("Misc", ImVec2(32, 25), tab, 3, false, false)) tab = 3; ImGui::SameLine(0, 0);
			if (ImGui::ButtonT("Config", ImVec2(44, 25), tab, 4, false, false)) tab = 4; ImGui::SameLine(0, 0);
			ImGui::SameLine();
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 1);
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 250);
			ImGui::PopFont();
			ImGui::PushFont(font_title);
			ImGui::Text("tuamae");
			ImGui::PopFont();
			ImGui::SameLine();
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 1);
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 5);
			ImGui::GetStyle().Colors[ImGuiCol_Text] = ImVec4(200 / 255.f, 150 / 255.f, 255 / 255.f, 1.f);
			ImGui::Text(".tk");
			ImGui::GetStyle().Colors[ImGuiCol_Text] = ImVec4(1, 1, 1, 1);
			ImGui::PushFont(font_menu);
			if (tab == 1) {
				ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5);
				ImGui::Columns(2, NULL, false);
				ImGui::Dummy(ImVec2(0, -2)); ImGui::SameLine();
				ImGui::Dummy(ImVec2(0, 0)); ImGui::SameLine();
				ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(10, 15));
				ImGui::BeginChild("Aimbot", ImVec2(230, 218), true); {
					ImGui::Checkbox("Aimbot", &cfg.aimbot.aimbot);
					ImGui::Combo("Weapon", &weapon, "Pistol\0Rifle\0Sniper\0SMG\0Heavy");
					ImGui::Checkbox("Silent", &cfg.aimbot.silent);
					ImGui::Checkbox("Backtrack", &cfg.aimbot.backtrack);
				} ImGui::EndChild(true);
				ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 15);
				ImGui::BeginChild("Trigger", ImVec2(230, 218), true); {

				} ImGui::EndChild(true);
				ImGui::NextColumn();
				ImGui::BeginChild("Settings", ImVec2(230, 445), true); {
					switch (weapon) {
					case 0:
						ImGui::SliderInt("Fov", &cfg.aimbot.fovpistol, 0, 180);
						ImGui::SliderInt("Smooth", &cfg.aimbot.smoothpistol, 1, 50);
						ImGui::SliderInt("RCS", &cfg.aimbot.rcspistol, 0, 100);
						ImGui::Combo("Hitbox", &cfg.aimbot.bonepistol, "Head\0Neck\0Chest\0Stomach\0Pelvis\0Nearest");
						break;
					case 1:
						ImGui::SliderInt("Fov", &cfg.aimbot.fovrifle, 0, 180);
						ImGui::SliderInt("Smooth", &cfg.aimbot.smoothrifle, 1, 50);
						ImGui::SliderInt("RCS", &cfg.aimbot.rcsrifle, 0, 100);
						ImGui::Combo("Hitbox", &cfg.aimbot.bonerifle, "Head\0Neck\0Chest\0Stomach\0Pelvis\0Nearest");
						break;
					case 2:
						ImGui::SliderInt("Fov", &cfg.aimbot.fovsniper, 0, 180);
						ImGui::SliderInt("Smooth", &cfg.aimbot.smoothsniper, 1, 50);
						ImGui::SliderInt("RCS", &cfg.aimbot.rcssniper, 0, 100);
						ImGui::Combo("Hitbox", &cfg.aimbot.bonesniper, "Head\0Neck\0Chest\0Stomach\0Pelvis\0Nearest");
						break;
					case 3:
						ImGui::SliderInt("Fov", &cfg.aimbot.fovsmg, 0, 180);
						ImGui::SliderInt("Smooth", &cfg.aimbot.smoothsmg, 1, 50);
						ImGui::SliderInt("RCS", &cfg.aimbot.rcssmg, 0, 100);
						ImGui::Combo("Hitbox", &cfg.aimbot.bonesmg, "Head\0Neck\0Chest\0Stomach\0Pelvis\0Nearest");
						break;
					case 4:
						ImGui::SliderInt("Fov", &cfg.aimbot.fovheavy, 0, 180);
						ImGui::SliderInt("Smooth", &cfg.aimbot.smoothheavy, 1, 50);
						ImGui::SliderInt("RCS", &cfg.aimbot.rcsheavy, 0, 100);
						ImGui::Combo("Hitbox", &cfg.aimbot.boneheavy, "Head\0Neck\0Chest\0Stomach\0Pelvis\0Nearest");
						break;
					}
				} ImGui::EndChild(true);
				ImGui::PopStyleVar();
				ImGui::Columns();
			}
			if (tab == 2) {
				ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5);
				ImGui::Columns(2, NULL, false);
				ImGui::Dummy(ImVec2(0, -2)); ImGui::SameLine();
				ImGui::Dummy(ImVec2(0, 0)); ImGui::SameLine();
				ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(10, 15));
				ImGui::BeginChild("ESP", ImVec2(230, 218), true); {
					ImGui::Checkbox("Box", &cfg.visuals.box);
					ImGui::SameLine();
					ImGui::ColorEdit4("##boxcolor", cfg.visuals.boxcol, ImGuiColorEditFlags_NoInputs);
					ImGui::Combo("Box type", &cfg.visuals.boxtype, "Normal\0Corner");
					ImGui::Checkbox("Name", &cfg.visuals.name);
					ImGui::SameLine();
					ImGui::ColorEdit4("##namecol", cfg.visuals.namecol, ImGuiColorEditFlags_NoInputs);
					ImGui::Checkbox("Health text", &cfg.visuals.healthtext);
					ImGui::Checkbox("Health bar", &cfg.visuals.healthbar);
					ImGui::Checkbox("Override color", &cfg.visuals.healthoverride);
					ImGui::SameLine();
					ImGui::ColorEdit4("##healthcol", cfg.visuals.healthcol, ImGuiColorEditFlags_NoInputs);
					ImGui::Checkbox("Ammo", &cfg.visuals.ammo);
					ImGui::SameLine();
					ImGui::ColorEdit4("##ammocol", cfg.visuals.ammocol, ImGuiColorEditFlags_NoInputs);
					ImGui::Checkbox("Weapon", &cfg.visuals.weapon);
					ImGui::SameLine();
					ImGui::ColorEdit4("##weaponcol", cfg.visuals.weaponcol, ImGuiColorEditFlags_NoInputs);
					if (ImGui::BeginCombo("Flags", "...", ImVec2(0, 100))) {
						ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 8);
						ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 4);
						ImGui::Selectable(("Money"), &cfg.visuals.money, ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups);
						ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 4);
						ImGui::Selectable(("Bot"), &cfg.visuals.bot, ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups);
						ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 4);
						ImGui::Selectable(("Armor"), &cfg.visuals.armor, ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups);
						ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 4);
						ImGui::Selectable(("Scoped"), &cfg.visuals.scoped, ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups);
						ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 4);
						ImGui::Selectable(("Flashed"), &cfg.visuals.flashed, ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups);
						ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 4);
						ImGui::Selectable(("Defusing"), &cfg.visuals.defusing, ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups);
						ImGui::EndCombo();
					}
				} ImGui::EndChild(true);
				ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 15);
				ImGui::BeginChild("World", ImVec2(230, 218), true); {
					ImGui::Checkbox("Fog override", &cfg.visuals.fogoverride);
					ImGui::SameLine();
					ImGui::ColorEdit4("##fogcolor", cfg.visuals.fogcolor, ImGuiColorEditFlags_NoInputs);
					ImGui::SliderInt("Fog min", &cfg.visuals.fogmin, 0, 2500);
					ImGui::SliderInt("Fog max", &cfg.visuals.fogmax, 0, 2500);
				} ImGui::EndChild(true);
				ImGui::NextColumn();
				ImGui::BeginChild("Chams", ImVec2(230, 218), true); {

				} ImGui::EndChild(true);
				ImGui::BeginChild("Glow", ImVec2(230, 218), true); {
					ImGui::Checkbox("Enemy", &cfg.visuals.glow);
					ImGui::SameLine();
					ImGui::ColorEdit4("##glowcol", cfg.visuals.glowcol, ImGuiColorEditFlags_NoInputs);
				} ImGui::EndChild(true);
				ImGui::PopStyleVar();
				ImGui::Columns();
			}
			if (tab == 3) {
				ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5);
				ImGui::Columns(2, NULL, false);
				ImGui::Dummy(ImVec2(0, -2)); ImGui::SameLine();
				ImGui::Dummy(ImVec2(0, 0)); ImGui::SameLine();
				ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(10, 15));
				ImGui::BeginChild("Movement", ImVec2(230, 218), true); {
					ImGui::Checkbox("Bunnyhop", &cfg.misc.bunnyhop);
					ImGui::Checkbox("Jumpbug", &cfg.misc.jumpbug);
					ImGui::SameLine(); ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 45);
					ImGui::Hotkey("##jumpbugkey", &cfg.misc.jumpbugkey, ImVec2(100, 15));
					ImGui::Checkbox("Edgebug", &cfg.misc.edgebug);
					ImGui::SameLine(); ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 48);
					ImGui::Hotkey("##edgebugkey", &cfg.misc.edgebugkey, ImVec2(100, 15));
					if (cfg.misc.edgebugtype == 2) {
						ImGui::SliderInt("Edgebug radius", &cfg.misc.edgebugradius, 0, 32);
					}
					ImGui::Combo("Edgebug type", &cfg.misc.edgebugtype, "Prediction\0Trace-ray\0Smart");
					ImGui::Checkbox("Edgejump", &cfg.misc.edgejump);
					ImGui::SameLine(); ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 40);
					ImGui::Hotkey("##edgejumpkey", &cfg.misc.edgejumpkey, ImVec2(100, 15));
					ImGui::Checkbox("Longjump", &cfg.misc.longjump);
					ImGui::SameLine(); ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 40);
					ImGui::Hotkey("##longjumpkey", &cfg.misc.longjumpkey, ImVec2(100, 15));
					ImGui::Checkbox("Longjump on edge", &cfg.misc.longjumpej);
					if (ImGui::BeginCombo("Release keys", "...", ImVec2(0, 45))) {
						ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 8);
						ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 4);
						ImGui::Selectable(("Edge-jump"), &cfg.misc.edgejumpkeyrelease, ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups);
						ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 4);
						ImGui::Selectable(("Long-jump"), &cfg.misc.longjumpkeyrelease, ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups);
						ImGui::EndCombo();
					}
					ImGui::Checkbox("Minijump", &cfg.misc.minijump);
					ImGui::SameLine(); ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 44);
					ImGui::Hotkey("##minijumpkey", &cfg.misc.minijumpkey, ImVec2(100, 15));
				} ImGui::EndChild(true);
				ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 15);
				ImGui::BeginChild("Others", ImVec2(230, 218), true); {
					ImGui::Checkbox("Auto accept", &cfg.misc.autoaccept);
					ImGui::Checkbox("Spectator list", &cfg.misc.speclist);
					ImGui::Checkbox("Rank reveal", &cfg.misc.rankreveal);
					ImGui::Combo("Hitsound", &cfg.misc.hitsound, "None\0Switch press\0Cash");
				} ImGui::EndChild(true);
				ImGui::NextColumn();
				ImGui::BeginChild("Indicators", ImVec2(230, 445), true); {
					ImGui::Checkbox("Velocity indicator", &cfg.misc.velind);
					ImGui::Checkbox("Take off", &cfg.misc.takeoff);
					ImGui::SliderInt("Velocity offset", &cfg.misc.velindoffset, -250, +250);
					ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 18); ImGui::Text(">"); ImGui::SameLine();
					ImGui::ColorEdit4("##good", cfg.misc.good, ImGuiColorEditFlags_NoInputs);
					ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 18); ImGui::Text("="); ImGui::SameLine();
					ImGui::ColorEdit4("##normal", cfg.misc.normal, ImGuiColorEditFlags_NoInputs);
					ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 18); ImGui::Text("<"); ImGui::SameLine();
					ImGui::ColorEdit4("##bad", cfg.misc.bad, ImGuiColorEditFlags_NoInputs);  ImGui::Spacing();
					ImGui::Checkbox("Stamina indicator", &cfg.misc.stamind);
					ImGui::SameLine();
					ImGui::ColorEdit4("##stamcol", cfg.misc.stamcol, ImGuiColorEditFlags_NoInputs);
					ImGui::SliderInt("Stamina offset", &cfg.misc.stamindoffset, -250, +250);
					ImGui::Checkbox("Velocity Graph", &cfg.misc.velgraph);
					ImGui::SameLine();
					ImGui::ColorEdit4("##velgraphcol", cfg.misc.velgraphcol, ImGuiColorEditFlags_NoInputs);
					ImGui::Checkbox("Stamina Graph", &cfg.misc.stamgraph);
					ImGui::SameLine();
					ImGui::ColorEdit4("##stamgraphcol", cfg.misc.stamgraphcol, ImGuiColorEditFlags_NoInputs);
					ImGui::SliderInt("Graph offset", &cfg.misc.graphoffset, -250, +250);
					ImGui::Checkbox("Edgebug sound", &cfg.misc.ebsound);
				} ImGui::EndChild(true);
				ImGui::PopStyleVar();
				ImGui::Columns();
			}
			if (tab == 4) {
				ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5);
				ImGui::Columns(2, NULL, false);
				ImGui::Dummy(ImVec2(0, -2)); ImGui::SameLine();
				ImGui::Dummy(ImVec2(0, 0)); ImGui::SameLine();
				ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(10, 15));
				ImGui::BeginChild("Config", ImVec2(230, 258), true); {
					constexpr auto& config_items = cfg.get_configs();
					static int current_config = -1;

					if (static_cast<size_t>(current_config) >= config_items.size())
						current_config = -1;

					static char buffer[16];
					if (ImGui::Combo("", &current_config, [](void* data, int idx, const char** out_text) {
						auto& vector = *static_cast<std::vector<std::string>*>(data);
						*out_text = vector[idx].c_str();
						return true;
						}, &config_items, config_items.size(), 5) && current_config != -1)
						strcpy(buffer, config_items[current_config].c_str());

						ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 18);
						ImGui::PushID(0);
						ImGui::PushItemWidth(175);
						if (ImGui::InputText("", buffer, IM_ARRAYSIZE(buffer), ImGuiInputTextFlags_EnterReturnsTrue)) {
							if (current_config != -1)
								cfg.rename(current_config, buffer);
						}
						ImGui::PopID();
						ImGui::NextColumn();

						ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 18);
						if (ImGui::Button(("Add"), ImVec2(82.7, 20))) {
							cfg.add(buffer);
						}

						ImGui::SameLine();

						if (ImGui::Button(("Delete"), ImVec2(82.7, 20))) {
							cfg.remove(current_config);
						}

						ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 18);
						if (current_config != -1) {
							if (ImGui::Button(("Load"), ImVec2(82.7, 20))) {
								cfg.load(current_config);
								utilities::force_update();
							}

							ImGui::SameLine();

							if (ImGui::Button(("Save"), ImVec2(82.7, 20))) {
								cfg.save(current_config);
							}
						}
				} ImGui::EndChild(true);
				ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 15);
				ImGui::BeginChild("Others", ImVec2(230, 178), true); {
					ImGui::Checkbox("Watermark", &cfg.config.watermark);
				} ImGui::EndChild(true);
				ImGui::NextColumn();
				ImGui::BeginChild("Skins", ImVec2(230, 218), true); {
					ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 15);
					if (ImGui::Button(("Update"), ImVec2(180, 20))) {
						utilities::force_update();
					}
					ImGui::Combo("Knife model", &cfg.config.knife_model, "-\0Bayonet\0M9\0Karambit\0Bowie\0Butterfly\0Falchion\0Flip\0Gut\0Huntsman\0Shaddow-Daggers\0Navaja\0Stiletto\0Talon\0Ursus\0Canis\0Outdoor\0Cord\0Skeleton\0CSS-Classic\0Terrorist\0Counter-Terrorist");
					ImGui::Combo(("Knife skin"), &cfg.config.knife_skin, [](void* data, int idx, const char** out_text) {
						*out_text = parser_skins[idx].name.c_str();
						return true;
						}, nullptr, parser_skins.size(), 10);
					cfg.config.knife_skin_id = parser_skins[cfg.config.knife_skin].id;
					ImGui::Combo("Knife wear", &cfg.config.knife_wear, "Factory-new(FN)\0Minimal-wear(MW)\0Field-tested(FT)\0Wll-worn(WW)\0Battle-scarred(BS)");
					ImGui::Spacing();
					ImGui::Combo("Gloves model", &cfg.config.gloves_model, "-\0Blood\0Sport\0Slick\0Leather\0Moto\0Speci\0Hydra");
					ImGui::Combo(("Gloves skin"), &cfg.config.gloves_skin, [](void* data, int idx, const char** out_text) {
						*out_text = parser_gloves[idx].name.c_str();
						return true;
						}, nullptr, parser_gloves.size(), 10);
					cfg.config.gloves_skin_id = parser_gloves[cfg.config.gloves_skin].id;
					ImGui::Combo("Gloves wear", &cfg.config.gloves_wear, "Factory-new(FN)\0Minimal-wear(MW)\0Field-tested(FT)\0Wll-worn(WW)\0Battle-scarred(BS)");
					ImGui::Spacing();
					ImGui::Combo("Agent model", &cfg.config.agent_model, "-\0Special Agent Ava | FBI\0Operator | FBI SWAT\0Markus Delrow | FBI HRT\0Michael Syfers | FBI Sniper\0B Squadron Officer | SAS\0Seal Team 6 Soldier | NSWC SEAL\0Buckshot | NSWC SEAL\0Lt. Commander Ricksaw | NSWC SEAL\0Third Commando Company | KSK\0'Two Times' McCoy | USAF TACP\0Dragomir | Sabre\0Rezan The Ready | Sabre\0'The Doctor' Romanov | Sabre\0Maximus | Sabre\0Blackwolf | Sabre\0The Elite Mr. Muhlik | Elite Crew\0Ground Rebel | Elite Crew\0Osiris | Elite Crew\0Prof. Shahmat | Elite Crew\0Enforcer | Phoenix\0Slingshot | Phoenix\0Soldier | Phoenix\0Street Soldier | Phoenix\0'Blueberries' Buckshot | NSWC SEAL\0'Two Times' McCoy | TACP Cavalry\0Rezan the Redshirt | Sabre\0Dragomir | Sabre Footsoldier\0Cmdr. Mae 'Dead Cold' Jamison | SWAT\0 1st Lieutenant Farlow | SWAT\0John 'Van Healen' Kask | SWAT\0Bio-Haz Specialist | SWAT\0Sergeant Bombson | SWAT\0Chem-Haz Specialist | SWAT\0Sir Bloody Miami Darryl | The Professionals\0Sir Bloody Silent Darryl | The Professionals\0Sir Bloody Skullhead Darryl | The Professionals\0Sir Bloody Darryl Royale | The Professionals\0Sir Bloody Loudmouth Darryl | The Professionals\0Safecracker Voltzmann | The Professionals\0Little Kev | The Professionals\0Number K | The Professionals\0Getaway Sally | The Professionals");
					ImGui::Spacing();
				} ImGui::EndChild(true);
				ImGui::BeginChild("Fonts", ImVec2(230, 218), true); {

				} ImGui::EndChild(true);
				ImGui::PopStyleVar();
				ImGui::Columns();
			}
		}
		ImGui::PopFont();
		ImGui::End();
	}
}