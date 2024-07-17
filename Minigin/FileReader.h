#pragma once
#include "Component.h"
#include <string>
#include <vector>
#include <memory>
#include <fstream>
#include <any>
#include <map>
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/prettywriter.h"

namespace dae {
	class FileReader final : public Component
	{
	public:

		FileReader(const std::string& filePath);
		~FileReader();
		FileReader(const FileReader& other) = delete;
		FileReader(FileReader&& other) = delete;
		FileReader& operator=(const FileReader& other) = delete;
		FileReader& operator=(FileReader&& other) = delete;

		void Update() override {};
		virtual void FixedUpdate() override {};
		void Render() const override {};

		void WriteData(std::map<std::string, std::string> dataKeys);
		void WriteString(std::string text);
		const rapidjson::Document& GetDocument() { return m_Document; };

		void Open();
		void Close();

	private:
		std::ifstream m_File;
		std::string m_FileName;
		rapidjson::Document m_Document;

		const rapidjson::Document& ReadFile();
	};
}