#pragma once

#include <Files/IFile.hpp>

using namespace acid;

namespace micro
{
	class ConfigManager
	{
	private:
		std::unique_ptr<IFile> m_audio;
		std::unique_ptr<IFile> m_graphics;
	public:
		ConfigManager();

		void Load();

		void Save();
	};
}
