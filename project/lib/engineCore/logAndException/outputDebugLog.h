#pragma once
#include <string>
#include <fstream>


std::wstring ConvertString(const std::string& str);
std::string ConvertString(const std::wstring& str);
std::ofstream DebugLogInitialize();
void Log(std::string const& str_);
void Log(std::ostream& os, std::string const& message);

