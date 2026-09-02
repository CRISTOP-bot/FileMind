#pragma once

#include "filemind/context_engine.hpp"
#include "filemind/file_engine.hpp"
#include "filemind/intent_parser.hpp"
#include "filemind/mini_ai_service.hpp"

#include <filesystem>
#include <string>
#include <string_view>

namespace filemind {

class FileMindOrchestrator {
public:
    explicit FileMindOrchestrator(std::filesystem::path root = std::filesystem::current_path());
    bool initialize(const std::string& checkpoint);
    std::string process(std::string_view user_message);
    bool is_ready() const noexcept;
    void shutdown();

private:
    MiniAIService ai_;
    ContextEngine context_;
    FileEngine files_;
    IntentParser parser_;
    bool ready_ = false;
};

} // namespace filemind
