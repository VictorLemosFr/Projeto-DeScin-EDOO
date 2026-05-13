#pragma once
#include "blockchain.hpp"
#include <chrono>
#include <thread>



class Game {
    private:
    // Atributos de estado
    double balance; // Saldo do jogador
    double productionRate; // Taxa de produção (Cintokens por segundo)
    double computersInfected; // Número de máquinas infectadas
    double infectionCost; // Custo para infectar uma máquina
    bool isRunning; // Flag para controlar o loop do jogo

    std::chrono::steady_clock::time_point lastUpdateTime; // Tempo do último update
    

    // Sistema de registro (blockchain)
    BlockChain recordLog;

    // Métodos Internos (Auxiliares)
    void update(double dt);
    void render();
    void handleInput();
    void resetCursor();
    void hideCursor();
    void renderBlockchain();
    void renderCommands();

    public:
    Game();
    void run();

};