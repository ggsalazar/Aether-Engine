#pragma once
#include <iostream>
#include <unordered_map>
#include <memory>

using namespace std;

class DJ {
public:
	DJ() = default;

	//Track manipulation
	bool CheckTrack(const string track);

	bool LoadTrack(const string track, const float vol = 100);
	void PlayTrack(string track) { if (CheckTrack(track)) tracks[track]->play(); }
	void PauseTrack(string track) {
		if (CheckTrack(track) and tracks[track]->getStatus() == SoundSource::Status::Playing)
			tracks[track]->pause();
	}
	void StopTrack(string track) { if (CheckTrack(track)) tracks[track]->stop(); }

	//Other
	void SetVolume(float n_v) { for (const auto& track : tracks) track.second->setVolume(n_v); }

private:
	unordered_map<string, unique_ptr<Music>> tracks;
};