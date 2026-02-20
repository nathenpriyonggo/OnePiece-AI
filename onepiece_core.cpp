#include "onepiece_core.h"
#include <iostream>
#include <random>
#include <algorithm>


// --- PLAYER STATE METHODS ---

void PlayerState::extract_leader() {
    for (auto it = deck.begin(); it != deck.end(); ++it) {
        if (it->rarity == "L") {
            leader_card = *it;
            deck.erase(it);
            break;
        }
    }
}

void PlayerState::shuffle_deck() {
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(deck.begin(), deck.end(), g);
}

void PlayerState::draw_cards(int num_to_draw) {
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

void PlayerState::refresh_phase() {
    don_active_count += don_rested_count + don_attached_count;
    don_rested_count = 0;
    don_attached_count = 0;

    for (auto& card : field) {
        card.is_rested = false;
        card.current_power = card.base_power; // Reset power to base power at the start of the turn
    }
}

void PlayerState::add_active_don(int amount) {
    for (int i = 0; i < amount; ++i) {
        if (don_deck_count > 0) {
            don_deck_count--;
            don_active_count++;
        }
    }
}

void PlayerState::add_rested_don(int amount) {
    for (int i = 0; i < amount; ++i) {
        if (don_deck_count > 0) {
            don_deck_count--;
            don_rested_count++;
        }
    }
}


// --- GAME ENGINE METHODS ---

GameEngine::GameEngine() {
    turn_count = 0;
    current_player_index = 0;
}

PlayerState& GameEngine::get_current_player() {
    return (current_player_index == 0) ? p1 : p2;
}

void GameEngine::execute_turn() {
    PlayerState& current_player = get_current_player();

    turn_count++;
    std::cout << "\n=== START OF TURN " << turn_count << " (Player " << (current_player_index + 1) << ") ===" << std::endl;
    
    // --- PHASE 1: Refresh Phase ---
    current_player.refresh_phase();
    std::cout << "[Refresh Phase] Cards and Don!! reset." << std::endl;

    // --- PHASE 2: Draw Phase ---
    if (turn_count == 1) {
        std::cout << "[Draw Phase] Skipped (First Turn Rule)." << std::endl;
    } else {
        current_player.draw_cards(1);
        std::cout << "[Draw Phase] Player drew 1 card." << std::endl;
    }

    // --- PHASE 3: DON!! Phase ---
    int don_to_add = 2;
    if (turn_count == 1) {
        don_to_add = 1;
    }

    current_player.add_active_don(don_to_add);
    std::cout << "[DON!! Phase] Player added " << don_to_add << " active Don!!." << std::endl;
    
    // --- PHASE 4: Main Phase ---
    bool turn_ended = false;
    bool show_status = true;
    while (!turn_ended) {
        // A. Show Status
        if (show_status) {
            std::cout << "\n--- Player Status ---" << std::endl;
            std::cout << ">> " << current_player.leader_card.name << " (ID: " 
                << current_player.leader_card.id << ")" << std::endl
                << "Hand: " << current_player.hand.size() << " cards" << std::endl
                << "Deck: " << current_player.deck.size() << " cards" << std::endl
                << "Life: " << current_player.life.size() << " cards" << std::endl
                << "Trash: " << current_player.trash.size() << " cards" << std::endl
                << "Active Don!!: " << current_player.don_active_count << std::endl
                << "Rested Don!!: " << current_player.don_rested_count << std::endl;
            
            std::cout << "\n--- Field Status ---" << std::endl;
            if (current_player.field.empty()) {
                std::cout << "No cards on the field." << std::endl;
            }
            int i = 1;
            for (const auto& card : current_player.field) {
                std::cout << "[" << i++ << "] ID: " << card.id 
                    << ", Card Name: " << card.name 
                    << ", Type: " << card.type 
                    << ", Color: " << card.color 
                    << ", Cost: " << card.cost 
                    << ", Power: " << card.base_power << std::endl;
            }

            std::cout << "\n--- Hand Status ---" << std::endl;

            i = 1; // Reuse index for hand display
            for (const auto& card : current_player.hand) {
                std::cout << "[" << i++ << "] ID: " << card.id 
                    << ", Card Name: " << card.name 
                    << ", Type: " << card.type 
                    << ", Color: " << card.color 
                    << ", Cost: " << card.cost 
                    << ", Power: " << card.base_power << std::endl;
            }
            show_status = false; // Only show status at the beginning of the main phase and after each action
        }

        std::cout << "\nActions: (1) Play Card, (2) Attach Don, (3) Attack, (4) Activate:Main (9) End Turn" << std::endl;
        std::cout << "Choose an action: ";

        // B. Get Player Input
        int action;
        if (!(std::cin >> action)) { // Catch if user types a letter
            std::cin.clear();
            std::cin.ignore(1000, '\n');
            continue;
        }

        // C. Process Input
        if (action == 1) {
            std::cout << "Choose a card to play (0 to cancel): ";

            int card_choice;
            if (!(std::cin >> card_choice)) { // Catch if user types a letter
                std::cin.clear();
                std::cin.ignore(1000, '\n');
                continue;
            } 
            else if (card_choice == 0) { // Cancel action
                continue; 
            } 
            else if (card_choice > current_player.hand.size()) {
                std::cout << "Invalid card choice." << std::endl;
            } 
            else {
                Card chosen_card = current_player.hand[card_choice - 1];
                current_player.hand.erase(current_player.hand.begin() + card_choice - 1);
                current_player.field.push_back(chosen_card);
                std::cout << "PLAYED: " << chosen_card.id << " \"" << chosen_card.name << "\"" << std::endl;

                show_status = true; // Show updated status after playing a card
            }
        } 
        else if (action == 2) {
            std::cout << "Attach Don action selected. (Not implemented yet)" << std::endl;
        } 
        else if (action == 3) {
            std::cout << "Attack action selected. (Not implemented yet)" << std::endl;
        } 
        else if (action == 4) {
            std::cout << "Activate:Main action selected. (Not implemented yet)" << std::endl;
        } 
        else if (action == 9) {
            turn_ended = true;
            show_status = true; // Show status at the start of the next turn
            std::cout << "Ending Turn..." << std::endl;
        } 
        else {
            std::cout << "Invalid action." << std::endl;
        }
    }

    // --- PHASE 5: End Phase ---
    current_player_index = (current_player_index == 0) ? 1 : 0; // Switch player for next turn
    std::cout << "\n=== END OF TURN " << turn_count 
        << " (Player " << (current_player_index + 1) 
        << ") ===" << std::endl;

    std::cout << "\n******************************************************************************" << std::endl;
}