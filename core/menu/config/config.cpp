#include <fstream>
#include <ShlObj.h>
#include "config.hpp"
#include "archivex.hpp"

config cfg;

void config::run(const char* name) noexcept {
	PWSTR pathToDocuments;
	if (SUCCEEDED(SHGetKnownFolderPath(FOLDERID_Documents, 0, NULL, &pathToDocuments))) {
		path = pathToDocuments;
		path /= name;
		CoTaskMemFree(pathToDocuments);
	}

	if (!std::filesystem::is_directory(path)) {
		std::filesystem::remove(path);
		std::filesystem::create_directory(path);
	}

	std::transform(std::filesystem::directory_iterator{ path },
		std::filesystem::directory_iterator{ },
		std::back_inserter(configs),
		[](const auto& entry) { return entry.path().filename().string(); });
}

void config::load(size_t id) noexcept {
	if (!std::filesystem::is_directory(path)) {
		std::filesystem::remove(path);
		std::filesystem::create_directory(path);
	}

	std::ifstream in{ path / configs[id] };

	if (!in.good())
		return;

	ArchiveX<std::ifstream>{ in } >> aimbot;
	ArchiveX<std::ifstream>{ in } >> visuals;
	ArchiveX<std::ifstream>{ in } >> misc;
	ArchiveX<std::ifstream>{ in } >> config;
	in.close();
}

void config::save(size_t id) const noexcept {
	if (!std::filesystem::is_directory(path)) {
		std::filesystem::remove(path);
		std::filesystem::create_directory(path);
	}

	std::ofstream out{ path / configs[id] };

	if (!out.good())
		return;

	ArchiveX<std::ofstream>{ out } << aimbot;
	ArchiveX<std::ofstream>{ out } << visuals;
	ArchiveX<std::ofstream>{ out } << misc;
	ArchiveX<std::ofstream>{ out } << config;
	out.close();
}

void config::add(const char* name) noexcept {
	if (*name && std::find(std::cbegin(configs), std::cend(configs), name) == std::cend(configs))
		configs.emplace_back(name);
}

void config::remove(size_t id) noexcept {
	std::filesystem::remove(path / configs[id]);
	configs.erase(configs.cbegin() + id);
}

void config::rename(size_t item, const char* newName) noexcept {
	std::filesystem::rename(path / configs[item], path / newName);
	configs[item] = newName;
}

void config::reset() noexcept {
	aimbot = { };
	visuals = { };
	misc = { };
	config = { };
}