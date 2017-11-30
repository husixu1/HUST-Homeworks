#pragma once

#include <string>
#include <cstring>
#include <cstdio>

std::string md5(std::string dat);
std::string md5(const void* dat, size_t len);
std::string md5file(const char* filename);
std::string md5file(FILE* file);
std::string md5sum6(std::string dat);
std::string md5sum6(const void* dat, size_t len);
