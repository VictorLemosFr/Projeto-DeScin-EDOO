#pragma once

#include <string>
#include <vector>
#include <cstddef>

class Block {
private:
    int index;
    std::string data;
    std::string previousHash;
    std::size_t block_hash;
    long nonce;

public:
    Block(int index, std::string data, std::string previousHash);

    std::size_t generate_hash() const;
    void mine();

    int get_index() const;
    std::string get_data() const;
    std::string get_previousHash() const;
    std::size_t get_hash() const;
};

class BlockChain {
private:
    std::vector<Block> chain;

public:
    BlockChain();

    void add_block(const std::string& data);
    void display_chain() const;
    bool verify_chain() const;
    void hack_chain();
    std::vector<Block> get_latest_blocks(int count);
};