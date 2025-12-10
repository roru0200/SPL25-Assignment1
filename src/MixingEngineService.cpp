#include "MixingEngineService.h"
#include <iostream>
#include <memory>


/**
 * TODO: Implement MixingEngineService constructor
 */
MixingEngineService::MixingEngineService()
    : decks(), active_deck(1), auto_sync(false), bpm_tolerance(0)
{
    // Your implementation here
    decks[0] = nullptr;
    decks[1] = nullptr;
    std::cout << "[MixingEngineService] Initialized with 2 empty decks.\n";
}

/**
 * TODO: Implement MixingEngineService destructor
 */
MixingEngineService::~MixingEngineService() {
    // Your implementation here
    std::cout << "[MixingEngineService] Cleaning up decks....\n";
    for(AudioTrack* track : decks) {
        if(track != nullptr) {
            delete track;
            track = nullptr;
        }
    }
}


/**
 * TODO: Implement loadTrackToDeck method
 * @param track: Reference to the track to be loaded
 * @return: Index of the deck where track was loaded, or -1 on failure
 */
int MixingEngineService::loadTrackToDeck(const AudioTrack& track) {
    // Your implementation here
    std::cout << "\n=== Loading Track to Deck ===\n";
    PointerWrapper<AudioTrack> ptr = track.clone();
    AudioTrack* cloned = ptr.get(); 

    if(!cloned) {
        std::cout << "[ERROR] Track: \"" << track.get_title() << "\" failed to clone\n";
        return -1;
    }

    if(decks[0] == nullptr && decks[1] == nullptr) {
        decks[0] = cloned;

        active_deck = 0;
        // Log Deck Switch
        
        std::cout << "[Deck Switch] Target deck: 0\n";
        
        cloned->load();
        cloned->analyze_beatgrid();
        ptr.release();
        cloned = nullptr;
        std::cout << "[Sync BPM] Cannot sync - one of the decks is empty.\n";
        std::cout <<"[Load Complete] '"<<decks[0]->get_title()<<"' is now loaded on deck 0\n";
        active_deck = 0;
        std::cout << "[Active Deck] Switched to deck 0\n";
        return 0;
    }

    else {
        int target_deck = 1 - active_deck;
        std::cout << "\n[Deck Switch] Target deck: " << target_deck << "\n";

        if(decks[target_deck] != nullptr) {
            delete decks[target_deck];
            decks[target_deck] = nullptr;
        }

        cloned->load();
        cloned->analyze_beatgrid();

        if(decks[active_deck] && auto_sync && !can_mix_tracks(ptr)) {
            sync_bpm(ptr);
        }

        ptr.release();
        decks[target_deck] = cloned;

        std::cout << "[Load Complete] \"" << cloned->get_title() << "\" is now loaded on deck " << target_deck << "\n";
        //if(decks[active_deck]){
        //    std::cout << "[Unload] Unloading previous deck " << active_deck << " (" << decks[active_deck]->get_title() << ")\n";
        //    delete decks[active_deck];
        //    decks[active_deck] = nullptr;
        //}
        active_deck = target_deck;
        std::cout << "[Active Deck] Switched to deck " << target_deck << "\n";

        return target_deck;
    }
    return -1;
}

/**
 * @brief Display current deck status
 */
void MixingEngineService::displayDeckStatus() const {
    std::cout << "\n=== Deck Status ===\n";
    for (size_t i = 0; i < 2; ++i) {
        if (decks[i])
            std::cout << "Deck " << i << ": " << decks[i]->get_title() << "\n";
        else
            std::cout << "Deck " << i << ": [EMPTY]\n";
    }
    std::cout << "Active Deck: " << active_deck << "\n";
    std::cout << "===================\n";
}

/**
 * TODO: Implement can_mix_tracks method
 * 
 * Check if two tracks can be mixed based on BPM difference.
 * 
 * @param track: Track to check for mixing compatibility
 * @return: true if BPM difference <= tolerance, false otherwise
 */
bool MixingEngineService::can_mix_tracks(const PointerWrapper<AudioTrack>& track) const {
    // Your implementation here
    if(decks[active_deck] == nullptr) return false;
    
    if(!track) return false;
    
    if(std::abs(decks[active_deck]->get_bpm() - track->get_bpm()) <= bpm_tolerance) return true;

    return false; // Placeholder
}

/**
 * TODO: Implement sync_bpm method
 * @param track: Track to synchronize with active deck
 */
void MixingEngineService::sync_bpm(const PointerWrapper<AudioTrack>& track) const {
    // Your implementation here
    if(decks[active_deck] && track && track.get()) {
        int original_bpm = track->get_bpm();
        int deck_bpm = decks[active_deck]->get_bpm();
        int average_bpm = (original_bpm + deck_bpm)/2;
        track->set_bpm(average_bpm);
        std::cout << "[Sync BPM] Syncing BPM from " << original_bpm << " to " << average_bpm << "\n";
    }
}

