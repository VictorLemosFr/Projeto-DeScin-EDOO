# 🖥️ Grad Hacking

> Um idle/clicker game de terminal em C++ onde você constrói um
> império hacker, gera Cintokens com máquinas infectadas e mantém
> uma blockchain interna que registra cada ação do seu reinado.

---

## 📖 Sobre o jogo

Em **Grad Hacking**, você assume o papel de um hacker iniciante
com 500 Cintokens no bolso e ambições maiores. Cada máquina
infectada trabalha 24/7 gerando moeda — mas a complexidade
operacional cresce com você: o próximo alvo sempre custa mais
caro que o anterior.

Toda decisão que você toma é gravada em uma **blockchain interna**,
um livro-razão imutável que mantém o histórico completo da sua
operação. A integridade da cadeia é verificável a qualquer momento:
se algo foi alterado, o jogo detecta.

## 🎮 Como jogar

|                        GRAD HACKING INTERFACE                        |
|______________________________________________________________________|
| Balance: 1847                                                        |
| Infection Cost: 484                                                  |
Computers Infected: 4Recent Actions:- Infectou uma máquina! Total infectadas: 4- Infectou uma máquina! Total infectadas: 3- Infectou uma máquina! Total infectadas: 2
Commands:[i] Infectar maquina[q] Sair do jogo
______________________________________________________________________

### Controles

| Tecla | Ação                                          |
|-------|-----------------------------------------------|
| `i`   | Infectar uma nova máquina (consome Cintokens) |
| `q`   | Sair do jogo                                  |

### Mecânicas

- **Produção passiva:** cada máquina infectada gera 1 Cintoken por segundo
- **Curva de custo:** o preço da próxima infecção cresce 10% a cada compra
- **Log auditável:** toda infecção é registrada como um bloco na cadeia
- **Mineração:** cada bloco passa por uma prova de trabalho simples antes de ser aceito

## 🧱 Arquitetura

O projeto é dividido em três módulos com responsabilidades bem separadas:

### `blockchain` — A cadeia
Implementa as classes `Block` e `BlockChain`. Cada bloco contém
índice, dados, hash do bloco anterior, hash próprio e nonce.
A cadeia inicia com um bloco gênesis e cresce com cada ação
registrada. Inclui:

- Geração de hash baseada em `std::hash`
- Mineração com prova de trabalho (regra de paridade do hash)
- Verificação completa da integridade da cadeia
- Função `hack_chain()` para demonstrar detecção de adulteração

### `game` — O loop
Contém toda a lógica do jogo:

- **Estado:** saldo, taxa de produção, máquinas infectadas, custo de infecção
- **Loop principal:** update → render → input a 60 FPS, com delta-time real via `std::chrono::steady_clock`
- **Renderização:** interface ANSI no terminal com cores condicionais (verde quando há saldo suficiente para infectar, vermelho caso contrário)
- **Integração com blockchain:** cada ação relevante vira um bloco no `recordLog`

### `main` — Ponto de entrada
Instancia o jogo e dispara o loop.

## 🛠️ Compilação

Requer compilador com suporte a **C++17**.

```bash
g++ -std=c++17 -Wall -Wextra main.cpp game.cpp blockchain.cpp -o jogo.exe
./jogo.exe
```

### Plataforma
Atualmente Windows-only (usa `<conio.h>` e `<windows.h>` para
input não-bloqueante e controle do cursor). Portabilidade para
Linux/macOS está no roadmap.

## 🎯 Conceitos de C++/POO aplicados

Este projeto explora na prática:

- **Classes e encapsulamento:** `Block` e `BlockChain` com interface pública mínima e estado privado protegido
- **Composição:** `Game` contém um `BlockChain` como membro
- **Const-correctness:** getters e métodos puros marcados como `const`
- **Move semantics:** uso de `std::move` no construtor de `Block` para evitar cópias desnecessárias
- **STL idiomática:** `std::vector`, `std::string`, `std::hash`, `std::chrono`
- **Separação de interface e implementação:** `.hpp`/`.cpp` para cada módulo
- **Game loop com delta-time:** lógica de jogo desacoplada do framerate

## 🗺️ Roadmap

Próximas funcionalidades planejadas:

- [ ] **Sistema de upgrades permanentes** (multiplicadores de produção, redutores de custo)
- [ ] **Múltiplos tipos de alvo** (PCs domésticos, servidores corporativos, mainframes)
- [ ] **Save/load de partida** com verificação de integridade da blockchain salva
- [ ] **Eventos aleatórios** (detecção pelo FBI, zero-days, blackouts)
- [ ] **Verificação manual da cadeia** como ação de jogo (`v`) com bônus de auditoria
- [ ] **Win condition** (objetivo final + ranking de tempo)
- [ ] **Portabilidade Linux/macOS**
- [ ] **Hash criptográfica real** (SHA-256 via `picosha2`)

## ⚠️ Limitações conhecidas

Algumas escolhas foram intencionalmente didáticas:

- A prova de trabalho usa regra simples (hash par) em vez de N zeros à esquerda
- `std::hash` não é uma função criptográfica — é adequada para hash tables, não para resistência a colisão real
- A função `hack_chain()` existe para fins demonstrativos

Estas limitações estão documentadas no roadmap para evolução futura.

## 👤 Autor

**Victor [seu sobrenome]**
Sistemas de Informação @ CIn/UFPE

Projeto desenvolvido durante a disciplina de Estruturas de Dados
Orientada a Objetos (EDOO), com foco em aplicar conceitos de POO
em um contexto lúdico e tematicamente coerente.

## 📄 Licença

MIT
