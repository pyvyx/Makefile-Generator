#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <cstring>

class InputHandler
{
private:
	int m_Arguments;
	bool m_Lowered, m_Uppered;
	std::vector<std::string> m_InputStrings;
public:
	InputHandler(int& argc, char** argv) : m_Arguments(argc), m_Lowered(false), m_Uppered(false)
	{
		m_InputStrings.reserve(argc);
		for (int i = 0; i < m_Arguments; ++i)
			m_InputStrings.emplace_back(argv[i]);
	}


	bool contains(const char* element)
	{
		for (int i = 0; i < m_Arguments; ++i)
			if (strcmp(m_InputStrings[i].c_str(), element) == 0)
				return true;

		return false;
	}

	bool contains_l(const std::string& element)
	{
		std::string element_l = lower(element);

		if (m_Lowered)
		{
			for (int i = 0; i < m_Arguments; ++i)
				if (m_InputStrings[i] == element_l)
					return true;

			return false;
		}

		for (int i = 0; i < m_Arguments; ++i)
			if (lower(m_InputStrings[i]) == element_l)
				return true;

		return false;
	}

	bool contains_u(const std::string& element)
	{
		std::string element_u = upper(element);

		if (m_Uppered)
		{
			for (int i = 0; i < m_Arguments; ++i)
				if (m_InputStrings[i] == element_u)
					return true;

			return false;
		}

		for (int i = 0; i < m_Arguments; ++i)
			if (upper(m_InputStrings[i]) == element_u)
				return true;

		return false;
	}

	int isNumber(const char* data)
	{
		try {
			int number = std::stoi(data);
			return number;
		}
		catch (...) {
			return false;
		}
	}

	int location(const char* data)
	{
		for (int i = 0; i < m_Arguments; ++i)
			if (strcmp(m_InputStrings[i].c_str(), data) == 0)
				return i;
		return -1;
	}

	int location_l(const char* data)
	{
		std::string data_l = lower(data);
		if (m_Lowered)
		{
			for (int i = 0; i < m_Arguments; ++i)
				if (data_l == m_InputStrings[i])
					return i;
			return -1;
		}

		for (int i = 0; i < m_Arguments; ++i)
			if (data_l == lower(m_InputStrings[i]))
				return i;
		return -1;
	}

	int location_u(const char* data)
	{
		std::string data_u = upper(data);
		if (m_Uppered)
		{
			for (int i = 0; i < m_Arguments; ++i)
				if (data_u == m_InputStrings[i])
					return i;
			return -1;
		}

		for (int i = 0; i < m_Arguments; ++i)
			if (data_u == upper(m_InputStrings[i]))
				return i;
		return -1;
	}


	std::string lower(const std::string& data)
	{
		std::string result;

		for (size_t i = 0; i < data.size(); ++i)
			result += static_cast<char>(::tolower(data[i]));

		return result;
	}

	std::string upper(const std::string& data)
	{
		std::string result;

		for (size_t i = 0; i < data.size(); ++i)
			result += static_cast<char>(::toupper(data[i]));

		return result;
	}

	void upperAll()
	{
		if (m_Uppered)
			return;

		for (size_t i = 0; i < m_InputStrings.size(); ++i)
			for (size_t j = 0; j < m_InputStrings[i].size(); ++j)
				m_InputStrings[i][j] = static_cast<char>(std::toupper(m_InputStrings[i][j]));

		m_Uppered = true;
		m_Lowered = false;
	}

	void lowerAll()
	{
		if (m_Lowered)
			return;

		for (size_t i = 0; i < m_InputStrings.size(); ++i)
			for (size_t j = 0; j < m_InputStrings[i].size(); ++j)
				m_InputStrings[i][j] = static_cast<char>(std::tolower(m_InputStrings[i][j]));

		m_Uppered = false;
		m_Lowered = true;
	}


	inline std::string& operator[](const int& index) { return m_InputStrings[index]; }

	inline const int number(const std::string& data) const { return std::stoi(data); }
	inline const int number(const char* data) const { return std::stoi(data); }
	inline const int size() const { return m_Arguments; }
	inline const std::vector<std::string>& elements() const { return m_InputStrings; }
	inline const char* at(const int& index) const { return m_InputStrings[index].c_str(); }

	void print()
	{
		std::cout << '[';
		for (int i = 0; i < m_Arguments; ++i)
			std::cout << m_InputStrings[i] << ", ";
		std::cout << "]\n";
	}
};


static void PrintHelpMessage(const char* fileName) {

	std::cout << "Usage: " << fileName << " <options>\n";
	std::cout << "       -g or -gui = start in GUI mode\n";
	std::cout << "       <filename> = load this config file\n";
	std::cout << "If you're providing a file name without using the GUI mode\n";
	std::cout << "the makefile will immediately be created\n";
}


struct Input {
	bool guiMode;
	std::string filePath;
	bool error;
};


Input HandleInput(int argc, char** argv) {

	bool usingGuiMode = true;
	std::string filePath;
	bool error = false;

	InputHandler ip(argc, argv);
	ip.lowerAll();
	if (ip.contains("-h")) {
		PrintHelpMessage(argv[0]);
		error = true;
	}
	else if (ip.size() > 1)
	{
		if (!ip.contains("-g") && !ip.contains("-gui"))
			usingGuiMode = false;
		if (ip.size() > 2) {
			int location = ip.location("-g");
			if (location == -1)
				location = ip.location("-gui");
			if (location == 1) {
				filePath = ip[2];
			}
			else {
				filePath = ip[1];
			}
		}
		else if (!usingGuiMode) {
			filePath = ip[1];
		}
	}

	#ifndef USING_IMGUI
	if (usingGuiMode) {
		std::cout << "You don't have the gui mode installed! Recompile the program to get access to the gui mode\n";
		error = true;
	}
	#endif

	return { usingGuiMode, filePath, error };
}
