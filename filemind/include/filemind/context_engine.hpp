#pragma once

#include <cstddef>
#include <string>
#include <string_view>
#include <vector>

namespace filemind {

class ContextEngine {
public:
    // mini_ai currently has a 32-byte context. Keep room for a compact role marker.
    static constexpr std::size_t max_prompt_bytes = 32;

    void add_user_message(std::string_view message);
    void add_assistant_message(std::string_view message);
    std::string build_prompt(std::string_view current_message) const;
    void clear();

private:
    struct Message { bool user; std::string text; };
    std::vector<Message> history_;
};

} // namespace filemind
