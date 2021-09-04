#pragma once
#include "../../../dependencies/utilities/csgo.hpp"

namespace prediction {
	void run(c_usercmd* cmd, const std::function< void() >& fn);

	inline bool in_prediction = false;
	inline int flags;
	inline vec3_t velocity;
	inline vec3_t stamina;

	inline player_move_data data;
	inline float old_cur_time;
	inline float old_frame_time;
	inline float old_tick_count;
	inline int* prediction_random_seed;
	inline int* prediction_player;
};
