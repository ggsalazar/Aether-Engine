#include "SoundBoard.h"
#include <iostream>

SoundBoard::SoundBoard(std::vector<SFX> init_sfx) {
    //Create the mixer & the track
    mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, nullptr);
    sfx_track = MIX_CreateTrack(mixer);

    //Load the desired sfx
    //for (auto s : init_sfx)
        //sfx.insert({s, MIX_LoadAudio(mixer, "../assets/SFX/"+SFXToString(s)+".mp3", false);
}

SoundBoard::~SoundBoard() {
    //Free all the resources
    for (auto& [_, s] : sfx) MIX_DestroyAudio(s);
    sfx.clear();

    //Also destroys all tracks
    MIX_DestroyMixer(mixer);
}

bool SoundBoard::CheckSFX(SFX s) {
    if (sfx.count(s) > 0) return true;

    std::cout << "SoundBoard::CheckSFX(): Checked for non-loaded SFX!\n";
    return false;
}

void SoundBoard::PlaySFX(SFX s) {
    if (CheckSFX(s)) {
        //Load the audio into the track & play it
        MIX_SetTrackAudio(sfx_track, sfx[s]);
        MIX_PlayTrack(sfx_track, 0);
    }
    else std::cout << "SoundBoard::PlaySFX(): Cannot play non-loaded SFX!\n";
}

void SoundBoard::PlaySFXPitchShift(SFX s) {
    if (CheckSFX(s)) {
        //Load the sound
        MIX_SetTrackAudio(sfx_track, sfx[s]);

        //Play given sound with slight pitch shift
        float pitch_shift = (rand() % 11) * .01f;
        if (rand() % 2) pitch_shift *= -1;
        //NEED TO TEST
        MIX_SetTrackFrequencyRatio(sfx_track, 1+pitch_shift);
        MIX_PlayTrack(sfx_track, 0);
        MIX_SetTrackFrequencyRatio(sfx_track, 1);
    }
    else std::cout << "SoundBoard::PlaySFXPitchShift(): Cannot play non-loaded SFX!\n";
}
