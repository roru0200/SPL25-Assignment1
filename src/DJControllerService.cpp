#include "DJControllerService.h"
#include "MP3Track.h"
#include "WAVTrack.h"
#include <iostream>
#include <memory>

DJControllerService::DJControllerService(size_t cache_size)
    : cache(cache_size) {}
/**
 * TODO: Implement loadTrackToCache method
 */
int DJControllerService::loadTrackToCache(AudioTrack& track) {
    // Your implementation here 
    if(cache.contains(track.get_title())) {
        cache.get(track.get_title());
        return 1;
    }
    
    PointerWrapper<AudioTrack> clone_ptr = track.clone();

    if(!clone_ptr) {
        std::cout << "[ERROR] Track: \"" << track.get_title() << "\" failed to clone\n";
        return 0;
    }
    
    clone_ptr->load();
    clone_ptr->analyze_beatgrid();
    bool eviction = cache.put(std::move(clone_ptr)); ///

    if (eviction) return -1;
    return 0;
}

void DJControllerService::set_cache_size(size_t new_size) {
    cache.set_capacity(new_size);
}
//implemented
void DJControllerService::displayCacheStatus() const {
    std::cout << "\n=== Cache Status ===\n";
    cache.displayStatus();
    std::cout << "====================\n";
}

/**
 * TODO: Implement getTrackFromCache method
 */
AudioTrack* DJControllerService::getTrackFromCache(const std::string& track_title) { // this method DOES NOT transfer ownership
    // Your implementation here
    AudioTrack* ptr = cache.get(track_title);
    if (ptr) return ptr;
    return nullptr;
}
