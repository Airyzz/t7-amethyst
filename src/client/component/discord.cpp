#include <std_include.hpp>
#include "loader/component_loader.hpp"

#include <discord_rpc.h>

#include "scheduler.hpp"
#include <game/game.hpp>
#include <game/utils.hpp>

#include <utils/hook.hpp>
#include <component/game_event.hpp>

namespace discord
{
	namespace
	{
		int64_t startTimestamp = 0;

		void on_init_game() {
			auto time = std::chrono::system_clock::now();
			auto since_epoch = time.time_since_epoch();
			auto seconds = std::chrono::duration_cast<std::chrono::seconds>(since_epoch);
			long now = seconds.count();
			startTimestamp = now;
		}

		void update_status() {
			DiscordRichPresence discord_presence{};
			ZeroMemory(&discord_presence, sizeof(discord_presence));

			discord_presence.instance = 1;

			int count = 0;
			game::foreach_connected_client([&count](const game::client_s&)
			{
				++count;
			});

			discord_presence.partySize = count;
			discord_presence.partyMax = game::get_dvar_int("com_maxclients");;
			discord_presence.startTimestamp = startTimestamp;

			auto mapname = game::get_dvar_string("mapname");

			auto mode = game::Com_SessionMode_GetMode();

			std::string state = "";
			std::string details = "";

			if (mapname == "core_frontend") {
				details = "In the main menu";

				if (mode == game::MODE_ZOMBIES) {
					state = "Zombies\x00";
				}
				else if (mode == game::MODE_MULTIPLAYER) {
					state = "Multiplayer\x00";
				}
			}
			else {
				auto map = game::Com_GameInfo_GetMapRefCaps(mapname.c_str());

				if (map == 0 || strlen(map) == 0) {
					map = mapname.c_str();
				}
				else {
					map = game::UI_SafeTranslateString(map);
				}

				if (mode == game::MODE_ZOMBIES) {
					if (*game::level_rounds_played > 0) {
						state = std::format("Round {}", *game::level_rounds_played);
					}
				}

				if (mode == game::MODE_MULTIPLAYER) {
					auto mode = game::Com_GameInfo_GetGameTypeRefCaps((const char*)0x01579DF880_g);

					if (mode != 0) {
						mode = game::UI_SafeTranslateString(mode);
					}
					state = std::format("{}", mode);
				}

				details = std::format("{}", map);
			}


			discord_presence.state = state.c_str();
			discord_presence.details = details.c_str();
		
			Discord_UpdatePresence(&discord_presence);
		}

		void ready(const DiscordUser* request)
		{
			SetEnvironmentVariableA("discord_user", request->userId);

			printf("Discord: Ready: %s - %s\n", request->userId, request->username);

			DiscordRichPresence discord_presence{};
			ZeroMemory(&discord_presence, sizeof(discord_presence));

			discord_presence.instance = 1;

			discord_presence.partySize = 0;
			discord_presence.partyMax = 0;
			discord_presence.startTimestamp = 0;

			Discord_UpdatePresence(&discord_presence);

			scheduler::loop(update_status, scheduler::pipeline::async, 10s);
		}

		void errored(const int error_code, const char* message)
		{
			printf("Discord: Error (%i): %s\n", error_code, message);
		}

	}

	class component final : public client_component
	{
	public:
		void post_load() override
		{
			DiscordEventHandlers handlers;
			ZeroMemory(&handlers, sizeof(handlers));
			handlers.ready = ready;
			handlers.errored = errored;
			handlers.disconnected = errored;
			handlers.joinGame = nullptr;
			handlers.spectateGame = nullptr;
			handlers.joinRequest = nullptr;

			Discord_Initialize("1180599439862272060", &handlers, 1, nullptr);

			this->initialized_ = true;

			game_event::on_g_init_game(on_init_game);

			scheduler::loop(Discord_RunCallbacks, scheduler::pipeline::async, 1s);
		}

		void pre_destroy() override
		{
			if (this->initialized_)
			{
				Discord_Shutdown();
			}
		}

	private:
		bool initialized_ = false;
	};
}


REGISTER_COMPONENT(discord::component)
