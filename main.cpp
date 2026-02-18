#include <iostream>
#include <fstream>
#include <sstream>
#include "onepiece_core.h"


// Function to load cards from a CSV file into a vector of Card structures
void load_cards(std::string filename, std::vector<Card>& cards) {
    std::ifstream file(filename);
    std::string line;

    // Skip the header row of the CSV
        std::getline(file, line);

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string value;
        Card c;

        std::getline(ss, value, ',');       // card_id          (not used)
        std::getline(ss, c.id, ',');        // card_code
        std::getline(ss, c.rarity, ',');    // card_rarity
        std::getline(ss, c.type, ',');      // card_type
        std::getline(ss, value, ',');       // card_image       (not used)

        // card_art_variant
        std::getline(ss, value, ',');
        if (std::stoi(value) != 0) {
            continue; // Skip this card if it's an art variant
        }

        std::getline(ss, c.name, ',');      // card_name
        std::getline(ss, c.color, ',');     // card_color
        std::getline(ss, value, ',');       // card_expansion   (not used)
        
        // card_power
        std::getline(ss, value, ','); 
        if (!value.empty() && std::isdigit(value[0])) {
            c.base_power = std::stoi(value);    
        } else {
            c.base_power = 0;
        }

        // card_cost
        std::getline(ss, value, ','); 
        if (!value.empty() && std::isdigit(value[0])) {
            c.cost = std::stoi(value);    
        } else {
            c.cost = 0;
        }

        // card_counter
        std::getline(ss, value, ','); 
        if (!value.empty() && std::isdigit(value[0])) {
            c.counter = std::stoi(value);    
        } else {
            c.counter = 0;
        }
        
        std::getline(ss, value, ',');       // card_effect      (not used)
        std::getline(ss, value, ',');       // card_trigger     (not used)
        std::getline(ss, value, ',');       // card_banned      (not used)

        cards.push_back(c);
    }
}

// Helper function to find a card in the database
Card find_card_by_id( const std::string& id, const std::vector<Card>& cards) {
    for (const auto& card : cards) {
        if (card.id == id) {
            return card;
        }
    }
    // Return an empty card if not found
    Card empty_card;
    empty_card.name = "NOT FOUND";
    return empty_card; 
}

// Function to load a player's deck from a list of card IDs
// Returns a vector of Card structures representing the player's deck
void load_deck(std::string input, std::vector<Card>& player_deck, const std::vector<Card>& cards) {
    std::ifstream file(input);
    std::string line;

    while (std::getline(file, line)) {
        // Line is like "4xOP01-016"

        // Find the 'x' separator
        size_t x_pos = line.find('x');
        if (x_pos == std::string::npos) {
            continue; // Invalid line format, skip
        }

        // Get the quantity
        int quantity = std::stoi(line.substr(0, x_pos)); 

        // Get the card ID
        std::string card_id = line.substr(x_pos + 1);
        Card found_card = find_card_by_id(card_id, cards);

        if (found_card.name == "NOT FOUND") {
            std::cout << "Warning: Could not find card " << card_id << std::endl;
        } else {
            for (int i = 0; i < quantity; ++i) {
                player_deck.push_back(found_card);
            }
        }
    }
}


int main() {
    // Initialize the game engine
    GameEngine game;
    std::vector<Card> card_database;

    load_cards("OnePieceTCG_Cards.csv", card_database);

    std::cout << "Welcome to the One Piece Card Game!" << std::endl;
    std::cout << card_database.size() << " cards loaded from the database." << std::endl;
    // for (const auto& card : card_database) {
    //     std::cout << "Card Name: " << card.name << ", Type: " << card.type 
    //               << ", Color: " << card.color << ", Cost: " << card.cost 
    //               << ", Power: " << card.base_power << std::endl;
    // }

    // Load player decks
    std::cout << "Loading player decks..." << std::endl;
    load_deck("player1_deck.txt", game.p1.deck, card_database);

    // Shuffle decks and draw starting hands
    std::cout << "Shuffling decks and drawing starting hands..." << std::endl;
    game.p1.shuffle_deck();
    game.p1.draw_cards(5);

    std::cout << "Player 1 starting hand:" << std::endl;
    for (const auto& card : game.p1.hand) {
        std::cout << "- " << card.name << " (Cost: " << card.cost 
                  << ", Power: " << card.base_power << ")" << std::endl;
    }
    

    return 0;
}