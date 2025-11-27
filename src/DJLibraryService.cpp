#include "DJLibraryService.h"
#include "SessionFileParser.h"
#include "MP3Track.h"
#include "WAVTrack.h"
#include <iostream>
#include <memory>
#include <filesystem>


DJLibraryService::DJLibraryService(const Playlist& playlist) 
    : playlist(playlist), library() {}
/**
 * @brief Load a playlist from track indices referencing the library
 * @param library_tracks Vector of track info from config
 */
void DJLibraryService::buildLibrary(const std::vector<SessionConfig::TrackInfo>& library_tracks) {
    //Todo: Implement buildLibrary method
    std::cout << "TODO: Implement DJLibraryService::buildLibrary method\n"<< library_tracks.size() << " tracks to be loaded into library.\n";
    for(SessionConfig::TrackInfo track : library_tracks) {
        AudioTrack* song;

        if(track.type == "WAV") {
            song = new WAVTrack(track.title, track.artists, track.duration_seconds, track.bpm, track.extra_param1, track.extra_param2);
        }

        if(track.type == "MP3") {
            song = new MP3Track(track.title, track.artists, track.duration_seconds, track.bpm, track.extra_param1, track.extra_param2);
        }

        library.push_back(song);
    }
    std::cout << "[INFO] Track library built: " << library.size() << " tracks loaded\n";
}

/**
 * @brief Display the current state of the DJ library playlist
 * 
 */
void DJLibraryService::displayLibrary() const {
    std::cout << "=== DJ Library Playlist: " 
              << playlist.get_name() << " ===" << std::endl;

    if (playlist.is_empty()) {
        std::cout << "[INFO] Playlist is empty.\n";
        return;
    }

    // Let Playlist handle printing all track info
    playlist.display();

    std::cout << "Total duration: " << playlist.get_total_duration() << " seconds" << std::endl;
}

/**
 * @brief Get a reference to the current playlist
 * 
 * @return Playlist& 
 */
Playlist& DJLibraryService::getPlaylist() {
    // Your implementation here
    return playlist;
}

/**
 * TODO: Implement findTrack method
 * 
 * HINT: Leverage Playlist's find_track method
 */
AudioTrack* DJLibraryService::findTrack(const std::string& track_title) {
    return playlist.find_track(track_title);
}

void DJLibraryService::loadPlaylistFromIndices(const std::string& playlist_name, 
                                               const std::vector<int>& track_indices) { // bad design choice for the playlist, who initializes the playlist field? why is it not a pointer?
    // Your implementation here
    std::cout << "[INFO] Loading playlist: " << playlist_name << "\n";
    Playlist new_playlist(playlist_name);
    playlist = new_playlist;
    for (int i : track_indices) {
        if (i < 0 || i > library.size()){
            std::cout << "[WARNING] Invalid track index: "<< i <<"\n";
        }
        else {
            AudioTrack* current_track = library[i - 1];
            PointerWrapper<AudioTrack> clone = current_track->clone();
            if (!clone){
                std::cout << "[ERROR] Track: \"" << current_track->get_title() << "\" failed to clone";
                continue;
            } 
            clone->load();
            clone->analyze_beatgrid();
            playlist.add_track(clone.get());
            std::cout << "Added '" <<   clone->get_title() <<"' to playlist '"<< playlist_name << "'\n";
            clone.release();
        }
    }
    std::cout << "[INFO] Playlist loaded: " << playlist_name << " ("<< playlist.get_track_count() << " tracks)\n";
    // For now, add a placeholder to fix the linker error
    (void)playlist_name;  // Suppress unused parameter warning
    (void)track_indices;  // Suppress unused parameter warning
}
/**
 * TODO: Implement getTrackTitles method
 * @return Vector of track titles in the playlist
 */
std::vector<std::string> DJLibraryService::getTrackTitles() const {
    // Your implementation here
    std::vector<std::string> titles;
    std::vector<AudioTrack*> tracks = playlist.getTracks();

    for (AudioTrack* track : tracks) {
        titles.push_back(track->get_title());
        track = nullptr; //prevents data loss, the playlist still owns the tracks
    }

    return titles;
}
