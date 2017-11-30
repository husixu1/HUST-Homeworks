#include "HTML_analyser.h"
#include "http_fetcher.h"
#include "md5.h"
#include <fstream>
#include <ctime>
#include <stack>

#include "cocos2d.h"

HTMLAnalyser::HTMLAnalyser(bool doesThrowException_) {
	doesThrowException = doesThrowException_;
}

void HTMLAnalyser::throwException(HTMLAnalyserConst::error_t exception) {
	if (doesThrowException)
		throw exception;
	return;
}

void HTMLAnalyser::setUrl(const std::string &url_) {
	url = url_;
}

void HTMLAnalyser::download(const std::string &url, const std::string &filename) {
	downloadPage(filename, url);
}

class DOMTree HTMLAnalyser::downloadAndAnalyse(const std::string &fileName) {
	using namespace std;
	using namespace HTMLAnalyserConst;
	
	// download page
	string tempFileName;
	if (fileName == "")
		tempFileName += string("HTML_Page_") += md5(url) += string("_") += string("time_") += time(nullptr);
	else
		tempFileName = fileName;
	if (downloadPage(tempFileName, url) != 0)
		throw 1;
	
	return loadAndAnalyse(tempFileName);
}

// isspace that used to process unicode
inline bool m_isspace(char ch) {
	if (ch == ' ' ||
		ch == '\n' ||
		ch == '\r' ||
		ch == '\t' ||
		ch == '\v' ||
		ch == '\f')
		return true;
	return false;
}

class DOMTree HTMLAnalyser::loadAndAnalyse(const std::string &fileName) {
	using namespace std;
	using namespace HTMLAnalyserConst;
	// analyse the content
	ifstream inFile(fileName);
	if (!inFile.is_open()) {
		throwException(error_file_corrupt);
		return DOMTree();
	}
	// mark the reading status
	enum {
		status_start,
		status_open_tag,
		status_close_tag,
		status_comment,
		status_content
	} readStatus = status_start;
	// mark if the char should be escaped
	enum {
		status_in_string,
		status_out_of_string
	} inStringStatus = status_out_of_string;
	// mark if is in javascript
	enum {
		status_in_script,
		status_out_of_script
	} inScriptStatus = status_out_of_script;
	const char voidElementTable[16][10] = {
		"br",		"hr",		"img",		"input",
		"link",		"meta",		"area",		"base",
		"col",		"command",	"embed",	"keygen",
		"param",	"source",	"track	",	"wbr",
	};

	//global variables
	DOMTree result;
	DOMTree::iterator tempItr;
	char ch;
	

	// debug status and not debug status
#ifdef STATUS_DEBUG
	std::cout << "start Analyse" << endl;
	std::string __test_buffer;
	#define __getchar_command inFile.get(ch), cout << ch << flush
#else
	#define __getchar_command inFile.get(ch)
#endif

	// record the unclosed tag num while reading, use it to judge the hierarchy
	int unclosedTagNum = 0;
	
	//read till the tag opening
	for (__getchar_command; m_isspace(ch) && !inFile.eof(); __getchar_command);
	if (inFile.eof())
		return result;
	// opening char must be <
	if (ch != '<')
		return result;
	
