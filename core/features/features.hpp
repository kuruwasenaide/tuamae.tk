#pragma once
#include "../../dependencies/utilities/csgo.hpp"
#include "../menu/config/config.hpp"

namespace aimbot {
	bool aimbot(c_usercmd* cmd);
	void regtick(c_usercmd* cmd);
	void begin(c_usercmd* cmd);
	void end();
}

namespace visuals {
	struct box {
		int x, y, w, h;
		box() = default;
		box(int x, int y, int w, int h) {
			this->x = x;
			this->y = y;
			this->w = w;
			this->h = h;
		}
	};

	void run();
	bool get_playerbox(player_t* ent, box& in);
	void boxes(player_t* entity, visuals::box bbox);
	void name(player_t* entity, visuals::box bbox);
	void health(player_t* entity, visuals::box bbox);
	void ammo(player_t* entity, visuals::box bbox);
	void weapon(player_t* entity, visuals::box bbox);
	void flags(player_t* entity, visuals::box bbox);
	void glow();
	void fog();
}

namespace misc {
	void bunnyhop(c_usercmd* cmd);
	void jumpbug(c_usercmd* cmd, int unpredicted_flags);
	void edgebug(c_usercmd* cmd, int unpredicted_flags, int post_flags, float unpredicted_velocity);
	void edgejump(c_usercmd* cmd, int unpredicted_flags);
	void longjump(c_usercmd* cmd, int unpredicted_flags);
	void minijump(c_usercmd* cmd, int unpredicted_flags);
	void velind();
	void stamind();
	void velgraph();
	void stamgraph();
	void ebsound(float unpredicted_velocity);
	void autoaccept(const char* soundentry);
	void speclist();
	void rankreveal();
	void hitsound(i_game_event* event);
	void watermark();
}

namespace skins {
	void knife();
	void gloves();
	void agents(int frame_stage);
	void parser();
}

struct paint_kit {
	int id;
	std::string name;
	bool operator < (const paint_kit& other) {
		return name < other.name;
	}
};

class c_kit_parser {
public:
};

extern std::vector<paint_kit> parser_skins;
extern std::vector<paint_kit> parser_gloves;
extern c_kit_parser kit_parser;