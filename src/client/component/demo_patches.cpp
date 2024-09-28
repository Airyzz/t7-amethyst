#include <std_include.hpp>
#include "loader/component_loader.hpp"

#include <game/game.hpp>
#include <game/utils.hpp>

#include <utils/hook.hpp>
#include <utils/io.hpp>

#include <mmeapi.h>
#include <component/command.hpp>
#include <thread>
#include <utils/string.hpp>
#include "scheduler.hpp"
namespace demo_patches
{
	namespace
	{
		utils::hook::detour LiveFileshare_AddTag_hook;

		void patch_dvars()
		{
			auto demo_recordingrate = game::Dvar_FindVar("demo_recordingrate");
			demo_recordingrate->domain.integer.min = 1;

			game::dvar_add_flags("demo_recordingrate", game::DVAR_ARCHIVE);
			game::dvar_add_flags("demo_enableSvBandwidthLimitThrottle", game::DVAR_ARCHIVE);
		}

		__int64 LiveFileshare_AddTag_stub(uint64_t a1, uint64_t a2, int* a3, uint64_t a4, int a5) {
			uint64_t currentTagCount = *a3;

			if (a2 == UINT64_MAX)
				a2 = 0;

			if (currentTagCount + 1 <= a5) {
				*reinterpret_cast<uint64_t*>((currentTagCount << 6) + a4 + 32) = a1;
				*reinterpret_cast<uint64_t*>((currentTagCount << 6) + a4 + 40) = a2;
				++(*a3);
			}
			return currentTagCount + 1;
		}
	}

	class component final : public client_component
	{
	public:
		component()
		{
		}

		void post_unpack() override
		{
			scheduler::once(patch_dvars, scheduler::pipeline::main);

			// For no reason, multiplayer demos turn off after the first kill
			// nop CCS_ValidateChecksums for mp demo 
			utils::hook::nop(0x141365159_g, 5);

			// nop  if ( target < 0 || target >= com_maxclients )
			utils::hook::nop(0x1407F2055_g, 6);
			utils::hook::nop(0x1407F205D_g, 2);

			LiveFileshare_AddTag_hook.create(0x141DEF150_g, LiveFileshare_AddTag_stub);
		}
	};
}

REGISTER_COMPONENT(demo_patches::component)
