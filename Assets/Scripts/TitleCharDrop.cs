﻿using UnityEngine;
using System.Collections;

public class TitleCharDrop : MonoBehaviour {
	public Vector3 startPosition,endPosition;
	public float delay,currentTime,startTime;
	protected bool end = false, go = false;
	// Use this for initialization
	void Start () {
		startTime = Time.time;
		transform.localPosition = startPosition;
	}
	// Update is called once per frame
	void Update () {
		currentTime = Time.time - startTime;
		if (!end && !go && currentTime > delay) {
			startTime = Time.time;
			go = true;
			currentTime = Time.time - startTime;
		} 
		if (!end && go && transform.localPosition != endPosition) {
			float t = (currentTime);
			Vector3 lerpedPosition = Vector3.Lerp (startPosition, endPosition, Mathf.SmoothStep (0, 1, t));
			transform.localPosition = lerpedPosition;
		}
		if (!end && go && transform.localPosition == endPosition) {
			end = true;
		}
	}
}
