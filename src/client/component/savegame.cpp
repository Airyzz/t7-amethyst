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

namespace savegame
{
	namespace
	{
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
			game::Migration_SaveData(file, 0, 1);

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

		void save_game_f() {
			scheduler::once(do_save, scheduler::server);
		}
	}

	class component final : public generic_component
	{
	public:
		void post_unpack() override
		{
			command::add("save_game", save_game_f);
		}
	};
}

REGISTER_COMPONENT(savegame::component)
