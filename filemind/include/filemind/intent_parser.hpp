#pragma once

#include <filesystem>
#include <string>
#include <string_view>
#include <vector>

namespace filemind {

enum class IntentAction { SearchFiles, ListDirectory, FileInfo, ReadFile, SortFiles, Unknown };

struct Intent {
    IntentAction action = IntentAction::Unknown;
    std::string query;
    std::vector<std::string> extensions;
    std::filesystem::path path;
};

class IntentParser {
public:
    Intent parse(std::string_view model_output) const;
    // Conservative offline fallback for the current tiny model, which cannot yet
    // reliably emit structured tool calls.
    Intent parse_user_message(std::string_view user_message) const;
};

} // namespace filemind
