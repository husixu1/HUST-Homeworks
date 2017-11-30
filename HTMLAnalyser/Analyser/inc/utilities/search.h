#pragma once

#include <iostream>
#include <string>
#include <vector>

std::vector<int> KMPSearch(const std::string &str, const std::string &pattern);

int KMPSearchFirst(const std::string &str, const std::string &pattern);

std::vector<int> BMSearch(const std::string &str, const std::string &pattern);

