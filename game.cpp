#include "game.hpp"
#include <iostream>
#include <conio.h> // Para _kbhit() e _getch()
#include <iomanip> // Para std::setprecision
#include <windows.h> // Para manipulação do console (limpar tela, posicionar cursor)
#include <cmath>
#include <sstream> // Para std::stringstream

#define COLOR_RESET   "\033[0m"
#define COLOR_GREEN   "\033[32m"
#define COLOR_RED     "\033[31m"
#define COLOR_CYAN    "\033[36m"

// No topo de game.cpp, depois dos includes
static std::string short_hash(std::size_t hash) {
    std::stringstream ss;
    ss << std::hex << std::setw(8) << std::setfill('0') << (hash & 0xFFFFFFFF);
    return ss.str();
}

// Implementação dos métodos do Game
Game::Game() :
        balance(500),
        productionRate(5), // Exemplo: 5 Cintokens por segundo
        computersInfected(0),
        infectionCost(300), // Custo para infectar uma máquina
        isRunning(true),
        lastUpdateTime(std::chrono::steady_clock::now()) {
    // O construtor pode inicializar o blockchain com um bloco gênesis, se necessário
}

void Game::update(double dt) {
    // Atualiza o saldo com base na taxa de produção e no tempo decorrido
    balance += productionRate * dt;
}

void Game::render() {
    resetCursor(); // Limpa a tela
    
    // Topo da Interface
    std::cout << "____________________________________________" << "\n";
    std::cout << "|          GRAD HACKING INTERFACE          |" << "\n";
    std::cout << "|__________________________________________|" << "\n";

    // Status do Jogador e Informações
    if (balance >= infectionCost) {
        std::cout << COLOR_GREEN; // Verde para indicar que o jogador pode infectar
    } else {
        std::cout << COLOR_RED; // Vermelho para indicar que o jogador não tem saldo suficiente
    }
    std::cout << "| Balance: " << std::left << std::setw(31) << std::round(balance) << " |" << "\n";
    std::cout << "| Infection Cost: " << std::left << std::setw(24) << std::round(infectionCost) << " |" << "\n";
    std::cout << "| Computers Infected: " << std::left << std::setw(20) << computersInfected << " |" << "\n";
    std::cout << COLOR_RESET; // Reseta a cor

    renderBlockchain(); // Renderiza os blocos recentes do blockchain

    renderCommands();  // Renderiza os comandos disponíveis para o jogador
    
    std::cout << "|__________________________________________|" << "\n";
}

void Game::renderBlockchain() {
    constexpr int CONTENT_WIDTH = 40;
    constexpr int LOG_LINES = 3;
    
    // Cabeçalho da seção
    std::cout << "|------------------------------------------|" << "\n";
    std::cout << "| " << std::left << std::setw(CONTENT_WIDTH) 
              << "Recent Actions:" << " |" << "\n";
    
    auto latestBlocks = recordLog.get_latest_blocks(LOG_LINES);
    
    for (int i = 0; i < LOG_LINES; i++) {
        std::string lineContent;
        
        if (i < static_cast<int>(latestBlocks.size())) {
            const Block& b = latestBlocks[i];
            
            // Prefixo: "#NN [hash8] " 
            std::stringstream prefix;
            prefix << "#" << b.get_index() 
                   << " [" << short_hash(b.get_hash()) << "] ";
            std::string prefixStr = prefix.str();
            
            // Espaço restante pra mensagem
            int msgBudget = CONTENT_WIDTH - static_cast<int>(prefixStr.size());
            std::string msg = b.get_data();
            
            if (static_cast<int>(msg.size()) > msgBudget) {
                msg = msg.substr(0, msgBudget - 3) + "...";
            }
            
            lineContent = prefixStr + msg;
        }
        // else: linha vazia, mantém altura fixa
        
        std::cout << "| " << std::left << std::setw(CONTENT_WIDTH) 
                  << lineContent << " |" << "\n";
    }
}

void Game::renderCommands() {
  constexpr int CONTENT_WIDTH = 40;
    
    std::cout << "|------------------------------------------|" << "\n";
    std::cout << "| " << std::left << std::setw(CONTENT_WIDTH) 
              << "Commands:" << " |" << "\n";
    std::cout << "| " << std::left << std::setw(CONTENT_WIDTH) 
              << "  [i] Infectar maquina" << " |" << "\n";
    std::cout << "| " << std::left << std::setw(CONTENT_WIDTH) 
              << "  [q] Sair do jogo" << " |" << "\n";
}

void Game::handleInput() {
    if (_kbhit()) {
        char tecla = _getch();
        if (tecla == 'i') {
            if (balance >= infectionCost) { // Verifica se o jogador tem saldo suficiente para infectar uma máquina
            balance -= infectionCost; // Custo para infectar uma máquina
            infectionCost *= 1.1; // Aumenta o custo para a próxima máquina (exemplo: 10% de aumento)
            computersInfected += 1;
            productionRate += 1; // Cada máquina infectada aumenta a produção em 1 Cintoken por segundo
            recordLog.add_block("Infectou uma maquina! Total infectadas: " + std::to_string(computersInfected));
            }
        } else if(tecla == 'q') {
            isRunning = false; // Sai do jogo
        }
    }
}

void Game::run() {
    while (isRunning) {
        hideCursor(); // Esconde o cursor para uma melhor experiência visual
        auto currentTime = std::chrono::steady_clock::now();
        double deltaTime = std::chrono::duration<double>(currentTime - lastUpdateTime).count();
        lastUpdateTime = currentTime;

        update(deltaTime);
        render();
        handleInput();

        // Simula um atraso para limitar a taxa de atualização (por exemplo, 60 FPS)
        std::this_thread::sleep_for(std::chrono::milliseconds(16)); // Aproximadamente 16 ms para 60 FPS
    }
}

void Game::resetCursor() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD coord = { 0, 0 };
    SetConsoleCursorPosition(hOut, coord);
}

void Game::hideCursor() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hOut, &cursorInfo);
    cursorInfo.bVisible = FALSE; // Esconde o cursor
    SetConsoleCursorInfo(hOut, &cursorInfo);
}




