package edu.osu.hudl.mos.klt;

import java.util.Vector;

public class Video {
	public Vector<Track> tracks;
	Vector<Integer> localmaximums;

	public Video() {
		tracks = new Vector<Track>();
		localmaximums = new Vector<Integer>();
	}
}
