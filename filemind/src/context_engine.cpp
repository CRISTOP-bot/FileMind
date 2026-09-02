#include "filemind/context_engine.hpp"

#include <algorithm>

namespace filemind {

void ContextEngine::add_user_message(std::string_view message) {
    history_.push_back({true, std::string(message)});
}

void ContextEngine::add_assistant_message(std::string_view message) {
    history_.push_back({false, std::string(message)});
}

std::string ContextEngine::build_prompt(std::string_view current_message) const {
    // The current request always wins. History is only included if bytes remain.
    const std::size_t n = std::min(max_prompt_bytes, current_message.size());
    std::string prompt(current_message.substr(current_message.size() - n));
    if (prompt.size() == max_prompt_bytes)
        return prompt;

    for (auto it = history_.rbegin(); it != history_.rend(); ++it) {
        const std::string marker = it->user ? "U:" : "A:";
        const std::size_t available = max_prompt_bytes - prompt.size();
        if (available <= marker.size())
            break;
        const std::size_t take = std::min(it->text.size(), available - marker.size());
        std::string part = marker + it->text.substr(it->text.size() - take);
        if (part.size() > available)
            part.resize(available);
        prompt = part + prompt;
        if (prompt.size() == max_prompt_bytes)
            break;
    }
    return prompt;
}

void ContextEngine::clear() { history_.clear(); }

} // namespace filemind
