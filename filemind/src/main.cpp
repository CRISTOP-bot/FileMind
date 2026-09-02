#include "filemind/runtime.hpp"

#include <iostream>

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Uso: mini_ai_filemind CHECKPOINT [consulta]\n";
        return 2;
    }
    filemind::FileMindRuntime app;
    if (!app.initialize(argv[1])) {
        std::cerr << "No se pudo cargar el checkpoint.\n";
        return 1;
    }
    const std::string query = argc >= 3 ? argv[2] : "Busca mis archivos .cpp";
    std::cout << app.process(query) << '\n';
    app.shutdown();
    return 0;
}
