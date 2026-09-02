#include "filemind/mini_ai_service.hpp"

#include "mini_ai/model.hpp"
#include "mini_ai/tokenizer.hpp"

#include <algorithm>
#include <exception>
#include <utility>

namespace filemind {

MiniAIService::MiniAIService() = default;
MiniAIService::~MiniAIService() { shutdown(); }

bool MiniAIService::initialize(const std::string& checkpoint) {
    try {
        // Load before taking ownership, so a failed load never replaces a good model.
        auto candidate = std::make_unique<mini_ai::Model>();
        candidate->load(checkpoint);
        std::lock_guard lock(mutex_);
        model_ = std::move(candidate);
        return true;
    } catch (const std::exception&) {
        return false;
    }
}

std::string MiniAIService::generate(std::string_view prompt, std::size_t max_tokens) {
    std::lock_guard lock(mutex_);
    if (!model_ || prompt.empty() || max_tokens == 0)
        return {};
    try {
        mini_ai::ByteTokenizer tokenizer;
        const std::string input(prompt);
        const auto ids = tokenizer.encode(input);
        auto output_ids = model_->generate(ids, max_tokens, 0.8f);
        std::string output = tokenizer.decode(output_ids);
        // Model::generate retains the prompt suffix. Remove it for a service-style API.
        if (output.size() >= input.size() &&
            std::equal(input.begin(), input.end(), output.begin()))
            output.erase(0, input.size());
        return output;
    } catch (const std::exception&) {
        return {};
    }
}

bool MiniAIService::is_ready() const noexcept {
    std::lock_guard lock(mutex_);
    return static_cast<bool>(model_);
}

void MiniAIService::shutdown() {
    std::lock_guard lock(mutex_);
    model_.reset();
}

} // namespace filemind
