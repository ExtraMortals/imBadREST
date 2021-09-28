#include <windows.h>
#include <cpprest/http_client.h>
#include <cpprest/filestream.h>
#include <sstream>
#include <fstream>

void http_send_form_data(const char* filename, const std::string& content) {
	using namespace std;
	using namespace web;
	using namespace web::http;
	using namespace web::http::client;

	auto url = uri_builder()
		.set_scheme(L"http")
		.set_host(L"192.168.1.254")
		.set_port(L"3001")
		.set_path(L"profile")
		.to_uri();
	http_client client(url);
	string boundary = "X-BADGUY";

	ostringstream oss;
	oss << "--" << boundary << "\r\n";
	oss << "Content-Disposition: form-data; name=\"image\"; filename=\""<< filename << "\"\r\n";
	oss << "Content-Type: image/png\r\n\r\n";\
	oss << content; // real image data...
	oss << "\r\n--" << boundary << "--\r\n";

	http_request req(methods::POST);
	req.set_body(oss.str(), "multipart/form-data; boundary=" + boundary);
 
	auto future = client.request(req);
	try{
		auto result = future.get();
		cout << "send " << filename << endl;
	}
	catch (const http_exception& e){
		wcerr << e.what() << e.error_code() << endl;
	}
	return;
}

int main() {
	using namespace std;

	ifstream ifs("C:\\Users\\jychoi\\Desktop\\초기화면.png", ios::binary | ios::ate);
	ostringstream oss;
	if (ifs.is_open()) {
		auto sz = ifs.tellg();
		string buff(sz, 0);
		ifs.seekg(0);
		unsigned char name[3] = { 0 };

		if (ifs.read(&buff[0], sz)) {
			for (unsigned char i = 5ul; i < 255ul; i++)
			{
				for (unsigned char j = 0ul; j < 255ul; j++)
				{
					name[0] = i;
					name[1] = j;
					printf("%d %d\n", i, j);
					http_send_form_data(reinterpret_cast<char*>(name), buff);
				}
			}
		}
	}
}
