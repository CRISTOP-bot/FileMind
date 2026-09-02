#pragma once

#include <string>
#include <string_view>

namespace filemind {

// Stable façade for CLI, JNI and future hosts. Internals remain private.
class FileMindRuntime {
public:
    explicit FileMindRuntime(std::string root = {});
    ~FileMindRuntime();
    FileMindRuntime(const FileMindRuntime&) = delete;
    FileMindRuntime& operator=(const FileMindRuntime&) = delete;
    bool initialize(const std::string& checkpoint);
    std::string process(std::string_view message);
    bool is_ready() const noexcept;
    void shutdown();

private:
    class Impl;
    Impl* impl_;
};

} // namespace filemind
