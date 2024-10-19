#include <std_include.hpp>
#include "loader/component_loader.hpp"
#include "chat.hpp"

#include "game/game.hpp"
#include "game/utils.hpp"

#include <utils/hook.hpp>
#include <utils/string.hpp>
#include "scheduler.hpp"

#include "command.hpp"
#include "client_command.hpp"
#include <utils/io.hpp>
#include <utils/hook.hpp>
#include <Windows.h>

namespace savegame
{
	struct SaveGameHeader {
		int size;
		int migration_version;
		int checksum;
		int network_protocol_version;
		int a4;
		int a5;
		int a6;
	};

	namespace
	{
		utils::hook::detour g_preload_game_hook;
		utils::hook::detour sv_init_game_vm_hook;
		utils::hook::detour livestats_read_xuid_from_stats_buffer_hook;

		void do_save() {
			auto mode = game::Com_SessionMode_GetMode();

			if (mode != game::eModes::MODE_MULTIPLAYER && mode != game::eModes::MODE_ZOMBIES) {
				return;
			}

			const int size = 1024 * 1024 * 10;
			auto buffer = (char*)malloc(size);

			if (buffer == nullptr) {
				printf("Failed to allocate space to save game\n");
				return;
			}

			memset(buffer, 0, size);

			auto file = utils::hook::invoke<game::MemoryFile*>(0x142154BC0_g);
			game::MemFile_CommonInit(file, size, buffer, true, true);
			game::G_SaveState(file, 1, 1);

			int length = file->bytesUsed;
			printf("Wrote %d bytes \n", length);

			std::ofstream stream("save_dump.bin", std::ios::binary | std::ofstream::out);

			if (stream.is_open())
			{
				stream.write(buffer, length + 8);
				stream.close();
			}

			printf("Finished saving game\n");
		}

		std::string savedata;
		SaveGameHeader header;

		void save_game_f() {
			scheduler::once(do_save, scheduler::server);
		}

		SaveGameHeader* get_savedata_header() {
			printf("Getting save data size\n");
			header.size = savedata.length() - 8;
			header.migration_version = 3;
			header.checksum = 0x1337;
			header.network_protocol_version = 18532;
			header.a4 = 0;
			header.a5 = 0;
			header.a6 = 0;

			return &header;
		}

		char* get_savedata_buffer() {
			printf("Getting save data buffer\n");
			return savedata.data();
		}

		uint64_t g_preload_game_stub(uint64_t a1, uint64_t a2) {
			printf("G_PreloadGame called (%llx, %llx)\n", a1, a2);
			savedata = utils::io::read_file("save_dump.bin");

			printf("Read %d bytes of data from save\n", savedata.length());

			int result = g_preload_game_hook.invoke<uint64_t>(a1, a2);
			return result;
		}

		int sv_init_game_vm_stub(uint64_t a1, uint64_t a2, bool use_save_game) {
			if (game::Com_IsInGame() && !game::Com_IsRunningUILevel()) {
				if (utils::io::file_exists("save_dump.bin")) {
					use_save_game = true;
				}
			}

			printf("SV_InitGameVM called (%llx, %llx, %llx)\n", a1, a2, use_save_game);
			int result = sv_init_game_vm_hook.invoke<int>(a1, a2, use_save_game);

			return result;
		}

		int block_checksum_stub(void* data, int len) {
			return 0x1337;
		}

		uint64_t livestats_read_xuid_from_stats_buffer_stub(uint64_t a1, uint64_t a2) {
			printf("Livestats_ReadXUIDFromStatsBuffer: %llx, %llx\n");

			uint64_t result = livestats_read_xuid_from_stats_buffer_hook.invoke<uint64_t>(a1, a2);

			game::access_connected_client(0, [&result](game::client_s& client) {
				printf("Client xuid is: %llx\n", client.xuid);
				result = client.xuid;
			});

			printf("Result xuid is: %llx\n", result);
			return result;
		}

		uint64_t sv_spawn_server_stub(uint64_t a1, uint64_t a2, int a3, bool a4) {
			// MAP_PRELOAD_INGAME = 2
			auto result = utils::hook::invoke<uint64_t>(0x01422528C0_g, a1, a2, 2, true);


			printf("MAP RELOAD HOOK CALLED!\n"); printf("MAP RELOAD HOOK CALLED!\n"); printf("MAP RELOAD HOOK CALLED!\n"); printf("MAP RELOAD HOOK CALLED!\n"); printf("MAP RELOAD HOOK CALLED!\n");

			return result;
		}
	}

	class component final : public generic_component
	{
	public:
		void post_unpack() override
		{
			command::add("save_game", save_game_f);
			g_preload_game_hook.create(0x14012E1C0_g, g_preload_game_stub);
			sv_init_game_vm_hook.create(0x14224F720_g, sv_init_game_vm_stub);
			livestats_read_xuid_from_stats_buffer_hook.create(0x141EA47C0_g, livestats_read_xuid_from_stats_buffer_stub);

			utils::hook::call(0x14012E1EA_g, get_savedata_header);
			utils::hook::call(0x14012E1F2_g, get_savedata_buffer);
			utils::hook::call(0x14012E292_g, block_checksum_stub);
			utils::hook::call(0x142245F25_g, sv_spawn_server_stub);
		}
	};
}

REGISTER_COMPONENT(savegame::component)
