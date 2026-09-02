#pragma once

#include <cstdint>
#include <filesystem>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

namespace filemind {

struct FileInfo {
    std::string name;
    std::string path;
    std::string extension;
    std::uintmax_t size = 0;
    std::int64_t modified_time = 0;
    bool is_directory = false;
};

struct FileQuery {
    std::string text;
    std::vector<std::string> extensions;
    std::optional<std::filesystem::path> directory;
};

enum class SortOrder { NameAsc, ModifiedDesc, SizeDesc };

class FileEngine {
public:
    explicit FileEngine(std::filesystem::path root = std::filesystem::current_path());
    std::vector<FileInfo> search_files(const FileQuery& query) const;
    std::vector<FileInfo> list_directory(const std::filesystem::path& directory = {}) const;
    std::optional<FileInfo> get_file_info(const std::filesystem::path& path) const;
    std::vector<FileInfo> filter_files(std::vector<FileInfo> files, const FileQuery& query) const;
    std::vector<FileInfo> sort_files(std::vector<FileInfo> files, SortOrder order) const;

private:
    std::filesystem::path root_;
    bool safe_path(const std::filesystem::path& path) const;
    static FileInfo describe(const std::filesystem::directory_entry& entry);
};

} // namespace filemind
