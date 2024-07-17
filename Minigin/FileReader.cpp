#include "FileReader.h"
#include <algorithm>
#include <sstream>
#include <vector>
#include <iostream>
#include <type_traits>
#include <cassert>

#define _SILENCE_CXX20_IS_POD_DEPRECATION_WARNING
#define _SILENCE_ALL_CXX20_DEPRECATION_WARNINGS


dae::FileReader::FileReader(const std::string& filePath)
	: m_File{}
	, m_FileName{ filePath }
{
	ReadFile();
}

dae::FileReader::~FileReader()
{
	Close();
}

const rapidjson::Document& dae::FileReader::ReadFile() {
	Open();
	std::ifstream inputFile(m_FileName);
	if (!inputFile) {
		std::cerr << "Failed to open the input file." << std::endl;
	}

	std::string content;
	std::string line;

	while (std::getline(inputFile, line)) {
		content += line;
	}
	Close();

	m_Document.Parse(content.c_str());

	if (m_Document.HasParseError()) {
		std::cerr << "Parsing failed. Error code: " << m_Document.GetParseError() << std::endl;
	}

	if (m_Document.IsObject()) {
		const rapidjson::Value& contents = m_Document.GetObject();
		rapidjson::StringBuffer buffer;
		rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
		contents.Accept(writer);
		return m_Document;
	}
	else {
		m_Document.SetObject();
	}
	return m_Document;
}

void dae::FileReader::WriteData(std::map<std::string, std::string> dataKeys)
{
	rapidjson::Document document;
	document.SetObject();
	rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
	for (auto& item : dataKeys) {
		document.AddMember(rapidjson::StringRef(item.first.c_str()), rapidjson::StringRef(item.second.c_str()), allocator);
	}

	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	document.Accept(writer);

	std::ofstream ofs(m_FileName);
	if (ofs.is_open()) {
		ofs << buffer.GetString();
		std::cout << "Successfully wrote JSON to " << m_FileName << std::endl;
	}
	else {
		std::cerr << "Failed to open file " << m_FileName << std::endl;
	}
	Close();
}

void dae::FileReader::WriteString(std::string text)
{
	std::ofstream outFile(m_FileName);
	if (!outFile.is_open()) {
		std::cerr << "Failed to open the file: " << m_FileName << std::endl;
		return;
	}

	outFile << text;
	Close();
}


void dae::FileReader::Open()
{
	m_File.open(m_FileName, std::ios::in);
}

void dae::FileReader::Close()
{
	m_File.close();
}