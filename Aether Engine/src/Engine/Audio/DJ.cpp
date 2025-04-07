#include "DJ.h"


bool DJ::CheckTrack(const string track) {
	if (tracks.count(track) > 0) return true;
	else cout << "Track " << track << " does not exist!" << endl;
	return false;
}

bool DJ::LoadTrack(const string track, const float vol) {

	auto new_track = make_unique<Music>();
	if (new_track->openFromFile("assets/Music/" + track + ".mp3")) {
		new_track->setVolume(vol);
		tracks.insert(make_pair(track, move(new_track)));
		return true;
	}
	cout << "Failed to load track " << track << "!" << endl;
	return false;
}