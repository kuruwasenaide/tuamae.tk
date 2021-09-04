#pragma once

#include <filesystem>

class config final {
public:
	void run(const char*) noexcept;
	void load(size_t) noexcept;
	void save(size_t) const noexcept;
	void add(const char*) noexcept;
	void remove(size_t) noexcept;
	void rename(size_t, const char*) noexcept;
	void reset() noexcept;

	constexpr auto& get_configs() noexcept {
		return configs;
	}

	struct {
		bool aimbot = false;
		bool silent = false;
		bool backtrack = false;
		int fovpistol{ 0 };
		int smoothpistol{ 1 };
		int rcspistol{ 0 };
		int fovrifle{ 0 };
		int smoothrifle{ 1 };
		int rcsrifle{ 0 };
		int fovsniper{ 0 };
		int smoothsniper{ 1 };
		int rcssniper{ 0 };
		int fovheavy{ 0 };
		int smoothheavy{ 1 };
		int rcsheavy{ 0 };
		int fovsmg{ 0 };
		int smoothsmg{ 1 };
		int rcssmg{ 0 };
		int bonesmg{ 0 };
		int bonesniper{ 0 };
		int bonepistol{ 0 };
		int bonerifle{ 0 };
		int boneheavy{ 0 };
	} aimbot;

	struct {
		bool box = false;
		float boxcol[4]{ 1.0f,1.0f,1.0f, 1.f };
		int boxtype = 0;
		bool name = false;
		float namecol[4]{ 1.0f,1.0f,1.0f, 1.f };
		bool healthtext = false;
		bool healthbar = false;
		bool healthoverride = false;
		float healthcol[4]{ 1.0f,1.0f,1.0f, 1.f };
		bool ammo = false;
		float ammocol[4]{ 0.5f,0.5f,1.0f, 1.f };
		bool weapon = false;
		float weaponcol[4]{ 1.0f,1.0f,1.0f, 1.f };
		bool money = false;
		bool armor = false;
		bool bot = false;
		bool scoped = false;
		bool flashed = false;
		bool defusing = false;
		bool reloading = false;
		bool glow = false;
		float glowcol[4]{ 1.0f,1.0f,1.0f, 1.f };
		bool fogoverride = false;
		int fogmin = 0;
		int fogmax = 0;
		float fogdensity = 1;
		float fogcolor[4]{ 1.0f,1.0f,1.0f, 1.f };
	} visuals;

	struct {
		bool bunnyhop = false;
		bool jumpbug = false;
		int jumpbugkey = 0;
		bool edgebug = false;
		int edgebugkey = 0;
		int edgebugradius = 0;
		int edgebugtype = 0;
		bool edgejump = false;
		int edgejumpkey = 0;
		bool edgejumpkeyrelease = false;
		bool longjump = false;
		int longjumpkey = 0;
		bool longjumpej = false;
		bool longjumpkeyrelease = false;
		bool minijump = false;
		int minijumpkey = 0;
		bool velind = false;
		float good[4]{ 25 / 255.f, 255 / 255.f, 125 / 255.f, 1.f };
		float normal[4]{ 255 / 255.f, 200 / 255.f, 100 / 255.f, 1.f };
		float bad[4]{ 225 / 255.f, 100 / 255.f, 100 / 255.f, 1.f };
		bool takeoff = false;
		int velindoffset = 0;
		bool stamind = false;
		float stamcol[4]{ 200 / 255.f, 200 / 255.f, 200 / 255.f, 1.f };
		int stamindoffset = 0;
		bool velgraph = false;
		float velgraphcol[4]{ 255 / 255.f, 255 / 255.f, 255 / 255.f, 1.f };
		bool stamgraph = false;
		float stamgraphcol[4]{ 255 / 255.f, 150 / 255.f, 255 / 255.f, 1.f };
		int graphoffset = 0;
		bool ebsound = false;
		bool autoaccept = false;
		bool speclist = false;
		bool rankreveal = false;
		int hitsound = 0;
	} misc;

	struct {
		float color[4]{ 200 / 255.f, 150 / 255.f, 255 / 255.f , 1.f};
		bool watermark = true;
		int knife_model = 0;
		int knife_skin = 0;
		int knife_skin_id = 0;
		int knife_wear = 0;
		int gloves_model = 0;
		int gloves_skin = 0;
		int gloves_skin_id = 0;
		int gloves_wear = 0;
		int agent_model = 0;
	} config;

private:
	std::filesystem::path path;
	std::vector<std::string> configs;
};

extern config cfg;