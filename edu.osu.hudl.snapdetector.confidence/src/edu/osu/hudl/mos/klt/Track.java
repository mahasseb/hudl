package edu.osu.hudl.mos.klt;

import java.util.Vector;

public class Track {
	public long id;
	public String name;
	public Vector<Point> points;
	public int startFrame;
	public int endFrame;
	public double length;

	public Track() {
		points = new Vector<Point>();
	}

	@Override
	public String toString() {
		return "Id:" + id + " Name:" + name;
	}
}
