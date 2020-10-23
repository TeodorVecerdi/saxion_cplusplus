#pragma once
#include <algorithm>
#include <memory>
#include <string>
#include <stdexcept>


// Taken from: https://stackoverflow.com/a/26221725/5181524
template<typename ... Args>
static std::string string_format(const std::string& format, Args ... args) {
    size_t size = snprintf(nullptr, 0, format.c_str(), args ...) + 1;
    if(size <= 0) { throw std::runtime_error("Error during formatting."); }
    std::unique_ptr<char[]> buf(new char[size]);
    snprintf(buf.get(), size, format.c_str(), args ...);
    return std::string(buf.get(), buf.get() + size - 1);
}

template<typename T>
static std::string default_identifier(T* object) {
    auto typeName = std::string(typeid(T).name());
    size_t start = typeName.find("class ");
    if(start != std::string::npos)
        typeName = typeName.replace(start, 6, "");
    return string_format("%s_%x", typeName.c_str(), object);
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
    return string_format("%c%s", toupper(str[0]), str.substr(1));
}