#include <iostream>
#include "DJ.h"
#include "../Math/Math.h"

DJ::DJ() {
	MIX_Init(); //Calls SDL_INIT_AUDIO

	//Create a mixer & the track
	mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, nullptr);
	msc_track = MIX_CreateTrack(mixer);

	//Initialize all of the game's tracks (the Engine has no tracks)
	//songs.insert({Song::<Songname>, MIX_LoadAudio(mixer, "path_to_file.mp3/ogg/etc", false)});
	//Repeat above line as necessary
}

void DJ::PlaySong(const Song s, const char loop_num, float fadein) {
	if (!CheckSong(s)) {
		std::cout << "DJ::PlaySong(): Trying to play non-existent song!\n";
		return;
	}

	//Will we be starting a new song?
	bool start_new_song = true;

	//Check to see if there's a song already playing
	if (MIX_TrackPlaying(msc_track)) {
		//If the song playing is the requested song, do nothing
		//Else, stop the playing song and play the new one
		start_new_song = MIX_GetTrackAudio(msc_track) != songs[s];
	}
	//Is the current song paused?
	else if (MIX_TrackPaused(msc_track)) {
		//If the paused song is the requested song, resume it
		if (MIX_GetTrackAudio(msc_track) == songs[s]) {
			MIX_ResumeTrack(msc_track);
			start_new_song = false;
		}
		//Else, we will start a new song
	}

	if (start_new_song) {
		MIX_StopTrack(msc_track, 0);
		MIX_SetTrackAudio(msc_track, songs[s]);
		SDL_PropertiesID options = SDL_CreateProperties();
		SDL_SetNumberProperty(options, MIX_PROP_PLAY_LOOPS_NUMBER, loop_num);
		//Clamp the length of the fadein to the length of the song in milliseconds
		Math::Clamp(fadein, 0.f, (float) MIX_AudioFramesToMS(songs[s], MIX_GetAudioDuration(songs[s])) / (float) SEC);
		SDL_SetNumberProperty(options, MIX_PROP_PLAY_FADE_IN_MILLISECONDS_NUMBER, SEC * fadein);
		MIX_PlayTrack(msc_track, options);
	}
}

void DJ::SetVolume(float n_v) {
	Math::Clamp(n_v, 0.f, 100.f);
	n_v /= 50.f;
	MIX_SetMasterGain(mixer, n_v);
}