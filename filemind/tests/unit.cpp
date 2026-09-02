#include "filemind/context_engine.hpp"
#include "filemind/file_engine.hpp"
#include "filemind/intent_parser.hpp"
#include "filemind/runtime.hpp"

#include <cassert>
#include <filesystem>
#include <fstream>

using namespace filemind;
namespace fs = std::filesystem;

int main() {
    ContextEngine context;
    context.add_user_message("historial antiguo");
    context.add_assistant_message("respuesta");
    const auto prompt = context.build_prompt("Busca mis archivos .cpp");
    assert(prompt.size() <= ContextEngine::max_prompt_bytes);
    assert(prompt.find(".cpp") != std::string::npos);
    context.clear();
    assert(context.build_prompt("actual") == "actual");

    IntentParser parser;
    assert(parser.parse(R"({"action":"search_files","query":"Linux"})").action == IntentAction::SearchFiles);
    assert(parser.parse(R"({"action":"list_directory","path":"."})").action == IntentAction::ListDirectory);
    assert(parser.parse(R"({"action":"file_info","path":"a.txt"})").action == IntentAction::FileInfo);
    assert(parser.parse("respuesta normal").action == IntentAction::Unknown);
    assert(parser.parse_user_message("Busca mis archivos .cpp").action == IntentAction::SearchFiles);

    const fs::path root = fs::temp_directory_path() / "filemind_unit_root";
    fs::remove_all(root);
    fs::create_directories(root / "nested");
    std::ofstream(root / "main.cpp") << "int main() {}";
    std::ofstream(root / "note.txt") << "nota";
    FileEngine engine(root);
    FileQuery query;
    query.extensions = {".cpp"};
    const auto found = engine.search_files(query);
    assert(found.size() == 1 && found.front().extension == ".cpp");
    assert(engine.search_files(FileQuery{.text = "missing"}).empty());
    assert(engine.list_directory().size() == 3);
    assert(!engine.get_file_info("../outside.txt").has_value());
    fs::remove_all(root);

    FileMindRuntime runtime;
    assert(!runtime.initialize("missing-model.mai3"));
    assert(!runtime.is_ready());
    runtime.shutdown();
    runtime.shutdown();
    return 0;
}
