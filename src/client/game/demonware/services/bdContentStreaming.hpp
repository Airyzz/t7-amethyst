#pragma once

namespace demonware
{
	class bdContentStreaming final : public service
	{
	public:
		bdContentStreaming();

	private:
		void unk2(service_server* server, byte_buffer* buffer) const;
		void idk(service_server* server, byte_buffer* buffer) const;
		void _preDownloadSummary(service_server* server, byte_buffer*) const;
		void _preUpload(service_server* server, byte_buffer*) const;
		void _postUploadFile(service_server* server, byte_buffer*) const;
		void list_files_by_owners(service_server* server, byte_buffer* buffer) const;
	};
}
