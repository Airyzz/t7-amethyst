#include <std_include.hpp>
#include "../services.hpp"
#include "game/game.hpp"
#include "bdPooledStorage.hpp"
#include <utils/io.hpp>

namespace demonware
{
	bdContentStreaming::bdContentStreaming() : service(50, "bdContentStreaming")
	{
		this->register_task(2, &bdContentStreaming::unk2);
		this->register_task(14, &bdContentStreaming::list_all_publisher_files);
		this->register_task(3, &bdContentStreaming::idk);

		this->register_task(1, &bdPooledStorage::getPooledMetaDataByID);
		this->register_task(5, &bdPooledStorage::_preUpload);
		this->register_task(6, &bdPooledStorage::_postUploadFile);
		this->register_task(8, &bdPooledStorage::remove);
		this->register_task(9, &bdPooledStorage::_preDownload);
		this->register_task(17, &bdPooledStorage::_preUploadSummary);
		this->register_task(18, &bdPooledStorage::_postUploadSummary);
		this->register_task(19, &bdPooledStorage::_preDownloadSummary);
		this->register_task(20, &bdPooledStorage::_preUploadMultiPart);
		this->register_task(21, &bdPooledStorage::_postUploadMultiPart);
		this->register_task(22, &bdPooledStorage::_preDownloadMultiPart);
	}

	void bdContentStreaming::idk(service_server* server, byte_buffer* /*buffer*/) const
	{
		// TODO: Read data as soon as needed
		auto reply = server->create_reply(this->task_id());
		reply.send();
	}

	void bdContentStreaming::_preDownloadSummary(service_server* server, byte_buffer* buffer) const
	{
		uint64_t id;
		buffer->read_uint64(&id);
		auto reply = server->create_reply(this->task_id());
		auto demos = scan_folder_demo();

		auto result = std::make_unique<bdSummaryMetaHandler>();
		result->url = std::format("{}/cloud/{}/{}.summary", "http://localhost",demo_folder, demos[id]);

		std::string demoFilePath = demo_folder + "/" + demos[id];
		result->m_size = (uint32_t)utils::io::file_size((demoFilePath + ".summary"));

		reply.add(result);
		reply.send();

	}

	void bdContentStreaming::unk2(service_server* server, byte_buffer* /*buffer*/) const
	{
		// TODO:
		auto reply = server->create_reply(this->task_id());
		reply.send();
	}

	void bdContentStreaming::list_all_publisher_files(service_server* server, byte_buffer* buffer) const
	{
		std::uint32_t start_date;
		buffer->read_uint32(&start_date);

		std::uint16_t max_num_results;
		buffer->read_uint16(&max_num_results);

		std::uint16_t offset;
		buffer->read_uint16(&offset);

		std::uint16_t category;
		buffer->read_uint16(&category);

		std::string filename;
		buffer->read_string(&filename);

		/*if(filename.empty())
		{
			server->create_reply(this->task_id(), game::BD_NO_FILE).send();
			return;
		}*/

		auto reply = server->create_reply(this->task_id());
		reply.send();
	}
}
