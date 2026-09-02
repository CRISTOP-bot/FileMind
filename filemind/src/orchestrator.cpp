#include "filemind/orchestrator.hpp"

#include <algorithm>
#include <sstream>
#include <utility>

namespace filemind {

FileMindOrchestrator::FileMindOrchestrator(std::filesystem::path root) : files_(std::move(root)) {}

bool FileMindOrchestrator::initialize(const std::string& checkpoint) {
    ready_ = ai_.initialize(checkpoint);
    return ready_;
}

std::string FileMindOrchestrator::process(std::string_view user_message) {
    if (!ready_ || user_message.empty()) return "FileMind no está listo.";
    const std::string prompt = context_.build_prompt(user_message);
    const std::string model_output = ai_.generate(prompt, 48);
    Intent intent = parser_.parse(model_output);
    // The current mini_ai is a byte LM, not a reliable structured-output model.
    // Use a bounded deterministic fallback so the MVP remains safe and usable.
    if (intent.action == IntentAction::Unknown)
        intent = parser_.parse_user_message(user_message);

    std::vector<FileInfo> results;
    if (intent.action == IntentAction::SearchFiles) {
        FileQuery query;
        query.text = intent.query;
        query.extensions = intent.extensions;
        results = files_.search_files(query);
    } else if (intent.action == IntentAction::ListDirectory) {
        results = files_.list_directory(intent.path);
    } else if (intent.action == IntentAction::FileInfo) {
        if (auto info = files_.get_file_info(intent.path)) results.push_back(*info);
    }

    std::ostringstream response;
    if (!results.empty() || intent.action == IntentAction::SearchFiles || intent.action == IntentAction::ListDirectory) {
        response << "Encontré " << results.size() << " archivo" << (results.size() == 1 ? "" : "s") << ".";
        const std::size_t shown = std::min<std::size_t>(results.size(), 8);
        for (std::size_t i = 0; i < shown; ++i) response << "\n- " << results[i].name;
    } else if (intent.action == IntentAction::Unknown) {
        response << "No reconocí una operación de archivos segura para esa solicitud.";
    } else {
        response << "No encontré ese archivo.";
    }

    const std::string answer = response.str();
    context_.add_user_message(user_message);
    context_.add_assistant_message(answer);
    return answer;
}

bool FileMindOrchestrator::is_ready() const noexcept { return ready_ && ai_.is_ready(); }

void FileMindOrchestrator::shutdown() {
    ready_ = false;
    context_.clear();
    ai_.shutdown();
}

} // namespace filemind
