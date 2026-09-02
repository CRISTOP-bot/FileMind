#pragma once

#include <cstddef>
#include <memory>
#include <mutex>
#include <string>
#include <string_view>

namespace mini_ai { class Model; }

namespace filemind {

class MiniAIService {
public:
    MiniAIService();
    ~MiniAIService();
    MiniAIService(const MiniAIService&) = delete;
    MiniAIService& operator=(const MiniAIService&) = delete;

    bool initialize(const std::string& checkpoint);
    std::string generate(std::string_view prompt, std::size_t max_tokens);
    bool is_ready() const noexcept;
    void shutdown();

private:
    mutable std::mutex mutex_;
    std::unique_ptr<mini_ai::Model> model_;
};

} // namespace filemind
