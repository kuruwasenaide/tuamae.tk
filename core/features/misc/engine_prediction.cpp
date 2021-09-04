#include "engine_prediction.hpp"

namespace prediction_util {
	void start(c_usercmd* cmd) {
		prediction::in_prediction = true;

		if (!csgo::local_player)
			return;

		prediction::flags = csgo::local_player->flags();
		prediction::velocity = csgo::local_player->velocity();
		prediction::stamina = csgo::local_player->stamina();

		if (!prediction::prediction_random_seed)
			prediction::prediction_random_seed = *reinterpret_cast<int**>(utilities::pattern_scan("client.dll", sig_prediction_random_seed) + 2);

		*prediction::prediction_random_seed = cmd->randomseed & 0x7FFFFFFF;

		prediction::old_cur_time = interfaces::globals->cur_time;
		prediction::old_frame_time = interfaces::globals->frame_time;
		prediction::old_tick_count = interfaces::globals->tick_count;

		interfaces::globals->cur_time = csgo::local_player->get_tick_base() * interfaces::globals->interval_per_tick;
		interfaces::globals->frame_time = interfaces::globals->interval_per_tick;
		interfaces::globals->tick_count = csgo::local_player->get_tick_base();

		interfaces::game_movement->start_track_prediction_errors(csgo::local_player);

		memset(&prediction::data, 0, sizeof(prediction::data));
		interfaces::move_helper->set_host(csgo::local_player);
		interfaces::prediction->setup_move(csgo::local_player, cmd, interfaces::move_helper, &prediction::data);
		interfaces::game_movement->process_movement(csgo::local_player, &prediction::data);
		interfaces::prediction->finish_move(csgo::local_player, cmd, &prediction::data);
	}

	void end(c_usercmd* cmd) {
		prediction::in_prediction = false;

		if (!csgo::local_player)
			return;

		interfaces::game_movement->finish_track_prediction_errors(csgo::local_player);
		interfaces::move_helper->set_host(nullptr);

		*prediction::prediction_random_seed = -1;

		interfaces::globals->cur_time = prediction::old_cur_time;
		interfaces::globals->frame_time = prediction::old_frame_time;
		interfaces::globals->tick_count = prediction::old_frame_time;
	}
}

void prediction::run(c_usercmd* cmd, const std::function< void() >& fn) {
	prediction_util::start(cmd);
	fn();
	prediction_util::end(cmd);
}