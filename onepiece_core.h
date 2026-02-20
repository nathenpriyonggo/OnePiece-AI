#ifndef ONEPIECE_CORE_H
#define ONEPIECE_CORE_H

#include <string>
#include <vector>
#include <map>
#include <memory>


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
    Card leader_card;

    std::vector<Card> hand;
    std::vector<Card> deck;
    std::vector<Card> trash;
    std::vector<Card> field;
    std::vector<Card> life;
    
    int don_deck_count = 0;
    int don_active_count = 0;
    int don_rested_count = 0;
    int don_attached_count = 0;

    void extract_leader();
    void shuffle_deck();
    void draw_cards(int num_to_draw);
    void refresh_phase();
    void add_active_don(int amount);
    void add_rested_don(int amount);
};


class GameEngine {
public:
    PlayerState p1;
    PlayerState p2;
    int turn_count;
    int current_player_index; // 0 for p1, 1 for p2

    GameEngine();
    PlayerState& get_current_player();
    void execute_turn();
};

#endif // ONEPIECE_CORE_H