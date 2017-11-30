#include "http_fetcher.h"
#include "config.h"
#include <iostream>
#include <fstream>


namespace HTPFetcherConst{
	typedef enum{
		no_error,
		error_connection_failed,
		error_connextion_timed_out
	}error_t;
}

#ifdef SYSTEM_WIN32

#include <windows.h>
#include <wininet.h>
#include "tchar.h"

int downloadPage(const std::string &filename, const std::string &url){
	using namespace std;
	HINTERNET iNet, file;
	iNet = InternetOpen(L"Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/49.0.2623.22 Safari/537.36 SE 2.X MetaSr 1.0", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
	if(!iNet) {
		return 1;
	}
	wchar_t *url_conv = new wchar_t [url.length()+1];
	mbstowcs(url_conv, url.c_str(), strlen(url.c_str()) + 1);//Plus null
	file = InternetOpenUrl(iNet, url_conv, NULL, 0, 0, 0);
	if (file) {
		char buffer[1024];
		DWORD read;

		// the file to store the page
		ofstream outFile(filename);

		while (InternetReadFile(file, buffer, 1023, &read)) {
			if (!read)
				break;
			buffer[read] = '\0';
			outFile.write(buffer, int(read));
		}

		//close handle and file
		InternetCloseHandle(file);
		outFile.close();
	}
	else {
		delete[] url_conv;
		return -1;
	}
	delete [] url_conv;
	InternetCloseHandle(iNet);
	return 0;
}

#endif
