#pragma once
#include <fstream>
#include <stack>
#include <string>

class IO {
public:
	static bool BeginRead(const std::string& filePath) {
		IO& inst = getInstance();
		auto* current = new std::ifstream();
		current->open(filePath);
		if (current->fail()) {
			return false;
		}
		inst.readStack.push(current);
		inst.readFileNameStack.push(filePath);
		return true;
	}

	static void EndRead() {
		IO& inst = getInstance();
		std::ifstream* top = inst.readStack.top();
		if (top == nullptr) {
			printf(
				"Calling EndRead without calling BeginRead first. Ensure you have the same amount of BeginRead and EndRead calls.\n");
			return;
		}
		inst.readStack.pop();
		inst.readFileNameStack.pop();
		delete top;
	}

	static void ReadString(std::string& value, const std::string& skipLineChar = "#") {
		IO& inst = getInstance();
		std::ifstream* top = inst.readStack.top();
		std::string line;
		do {
			std::getline(*top, line);
		} while (line.starts_with(skipLineChar));
		value = line;
	}

	static void ReadInt(int& value, const std::string& skipLineChar = "#") {
		std::string read;
		ReadString(read, skipLineChar);
		value = std::stoi(read);
	}

	static void ReadBool(bool& value, const std::string& skipLineChar = "#") {
		int read;
		ReadInt(read, skipLineChar);
		value = read != 0;
	}
	
	static void ReadFloat(float& value, const std::string& skipLineChar = "#") {
		std::string read;
		ReadString(read, skipLineChar);
		value = std::stof(read);
	}

	static bool BeginWrite(const std::string& filePath, const int openMode = std::ios::out) {
		IO& inst = getInstance();
		auto* current = new std::ofstream();

		current->open(filePath, openMode);
		if (current->fail()) {
			return false;
		}
		inst.writeStack.push(current);
		inst.writeFileNameStack.push(filePath);
		return true;
	}

	static void WriteTitle(const std::string& title, const std::string& titleStartChar = "#") {
		IO& inst = getInstance();
		std::ofstream* top = inst.writeStack.top();
		*top << titleStartChar << title << "\n";
	}

	static void EndWrite() {
		IO& inst = getInstance();
		std::ofstream* top = inst.writeStack.top();
		if (top == nullptr) {
			printf(
				"Calling EndWrite without calling BeginWrite first. Ensure you have the same amount of BeginWrite and EndWrite calls.\n");
			return;
		}
		inst.writeStack.pop();
		inst.writeFileNameStack.pop();
		delete top;
	}

	template <typename T>
	static void Write(T value, const std::string& title = "", const std::string& titleStartChar = "#");
private:
	static IO& getInstance() {
		static IO instance;
		return instance;
	}

	~IO() {
		while (!readStack.empty()) {
			auto* current = readStack.top();
			printf("EndRead not called on file %s.\n", readFileNameStack.top().c_str());
			delete current;
			readStack.pop();
			readFileNameStack.pop();
		}

		while (!writeStack.empty()) {
			auto* current = writeStack.top();
			printf("EndWrite not called on file %s.\n", writeFileNameStack.top().c_str());
			delete current;
			writeStack.pop();
			writeFileNameStack.pop();
		}
	}

	IO() {
	}

	std::stack<std::ifstream*> readStack;
	std::stack<std::string> readFileNameStack;

	std::stack<std::ofstream*> writeStack;
	std::stack<std::string> writeFileNameStack;
};

template <typename T>
void IO::Write(T value, const std::string& title, const std::string& titleStartChar) {
	IO& inst = getInstance();
	std::ofstream* top = inst.writeStack.top();
	if (!title.empty()) {
		*top << titleStartChar << title << "\n";
	}
	*top << value << "\n";
}
