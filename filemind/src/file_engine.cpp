#include "filemind/file_engine.hpp"

#include <algorithm>
#include <chrono>
#include <cctype>
#include <system_error>

namespace filemind {
namespace fs = std::filesystem;

FileEngine::FileEngine(fs::path root) : root_(fs::weakly_canonical(std::move(root))) {}

bool FileEngine::safe_path(const fs::path& path) const {
    std::error_code ec;
    const auto candidate = fs::weakly_canonical(path.is_absolute() ? path : root_ / path, ec);
    if (ec) return false;
    auto root = fs::weakly_canonical(root_, ec);
    if (ec) return false;
    const auto relative = fs::relative(candidate, root, ec);
    if (ec) return false;
    const auto rel = relative.lexically_normal().string();
    return rel.empty() || (rel != ".." && rel.rfind("../", 0) != 0 && rel.rfind("..\\", 0) != 0);
}

FileInfo FileEngine::describe(const fs::directory_entry& entry) {
    std::error_code ec;
    FileInfo out;
    out.name = entry.path().filename().string();
    out.path = entry.path().string();
    out.is_directory = entry.is_directory(ec);
    if (!out.is_directory) out.size = entry.file_size(ec);
    out.extension = entry.path().extension().string();
    std::replace(out.extension.begin(), out.extension.end(), '\\', '/');
    auto time = entry.last_write_time(ec);
    if (!ec) out.modified_time = time.time_since_epoch().count();
    return out;
}

std::vector<FileInfo> FileEngine::list_directory(const fs::path& directory) const {
    const fs::path path = directory.empty() ? root_ : (directory.is_absolute() ? directory : root_ / directory);
    if (!safe_path(path)) return {};
    std::error_code ec;
    if (!fs::is_directory(path, ec)) return {};
    std::vector<FileInfo> result;
    for (fs::directory_iterator it(path, ec), end; !ec && it != end; it.increment(ec))
        result.push_back(describe(*it));
    return result;
}

std::vector<FileInfo> FileEngine::filter_files(std::vector<FileInfo> files, const FileQuery& query) const {
    auto lower = [](std::string value) {
        for (char& c : value) c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
        return value;
    };
    const std::string text = lower(query.text);
    files.erase(std::remove_if(files.begin(), files.end(), [&](const FileInfo& f) {
        if (!text.empty() && lower(f.name).find(text) == std::string::npos &&
            lower(f.path).find(text) == std::string::npos) return true;
        if (!query.extensions.empty()) {
            const std::string ext = lower(f.extension);
            bool match = false;
            for (auto wanted : query.extensions) {
                wanted = lower(std::move(wanted));
                if (!wanted.empty() && wanted.front() != '.') wanted.insert(wanted.begin(), '.');
                if (ext == wanted) { match = true; break; }
            }
            if (!match) return true;
        }
        return false;
    }), files.end());
    return files;
}

std::vector<FileInfo> FileEngine::search_files(const FileQuery& query) const {
    std::vector<FileInfo> all;
    const fs::path start = query.directory.value_or(root_);
    if (!safe_path(start)) return {};
    std::error_code ec;
    if (!fs::is_directory(start, ec)) return {};
    for (fs::recursive_directory_iterator it(start, ec), end; !ec && it != end; it.increment(ec))
        all.push_back(describe(*it));
    return filter_files(std::move(all), query);
}

std::optional<FileInfo> FileEngine::get_file_info(const fs::path& path) const {
    if (!safe_path(path)) return std::nullopt;
    std::error_code ec;
    const fs::path resolved = path.is_absolute() ? path : root_ / path;
    if (!fs::exists(resolved, ec)) return std::nullopt;
    return describe(fs::directory_entry(resolved, ec));
}

std::vector<FileInfo> FileEngine::sort_files(std::vector<FileInfo> files, SortOrder order) const {
    std::sort(files.begin(), files.end(), [&](const FileInfo& a, const FileInfo& b) {
        if (order == SortOrder::ModifiedDesc) return a.modified_time > b.modified_time;
        if (order == SortOrder::SizeDesc) return a.size > b.size;
        return a.name < b.name;
    });
    return files;
}

} // namespace filemind
