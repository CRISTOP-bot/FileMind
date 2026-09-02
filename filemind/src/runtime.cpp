#include "filemind/runtime.hpp"
#include "filemind/orchestrator.hpp"

#include <filesystem>
#include <memory>
#include <utility>

namespace filemind {

class FileMindRuntime::Impl {
public:
    explicit Impl(std::string root)
        : orchestrator(root.empty() ? std::filesystem::current_path() : std::filesystem::path(std::move(root))) {}
    FileMindOrchestrator orchestrator;
};

FileMindRuntime::FileMindRuntime(std::string root) : impl_(new Impl(std::move(root))) {}
FileMindRuntime::~FileMindRuntime() { delete impl_; }

bool FileMindRuntime::initialize(const std::string& checkpoint) {
    return impl_ && impl_->orchestrator.initialize(checkpoint);
}

std::string FileMindRuntime::process(std::string_view message) {
    return impl_ ? impl_->orchestrator.process(message) : std::string{};
}

bool FileMindRuntime::is_ready() const noexcept {
    return impl_ && impl_->orchestrator.is_ready();
}

void FileMindRuntime::shutdown() {
    if (impl_) impl_->orchestrator.shutdown();
}

} // namespace filemind