	// skip the current and following blank
	#define __skip_blank {	for(;m_isspace(ch) && !inFile.eof(); __getchar_command); \
							if(inFile.eof()){throw error_file_corrupt;}else{;} }
	// skip to the next not blank char
	#define __skip_to_next {__getchar_command; \
							for(;m_isspace(ch) && !inFile.eof(); __getchar_command); \
							if(inFile.eof()){throw error_file_corrupt;}else{;} }

	#define __skip_to_next_regradness_eof {	__getchar_command; \
											for(;m_isspace(ch) && !inFile.eof(); __getchar_command); \
											if(inFile.eof()){break;}else{;}  }
	
	// read the html content
	for (;!inFile.eof();) {
		// decide the reading status
		if (ch == '<') {
			__skip_to_next;

			if (ch == '/') {
				string tempBuffer;
				if (inScriptStatus == status_in_script &&
					(__getchar_command, tempBuffer += ch, (ch == 's' || ch == 'S')) &&
					(__getchar_command, tempBuffer += ch, (ch == 'c' || ch == 'C')) &&
					(__getchar_command, tempBuffer += ch, (ch == 'r' || ch == 'R')) &&
					(__getchar_command, tempBuffer += ch, (ch == 'i' || ch == 'I')) &&
					(__getchar_command, tempBuffer += ch, (ch == 'p' || ch == 'P')) &&
					(__getchar_command, tempBuffer += ch, (ch == 't' || ch == 'T'))) {
					
					readStatus = status_close_tag;
				}
				else if (inScriptStatus == status_in_script) {
					if (*tempItr)
						(*tempItr)->content += tempBuffer;
					readStatus = status_content;
				}
				else {
					readStatus = status_close_tag;
				}
				__skip_to_next
			}
			else if (ch == '!') {
				if (inScriptStatus != status_in_script) {
					readStatus = status_comment;
					__skip_to_next
				}
				else {
					if (*tempItr)
						(*tempItr)->content += string("<") += ch;
					inFile.get(ch);
				}
			}
			else {
				if (inScriptStatus != status_in_script)
					readStatus = status_open_tag;
				else {
					if (*tempItr)
						(*tempItr)->content += string("<") += ch;
					inFile.get(ch);
				}
			}
		}
		
		else {
			readStatus = status_content;
		}
		
		// read the content according to the status
		switch (readStatus){
			case status_open_tag: {
				// a temp node
				HTMLNode tempNode;
				
				// read the type
				for (; !isblank(ch) && ch != '>'; __getchar_command)
					tempNode.type += ch;
				__skip_blank
				
				// if the open tag is script
				//if (tempNode.type == "script" || tempNode.type == "Script" || tempNode.type == "SCRIPT" )
				if(_stricmp(tempNode.type.c_str(), "script")==0)
					inScriptStatus = status_in_script;

				// read the properties
				for (; ch != '/' && ch != '>';) {
					// a temp property
					pair <string, string> tempProperty;
					
					// read the key
					for (; ch != '=' && !isblank(ch); __getchar_command)
						tempProperty.first += ch;
					__skip_blank
					
					if (ch != '=') {
						if(doesThrowException)
							throw error_key_has_no_value;
						continue;
					}
					__skip_to_next
					
					// read the value
					for (; !isblank(ch); __getchar_command) {
						if (ch == '"') {
							__getchar_command;
							for (; ch != '"'; __getchar_command)
								tempProperty.second += ch;
							__skip_to_next;
							break;
						}
						else if (ch == '\'') {
							__getchar_command;
							for (; ch != '\''; __getchar_command)
								tempProperty.second += ch;
							__skip_to_next
							break;
						}
						else
							tempProperty.second += ch;
					}
					__skip_blank;
					
					// save the property
					tempNode.properties.push_back(tempProperty);
				}
				
				// if the tag is root
				if (result.isEmpty()) {
					result.assignRoot(tempNode);
					tempItr = result.begin();
					unclosedTagNum++;
				}
				else {
					tempItr.insertChildAtTail(tempNode);
					unclosedTagNum++;
					tempItr = tempItr.firstChild();
					for(;!tempItr.rSibling().isEmpty(); tempItr = tempItr.rSibling());
				}

				// if this node is a void element
				bool isVoidElement = false;
				for (int i = 0; i < 16; i++) {
					if (_stricmp(tempNode.type.c_str(), voidElementTable[i]) == 0) {
						isVoidElement = true;
						break;
					}
				}

				// judge if the tag is closed in place
				if (ch == '/' ) {
					unclosedTagNum--;
					if (!result.isEmpty())
						tempItr = tempItr.parent();
					__skip_to_next
					
					if (ch != '>')
						throw error_tag_not_closed;
					
					__skip_to_next
				}
				else{
					if (isVoidElement) {
						if (!result.isEmpty())
							tempItr = tempItr.parent();
					}

					if (ch != '>')
						throw error_tag_not_closed;
					
					__skip_to_next
				}
				
			}
				break;
			case status_content:
				if (inScriptStatus == status_out_of_script) {
					for (; ch != '<';) {
						if (ch == '"') {
							tempItr.getValue()->content += ch;
							__getchar_command;
							for (; ch != '"'; __getchar_command)
								tempItr.getValue()->content += ch;
							tempItr.getValue()->content += ch;
							__getchar_command;
						}
						else if (ch == '\'') {
							tempItr.getValue()->content += ch;
							__getchar_command;
							for (; ch != '\''; __getchar_command)
								tempItr.getValue()->content += ch;
							tempItr.getValue()->content += ch;
							__getchar_command;
						}
						else {
							tempItr.getValue()->content += ch;
							__getchar_command;
						}
					}
				}
				// use another method for javascript
				else {
					std::string alphabetBuffer;
					for(;ch != '<' ;) {
						if ( ch >= 0 && isalpha(ch))
							alphabetBuffer += ch;
						else
							alphabetBuffer.clear();
						
						tempItr.getValue()->content += ch;
						__getchar_command;

						// jump the regexp
						if( alphabetBuffer == "replace" || 
							alphabetBuffer == "match"	||
							alphabetBuffer == "search"	||
							alphabetBuffer == "split"	) {
							for (; ch != ')';) {
								if (ch == '\\') {
									tempItr.getValue()->content += ch;
									__getchar_command;
								}
								tempItr.getValue()->content += ch;
								__getchar_command;
							}
							tempItr.getValue()->content += ch;
							__getchar_command;
						}
						else if(ch == '"' || ch == '\'') {
							char matchCh = ch;
							for (; ch != matchCh;) {
								if (ch == '\\') {
									tempItr.getValue()->content += ch;
									__getchar_command;
								}
								tempItr.getValue()->content += ch;
								__getchar_command;
							}
							tempItr.getValue()->content += ch;
							__getchar_command;
						}
					}
				}
				break;
			case status_close_tag: {
				if (inScriptStatus == status_in_script) {
					inScriptStatus = status_out_of_script;
					
					if (*tempItr)
						tempItr = tempItr.parent();
					if (!*tempItr)
						goto end_analyse;

					if (ch != '>')
						throw error_tag_not_closed;
					__skip_to_next_regradness_eof;
					break;
				}

				string tempStr;
				for (; !isblank(ch) && ch != '>'; tempStr.push_back(ch), __getchar_command);

				if (tempStr == "html")
					cout << "stop Analyse" << endl;

				// html allows somt tag unclosed
				// raise the level of all unclosed tags
				for (; *tempItr && tempItr.getValue()->type != tempStr; tempItr = tempItr.parent(), unclosedTagNum--);
				if (*tempItr)
					tempItr = tempItr.parent();
				if (!*tempItr)
					goto end_analyse;

				__skip_blank;
				
				if(ch!='>')
					throw error_tag_not_closed;
				
				__skip_to_next_regradness_eof;
				break;
			}
			case status_comment:
				for(int count = 0 ; count>0 || ch!='>';){
					if (ch == '"') {
						for (__getchar_command; ch != '"'; __getchar_command);
						__getchar_command;
					}
					else if (ch == '\'') {
						for (__getchar_command; ch != '\''; __getchar_command);
						__getchar_command;
					}
					else if (ch == '<') {
						count++;
						__getchar_command;
					}
					else if (ch == '>') {
						count--;
						__getchar_command;
					}
					else
						__getchar_command;
				}
				__skip_to_next
				break;
		}
	}
	end_analyse:

	#undef __skip_blank
	#undef __skip_to_next

	inFile.close();
	return result;
};