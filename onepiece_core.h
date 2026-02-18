#ifndef ONEPIECE_CORE_H
#define ONEPIECE_CORE_H

#include <string>
#include <vector>
#include <map>
#include <memory>
#include <random>
#include <algorithm>

// Enumerations for fast checking
enum class Zone { DECK, HAND, LIFE, FIELD, TRASH, DON_DECK, DON_ACTIVE, DON_RESTED };

// Card structure to represent each card in the game
struct Card {
    std::string id;
    std::string name;
    int cost;
    int base_power;
    int current_power;
    int counter = 0;      // Default to 0
    std::string color;
    std::string type;
    std::string rarity;
    std::string description;

    bool is_rested = false;     // Default to false
    int attached_don = 0;       // How many don cards are under this card
};

// PlayerState structure to represent the state of each player in the game
struct PlayerState {
    int life_points;

    std::vector<Card> hand;
    std::vector<Card> deck;
    std::vector<Card> trash;
    std::vector<Card> field;
    std::vector<Card> life;
    
    int don_deck_count;
    int don_active_count;
    int don_rested_count;
    int don_attached_count;

    void shuffle_deck() {
        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(deck.begin(), deck.end(), g);
    }

    void draw_cards(int num_to_draw) {
        for (int i = 0; i < num_to_draw; ++i) {
            if (deck.empty()) {
                std::cout << "DECK EMPTY! Cannot draw." << std::endl;
                break;
            }

            Card drawn_card = deck.back();
            deck.pop_back();
            hand.push_back(drawn_card);
        }
    }

    void refresh_turn() {
        don_active_count += don_rested_count + don_attached_count;
        don_rested_count = 0;
        don_attached_count = 0;
    }
};


class GameEngine {
public:
    PlayerState p1;
    PlayerState p2;
    int turn_count;
    int current_player_index; // 0 for p1, 1 for p2

    GameEngine() {
        turn_count = 0;
        current_player_index = 0;
    }
};

#endif // ONEPIECE_CORE_H