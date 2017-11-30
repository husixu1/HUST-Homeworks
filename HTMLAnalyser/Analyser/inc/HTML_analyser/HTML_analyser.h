#pragma once

#include "HTML_analyser.h"
#include "DOM_tree_ADT.h"
#include <iostream>
#include <string>
#include <vector>

namespace HTMLAnalyserConst{
	typedef enum {
		no_error,
		error_file_corrupt,
		error_key_has_no_value,
		error_tag_not_closed
		
	}error_t;
}
class HTMLAnalyser {
private:
	std::string url;

protected:
	bool doesThrowException;
	void throwException(HTMLAnalyserConst::error_t exception);
	
public:
	
	HTMLAnalyser(bool doesThrowException = false);
	void setUrl(const std::string &url);
	
	void download(const std::string &url, const std::string &);
	class DOMTree downloadAndAnalyse(const std::string &filename = "");
	class DOMTree loadAndAnalyse(const std::string &filename);
	
};
