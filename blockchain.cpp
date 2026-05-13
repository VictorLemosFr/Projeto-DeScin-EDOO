#include "blockchain.hpp"
#include <iostream>
#include <functional>

// ---------- Block ----------

Block::Block(int index, std::string data, std::string previousHash)
    : index(index),
      data(std::move(data)),
      previousHash(std::move(previousHash)),
      block_hash(0),
      nonce(0)
{
    block_hash = generate_hash();
}

std::size_t Block::generate_hash() const {
    std::string toHash = std::to_string(index) + data + previousHash + std::to_string(nonce);
    return std::hash<std::string>{}(toHash);
}

void Block::mine() {
    while (block_hash % 2 != 0) {
        nonce++;
        block_hash = generate_hash();
    }
}

int Block::get_index() const            { return index; }
std::string Block::get_data() const     { return data; }
std::string Block::get_previousHash() const { return previousHash; }
std::size_t Block::get_hash() const     { return block_hash; }

// ---------- BlockChain ----------

BlockChain::BlockChain() {
    chain.emplace_back(0, "Genesis Block", "0");
    chain.back().mine(); // mineração também pro gênesis, pra ele passar no verify
}

void BlockChain::add_block(const std::string& data) {
    int index = static_cast<int>(chain.size());
    std::string previousHash = std::to_string(chain.back().get_hash());
    chain.emplace_back(index, data, previousHash);
    chain.back().mine();
}

void BlockChain::display_chain() const {
    for (int i = static_cast<int>(chain.size()) - 1; i >= 0; i--) {
        std::cout << "Block " << chain[i].get_index() << ": " << chain[i].get_data() << "\n";
        std::cout << "Previous Hash: " << chain[i].get_previousHash() << "\n";
        std::cout << "Hash: " << chain[i].get_hash() << "\n";
        std::cout << "-------------------------\n";
    }
}

bool BlockChain::verify_chain() const {
    if (chain[0].get_previousHash() != "0"
        || chain[0].get_hash() != chain[0].generate_hash()
        || chain[0].get_hash() % 2 != 0)
    {
        std::cout << "Chain is invalid at genesis block!\n";
        return false;
    }

    for (std::size_t i = 1; i < chain.size(); i++) {
        if (chain[i].get_previousHash() != std::to_string(chain[i-1].get_hash())
            || chain[i].get_hash() != chain[i].generate_hash()
            || chain[i].get_hash() % 2 != 0)
        {
            std::cout << "Chain is invalid at block " << i << "\n";
            return false;
        }
    }
    std::cout << "Chain is valid!\n";
    return true;
}

void BlockChain::hack_chain() {
    if (chain.size() > 1) {
        chain[1] = Block(chain[1].get_index(), "Hacked Data", chain[1].get_previousHash());
        std::cout << "Chain has been hacked!\n";
    }
}

std::vector<Block> BlockChain::get_latest_blocks(int count) {
    std::vector<Block> latestBlocks;
    for (int i = static_cast<int>(chain.size()) - 1; i >= 0 && count > 0; i--, count--) {
        latestBlocks.push_back(chain[i]);
    }
    return latestBlocks;
}
