#pragma once
#include <unordered_map>
#include <SDL3_mixer/SDL_mixer.h>
#include "../Enums.h"

class DJ {
public:
	DJ();
	//Assumedly, when ~Engine calls MIX_Quit(), it frees all the Mixer-related resources
	~DJ() { songs.clear(); }

	//Check if a song exists
	bool CheckSong(const Song s);
	//What song is currently playing?
	Song CurrSong();

	//Play a song
	void PlaySong(const Song s, const char loop_num = 0, float fadein = 0.f);
	//Pause the currently playing song
	inline void PauseSong() { MIX_PauseTrack(msc_track); }
	//Stop the currently playing song
	inline void StopSong(float fadeout = 0.f) {
		if (MIX_TrackPaused(msc_track)) MIX_StopTrack(msc_track, 0);
		else MIX_StopTrack(msc_track, MIX_TrackMSToFrames(msc_track, fadeout*SEC));
	}

	//Where in the song are we (in minutes)?
	[[nodiscard]] inline float GetSongPos() { return MIX_TrackFramesToMS(msc_track, MIX_GetTrackPlaybackPosition(msc_track))/SEC/60; }

	//Volume
	[[nodiscard]] inline float GetVolume() const  { return msc_volume; }
	void SetVolume(const float n_v);

private:
	MIX_Mixer* mixer = nullptr;
	MIX_Track* msc_track = nullptr;
	std::unordered_map<Song, MIX_Audio*> songs;
	const unsigned int SEC = 1000; //1000 milliseconds
	float msc_volume = 100.f;
};