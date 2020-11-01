#pragma once

#include <algorithm>
#include <cstdarg>
#include <memory>
#include <sstream>
#include <string>

// Taken from: https://stackoverflow.com/a/26221725/5181524
template <typename ... Args>
static std::string string_format(const std::string& format, Args ... args) {
	const size_t size = snprintf(nullptr, 0, format.c_str(), args ...) + 1;
	if (size <= 0) { throw std::runtime_error("Error during formatting."); }
	const std::unique_ptr<char[]> buf(new char[size]);
	snprintf(buf.get(), size, format.c_str(), args ...);
	return std::string(buf.get(), buf.get() + size - 1);
}

static std::string replace(std::string str, const std::string from, const std::string to) {
	while (true) {
		const size_t found = str.find(from);
		if (found == std::string::npos) break;
		str = str.replace(found, from.size(), to);
	}
	return str;
}

static std::string uppercase(std::string str) {
	std::transform(str.begin(), str.end(), str.begin(), toupper);
	return str;
}

static std::string lowercase(std::string str) {
	std::transform(str.begin(), str.end(), str.begin(), tolower);
	return str;
}

static std::string capitalize(std::string str) {
	return string_format("%c%s", toupper(str[0]), str.substr(1).c_str());
}

static std::string get_spacing_tabs(std::string str, uint32_t tabs, uint32_t tabSize = 4) {
	int totalSize = tabs * tabSize;
	totalSize -= str.size();
	if(totalSize < 0) totalSize = 0;
	return std::string(totalSize, ' ');
}

template <typename T>
static std::string default_identifier(T* object) {
	auto typeName = std::string(typeid(T).name());
	typeName = replace(typeName, "class ", "");
	std::ostringstream address;
	address << static_cast<const void*>(object);
	return string_format("%s_%s", typeName.c_str(), capitalize(address.str()).c_str());
}