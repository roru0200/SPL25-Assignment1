#include "MP3Track.h"
#include <iostream>
#include <cmath>
#include <algorithm>

MP3Track::MP3Track(const std::string& title, const std::vector<std::string>& artists, 
                   int duration, int bpm, int bitrate, bool has_tags)
    : AudioTrack(title, artists, duration, bpm), bitrate(bitrate), has_id3_tags(has_tags) {

    std::cout << "MP3Track created: " << bitrate << " kbps" << std::endl;
}

// ========== TODO: STUDENTS IMPLEMENT THESE VIRTUAL FUNCTIONS ==========

void MP3Track::load() {
    std::cout << "[MP3Track::load] Loading MP3: \"" << title
              << "\" at " << bitrate << " kbps...\n";
    // TODO: Implement MP3 loading with format-specific operations
    // NOTE: Use exactly 2 spaces before the arrow (→) character
    if (has_id3_tags == true) {
        std::cout <<"  → Processing ID3 metadata (artist info, album art, etc.)...\n";
    }

    else {
        std::cout <<"  → No ID3 tags found.\n";
    }

    std::cout <<"  → Decoding MP3 frames...\n";
    std::cout <<"  → Load complete.\n";
}

void MP3Track::analyze_beatgrid() {
     std::cout << "[MP3Track::analyze_beatgrid] Analyzing beat grid for: \"" << title << "\"\n";
    // TODO: Implement MP3-specific beat detection analysis
    // NOTE: Use exactly 2 spaces before each arrow (→) character
    double beats = (duration_seconds / 60.0) * bpm; // estimated beats
    double precision_factor = bitrate / 320.0; // simulates how compression artifacts affect beat detection accuracy
}

double MP3Track::get_quality_score() const {
    // TODO: Implement comprehensive quality scoring
    // NOTE: This method does NOT print anything
    int score = (bitrate / 320.0) * 100.0; // calculating base score

    if (has_id3_tags) score += 5;
    if (bitrate < 128) score -= 10; // bonuses and penelties

    if (score < 0.0) return 0.0;
    if (score > 100.0) return 100;
    return score;                   // clamping score to [0,100]
}

PointerWrapper<AudioTrack> MP3Track::clone() const {
    // TODO: Implement polymorphic cloning
    MP3Track* clone = new MP3Track(*this); // the additonal fields are all primitive so the default is fine
    return PointerWrapper<AudioTrack>(clone);
}