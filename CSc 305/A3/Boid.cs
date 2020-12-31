<<<<<<< HEAD
﻿using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Boid : MonoBehaviour{
	
	private float collAv = 5.0f;//collision avoidance
	private float velMa = 5.0f;//vellocity matching
	private float cent = 2.0f;//centering
	private float maxSpeed = 1f;
	private float maxForce = 0.025f;
	
	public Vector3 pos;//position
	public Vector3 vel;//velocity
	public Vector3 acc;//acceleration
	
	public Boid(Vector3 hold){
		acc = new Vector3(0, 0, 0);
		vel = new Vector3(0.01f, 0.01f, 0.01f);
		pos = hold;//new Vector3(x, y, z);
	}
	
	public void run(List<Boid> boids){
		flock(boids);// compares state of boids with other boids
		update();
	}
	
	void applyForce(Vector3 force){
		acc += force;
	}
	
	void flock(List<Boid> boids){
		Vector3 collAv2 = separate(boids);
		Vector3 velMa2 = align(boids);
		Vector3 cent2 = cohesion(boids);
		
		collAv2 = collAv2 * collAv;
		velMa2 = velMa2 * velMa;
		cent2 = cent2 * cent;
		
		applyForce(collAv2);
		applyForce(velMa2);
		applyForce(cent2);
	}
	
	void update(){
		vel += acc;
		
		vel = vel/maxSpeed;
		
		pos += vel; 
		
		acc = acc * 0;
	}
	
	Vector3 seek(Vector3 target){
		Vector3 desired = target - pos;
		
		desired.Normalize();
		
		desired = desired * maxSpeed;
		
		Vector3 steer = desired - vel;
		
		steer = steer*maxForce;
		
		return steer;
	}
	
	Vector3 separate(List<Boid> boids){
		float desiredseperation = 25.0f;
		Vector3 steer = new Vector3(0,0,0);
		int count = 0;
		
		for(int i = 0; i < boids.Count; i++){
			float d = Vector3.Distance(pos, boids[i].pos);
			
			if((d > 0) && (d < desiredseperation)){
				
				Vector3 diff = pos - boids[i].pos;
				diff.Normalize();
				
				diff = diff/d;
				
				steer += diff;
				
				count++;
			}
		}
		
		if(count > 0){
			steer = steer/count;
			
			steer.Normalize();
			steer = steer * maxSpeed;
			steer = steer - vel;
			
			steer = steer*maxForce;
		}
		return steer;
	}
	
	Vector3 align(List<Boid> boids){
		float neighbordist = 5.0f;
		
		Vector3 steer = new Vector3();
		int count = 0;
		
		for(int i = 0; i < boids.Count; i++){
			float d = Vector3.Distance(pos, boids[i].pos);
			
			if((d > 0) && (d < neighbordist)){
				steer = steer + boids[i].vel;
				count++;
			}
		}
		
		if(count > 0){
			steer = steer/count;
			
			steer.Normalize();
			steer = steer * maxSpeed;
			steer = steer - vel;
			steer = steer*maxForce;
		}
		return steer;
	}
	
	Vector3 cohesion(List<Boid> boids){
		float neighbordist = 5.0f;
		 Vector3 sum = new Vector3(0,0,0);
		 
		 int count = 0;
		 for(int i = 0; i < boids.Count; i++){
			 float d = Vector3.Distance(pos, boids[i].pos);
			 
			 if((d > 0) && (d < neighbordist)){
				 sum = sum + boids[i].pos;
				 count++;
			 }
		 }
		 
		 if(count > 0){
			 sum = sum/count;
			 return seek(sum);
		 }
		 return sum;
	}
}
=======
﻿using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Boid : MonoBehaviour{
	
	private float collAv = 5.0f;//collision avoidance
	private float velMa = 5.0f;//vellocity matching
	private float cent = 2.0f;//centering
	private float maxSpeed = 1f;
	private float maxForce = 0.025f;
	
	public Vector3 pos;//position
	public Vector3 vel;//velocity
	public Vector3 acc;//acceleration
	
	public Boid(Vector3 hold){
		acc = new Vector3(0, 0, 0);
		vel = new Vector3(0.01f, 0.01f, 0.01f);
		pos = hold;//new Vector3(x, y, z);
	}
	
	public void run(List<Boid> boids){
		flock(boids);// compares state of boids with other boids
		update();
	}
	
	void applyForce(Vector3 force){
		acc += force;
	}
	
	void flock(List<Boid> boids){
		Vector3 collAv2 = separate(boids);
		Vector3 velMa2 = align(boids);
		Vector3 cent2 = cohesion(boids);
		
		collAv2 = collAv2 * collAv;
		velMa2 = velMa2 * velMa;
		cent2 = cent2 * cent;
		
		applyForce(collAv2);
		applyForce(velMa2);
		applyForce(cent2);
	}
	
	void update(){
		vel += acc;
		
		vel = vel/maxSpeed;
		
		pos += vel; 
		
		acc = acc * 0;
	}
	
	Vector3 seek(Vector3 target){
		Vector3 desired = target - pos;
		
		desired.Normalize();
		
		desired = desired * maxSpeed;
		
		Vector3 steer = desired - vel;
		
		steer = steer*maxForce;
		
		return steer;
	}
	
	Vector3 separate(List<Boid> boids){
		float desiredseperation = 25.0f;
		Vector3 steer = new Vector3(0,0,0);
		int count = 0;
		
		for(int i = 0; i < boids.Count; i++){
			float d = Vector3.Distance(pos, boids[i].pos);
			
			if((d > 0) && (d < desiredseperation)){
				
				Vector3 diff = pos - boids[i].pos;
				diff.Normalize();
				
				diff = diff/d;
				
				steer += diff;
				
				count++;
			}
		}
		
		if(count > 0){
			steer = steer/count;
			
			steer.Normalize();
			steer = steer * maxSpeed;
			steer = steer - vel;
			
			steer = steer*maxForce;
		}
		return steer;
	}
	
	Vector3 align(List<Boid> boids){
		float neighbordist = 5.0f;
		
		Vector3 steer = new Vector3();
		int count = 0;
		
		for(int i = 0; i < boids.Count; i++){
			float d = Vector3.Distance(pos, boids[i].pos);
			
			if((d > 0) && (d < neighbordist)){
				steer = steer + boids[i].vel;
				count++;
			}
		}
		
		if(count > 0){
			steer = steer/count;
			
			steer.Normalize();
			steer = steer * maxSpeed;
			steer = steer - vel;
			steer = steer*maxForce;
		}
		return steer;
	}
	
	Vector3 cohesion(List<Boid> boids){
		float neighbordist = 5.0f;
		 Vector3 sum = new Vector3(0,0,0);
		 
		 int count = 0;
		 for(int i = 0; i < boids.Count; i++){
			 float d = Vector3.Distance(pos, boids[i].pos);
			 
			 if((d > 0) && (d < neighbordist)){
				 sum = sum + boids[i].pos;
				 count++;
			 }
		 }
		 
		 if(count > 0){
			 sum = sum/count;
			 return seek(sum);
		 }
		 return sum;
	}
}
>>>>>>> 17e2c9f0015b84508c0980fb762a587738dc55ae
