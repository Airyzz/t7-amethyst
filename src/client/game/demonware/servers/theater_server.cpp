#include <std_include.hpp>

#include "theater_server.hpp"
#include <utils/io.hpp>

namespace demonware
{
	inline std::string req_end = "\r\n\r\n";
	void theater_server::send_reply(reply* data)
	{
		if (!data) return;
		this->send(data->data());
	}

	std::string get_file_path(const std::string& request)
	{
		std::string delimiter = "/cloud/";
		std::string path;

		size_t pos = request.find(delimiter);
		if (pos != std::string::npos) {
			path = request.substr(pos + delimiter.length());
			size_t space_pos = path.find(" ");
			if (space_pos != std::string::npos) {
				path = path.substr(0, space_pos);
			}
			return path;
		}
		else {
			return "";
		}
	}

	std::string get_request_body(const std::string& request)
	{
		size_t pos = request.find("\r\n");
		if (pos == std::string::npos) {
			return "";
		}
		return request.substr(pos + 2, request.size() - pos - 4);

	}

	void doStuff(const std::string& body, std::string& path)
	{
		std::ofstream ofs(path.data(), std::ios::app | std::ios::binary);
		utils::io::write_file(path, body,true);
	}

	void theater_server::handle(const std::string& packet)
	{
		static bool put_chunk = false;
		static bool put = false;
		static std::string file;

		if (packet.starts_with("GET /cloud/"))
		{
			std::string path = get_file_path(packet);
			if (path.empty())
				return;

			std::ifstream ifs(path.data(), std::ios::binary);

			size_t range_pos = packet.find("range: bytes=");
			if (range_pos != std::string::npos) {
				size_t eq_pos = packet.find("=", range_pos);
				size_t dash_pos = packet.find("-", range_pos);
				size_t crlf_pos = packet.find("\r\n", range_pos);
				if (eq_pos != std::string::npos && dash_pos != std::string::npos && crlf_pos != std::string::npos) {
					std::string start_str = packet.substr(eq_pos + 1, dash_pos - eq_pos - 1);
					std::string end_str = packet.substr(dash_pos + 1, crlf_pos - dash_pos - 1);
					size_t start = std::stoul(start_str);
					size_t end = std::stoul(end_str);
					if (start > end) {
						return;
					}
					std::string content((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
					std::string range_content = content.substr(start, end - start + 1);
					std::ostringstream result;
					result << "HTTP/1.1 206 Partial Content\r\nContent-Range: bytes " << start << "-" << end << "/" << content.size() << "\r\nContent-Length: " << range_content.size() << req_end << range_content;
					raw_reply reply(result.str());
					this->send_reply(&reply);
					return;
				}
			}

			std::ostringstream result;
			std::string content((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));

			result << "HTTP/1.1 200 OK\r\nContent-Length: " << content.size() << req_end << content;

			raw_reply reply(result.str());
			this->send_reply(&reply);
			return;
		}
		else if (packet.starts_with("PUT /cloud/"))
		{
			file = get_file_path(packet);
			if (file.empty())
			{
				std::string response = "HTTP/1.1 400 Bad Request\r\n\r\nInvalid file path";
				raw_reply reply(response);
				this->send_reply(&reply);
				return;
			}

			size_t chunked_pos = packet.find("Transfer-Encoding: chunked");
			if (chunked_pos != std::string::npos) {
				put_chunk = true;
				return;
			}
			put = true;
			put_chunk = false;
			return;
		}
		else if (put_chunk || put)
		{
			std::string body;
			if (put_chunk)
				body = get_request_body(packet);
			else
				body = packet;
			doStuff(body, file);

			if (packet.ends_with(req_end) || put)
			{
				put_chunk = false;
				put = false;
				std::string response = "HTTP/1.1 201 Created\r\nContent-Length: 0\r\n\r\n";
				raw_reply reply(response);
				this->send_reply(&reply);
				return;
			}
		}
		else {
			std::string response = "HTTP/1.1 400 Bad Request\r\n\r\nInvalid request method";
			raw_reply reply(response);
			this->send_reply(&reply);
			return;
		}

	}
}
