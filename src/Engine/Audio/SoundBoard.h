#pragma once
#include <vector>
#include <unordered_map>
#include <SDL3_mixer/SDL_mixer.h>
#include "../Enums.h"

class SoundBoard {
public:
    SoundBoard(std::vector<SFX> init_sfx);
    ~SoundBoard();

    bool CheckSFX(SFX s);

    void PlaySFX(SFX s);
    void PlaySFXPitchShift(SFX s);

private:
    MIX_Mixer* mixer = nullptr;
    MIX_Track* sfx_track = nullptr;
    std::unordered_map<SFX, MIX_Audio*> sfx;
    const unsigned int SEC = 1000; //1000 Milliseconds
};