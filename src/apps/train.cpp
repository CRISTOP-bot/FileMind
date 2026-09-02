#include "mini_ai/mini_ai.hpp"

#include <fstream>
#include <iostream>
#include <iterator>
using namespace mini_ai;
// DOCUMENTACIÓN_AQUÍ: La consola reporta inicio/reanudación, train loss periódico y
// guardado final. Los fallos de entrada se muestran como errores y terminan con código 1.
int main(int argc, char **argv) {
    std::string file = argc > 1 ? argv[1] : "../data/train.txt",
                ck = argc > 2 ? argv[2] : "mini_ai.ckpt";
    std::ifstream in(file);
    if (!in) {
        std::cerr << "cannot open " << file << "\n";
        return 1;
    }
    std::string text((std::istreambuf_iterator<char>(in)), {});
    ByteTokenizer tok;
    auto ids = tok.encode(text);
    Config c;
    Dataset data(ids, c.seq, 42);
    Model model(c);
    // DOCUMENTACIÓN_AQUÍ: sólo se crea un modelo nuevo si el checkpoint no existe;
    // un checkpoint presente pero inválido es un error real y se reporta como FAIL.
    std::ifstream existing(ck, std::ios::binary);
    if (existing.good()) {
        try {
            model.load(ck);
            std::cout << "resumed " << ck << "\n";
        } catch (const std::exception &e) {
            std::cerr << "checkpoint error: " << e.what() << "\n";
            return 1;
        }
    } else {
        std::cout << "starting new model\n";
    }
    for (int step = 0; step < 100; step++) {
        float l = model.train_batch(data.sample(2));
        if (step % 10 == 0)
            std::cout << "step " << step << " loss " << l << "\n";
    }
    model.save(ck);
    std::cout << "saved " << ck << " (" << model.parameters() << " parameters)\n";
}
