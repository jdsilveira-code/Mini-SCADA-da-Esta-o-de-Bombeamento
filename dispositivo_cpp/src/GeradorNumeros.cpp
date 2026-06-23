#include "GeradorNumeros.hpp"
#include <chrono>

// Engine compartilhada entre todas as instâncias; seeded uma única vez com
// random_device XOR timestamp para garantir entropia mesmo em plataformas
// onde random_device é determinístico (ex.: MSVC no Windows).
static std::mt19937& sharedEngine() {
    static std::mt19937 engine(
        std::random_device{}() ^
        static_cast<unsigned>(
            std::chrono::high_resolution_clock::now().time_since_epoch().count()
        )
    );
    return engine;
}

GeradorAleatorio::GeradorAleatorio() = default;

int GeradorAleatorio::decimal(int minimo, int maximo) {
    std::uniform_int_distribution<int> dist(minimo, maximo);
    return dist(sharedEngine());
}
