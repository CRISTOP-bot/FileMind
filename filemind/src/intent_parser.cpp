#include "filemind/intent_parser.hpp"

#include <algorithm>
#include <cctype>
#include <regex>

namespace filemind {
namespace {
std::string lower(std::string value) {
    for (char& c : value) c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    return value;
}
std::string clean_extension(std::string ext) {
    if (!ext.empty() && ext.front() != '.') ext.insert(ext.begin(), '.');
    return ext;
}
}

Intent IntentParser::parse(std::string_view model_output) const {
    const std::string text(model_output);
    const std::string value = lower(text);
    Intent out;
    // Require an object-like output and one explicit action field. Do not infer
    // a tool from arbitrary prose containing an action name.
    if (value.find('{') == std::string::npos || value.find('}') == std::string::npos)
        return out;
    std::smatch match;
    const std::regex action_re(R"("action"\s*:\s*"([a-z_]+)")", std::regex::icase);
    if (!std::regex_search(text, match, action_re)) return out;
    const std::string action = lower(match[1].str());
    if (action == "search_files") out.action = IntentAction::SearchFiles;
    else if (action == "list_directory") out.action = IntentAction::ListDirectory;
    else if (action == "file_info") out.action = IntentAction::FileInfo;
    else if (action == "read_file") out.action = IntentAction::ReadFile;
    else if (action == "sort_files") out.action = IntentAction::SortFiles;
    else return out;

    // Only accept simple, bounded key/value fields; arbitrary output is not a path.
    const std::regex query_re(R"("query"\s*:\s*"([^"]{0,160})")", std::regex::icase);
    const std::regex path_re(R"("path"\s*:\s*"([^"]{1,300})")", std::regex::icase);
    if (std::regex_search(text, match, query_re)) out.query = match[1].str();
    if (std::regex_search(text, match, path_re)) out.path = match[1].str();
    if (out.action == IntentAction::FileInfo || out.action == IntentAction::ReadFile ||
        out.action == IntentAction::ListDirectory)
        if (out.path.empty()) return {};
    return out;
}

Intent IntentParser::parse_user_message(std::string_view user_message) const {
    const std::string original(user_message);
    const std::string text = lower(original);
    Intent out;
    if (text.find("busca") != std::string::npos || text.find("encuentra") != std::string::npos ||
        text.find("archivos") != std::string::npos) {
        out.action = IntentAction::SearchFiles;
        std::regex ext_re(R"((?:\.|extensi[oó]n\s*)?(cpp|hpp|h|c|pdf|txt|md)\b)", std::regex::icase);
        for (std::sregex_iterator it(original.begin(), original.end(), ext_re), end; it != end; ++it)
            out.extensions.push_back(clean_extension((*it)[1].str()));
        if (out.extensions.empty() && text.find("c++") != std::string::npos)
            out.extensions = {".cpp", ".hpp", ".h"};
        return out;
    }
    if (text.find("qué hay") != std::string::npos || text.find("lista") != std::string::npos) {
        out.action = IntentAction::ListDirectory;
        return out;
    }
    return out;
}

} // namespace filemind
