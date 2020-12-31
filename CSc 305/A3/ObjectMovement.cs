<<<<<<< HEAD
﻿using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ObjectMovement : MonoBehaviour {
	
	private class Object_Status
	{
		public Vector3 position;
		public Vector3 speed;
		public GameObject cube_flock;
	}
	
	private int sizeVec = 4;//equal to how many points you have. Used for the ease of having more turning points.
	
	private List<Object_Status> status_list;
	[Range(5,50)]
	public int boidCount = 5;
	//
	[Range(0.001f, 1f)]
	public float Speed = 0.01f;
	private float current_t;
	
	private List<GameObject>object_list;//
	
	private CatmullSpline3D spline;
	
	private List<Boid> boid_list;
	
	void Start () {
		
		spline = new CatmullSpline3D();
		Vector3[] cps = new Vector3[sizeVec*2];
		
		cps[0] = new Vector3(-3, 3, -3);
		cps[1] = new Vector3(0, 5, 3);
		cps[2] = new Vector3(4, 5, 3);
		cps[3] = new Vector3(3, 3, -3);
		cps[4] = cps[0];
		cps[5] = cps[1];
		cps[6] = cps[2];
		cps[7] = cps[3];
		spline.control_points = cps;
		current_t = 0;
		
		object_list = new List<GameObject>();
		status_list = new List<Object_Status>();
		
		boid_list = new List<Boid>();
		
		//create multiple boids
		for(int i = 0; i < boidCount; i++)
		{
			
			GameObject newBoid = Instantiate(GameObject.Find("simpleboid"));;
			
			newBoid.transform.parent = gameObject.transform;
			
			newBoid.transform.position += new Vector3(Random.Range(0,5),Random.Range(0,5),Random.Range(0,5));
			
			newBoid.name = "boid No. " + i.ToString();
			object_list.Add(newBoid);
			
			boid_list.Add(new Boid(newBoid.transform.position));
		}
	}
	
	// Update is called once per frame
	void Update () {
		
		GameObject Centre = GameObject.Find("Cube_Home");
		Vector3 CentrePos = Centre.transform.position;
		
		current_t += Speed;
		if(current_t > sizeVec) current_t = current_t - sizeVec;
		
		Vector3 curr_pos = EvaluateAt(current_t);
		Vector3 next_pos = EvaluateAt(current_t + Speed);
		
		gameObject.transform.position = curr_pos;
		
		Vector3 forward = next_pos - curr_pos;
		if(forward.sqrMagnitude > 1e-5)
		{
			forward.Normalize();
			
			gameObject.transform.forward = forward;
			
			for(int i = 0; i < boid_list.Count; i++){
				boid_list[i].run(boid_list);
				
				
				object_list[i].transform.rotation = Quaternion.LookRotation(GetComponent<Rigidbody>().velocity);//Advanced Req 1
				
				object_list[i].transform.position += boid_list[i].pos/10000;
				
				Vector3 saveObjPos = object_list[i].transform.position;
				
				if(saveObjPos.y < 0){
					saveObjPos.y = 0;
				}
				
				if(saveObjPos.y > 20){
					saveObjPos.y = 20;
				}
				
				if(saveObjPos.x > CentrePos.x+10){
					saveObjPos.x = CentrePos.x+10;
				}
				
				if(saveObjPos.x < CentrePos.x-10){
					saveObjPos.x = CentrePos.x-10;
				}
				
				if(saveObjPos.z > CentrePos.z+10){
					saveObjPos.z = CentrePos.z+10;
				}
				
				if(saveObjPos.z < CentrePos.z-10){
					saveObjPos.z = CentrePos.z-10;
				}
				
				object_list[i].transform.position = saveObjPos;
				
			}
			
		}
	}
	
	Vector3 EvaluateAt(float t)
	{
		if(t > sizeVec) t -= sizeVec;
		return spline.Sample(t);
	}
}
=======
﻿using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ObjectMovement : MonoBehaviour {
	
	private class Object_Status
	{
		public Vector3 position;
		public Vector3 speed;
		public GameObject cube_flock;
	}
	
	private int sizeVec = 4;//equal to how many points you have. Used for the ease of having more turning points.
	
	private List<Object_Status> status_list;
	[Range(5,50)]
	public int boidCount = 5;
	//
	[Range(0.001f, 1f)]
	public float Speed = 0.01f;
	private float current_t;
	
	private List<GameObject>object_list;//
	
	private CatmullSpline3D spline;
	
	private List<Boid> boid_list;
	
	void Start () {
		
		spline = new CatmullSpline3D();
		Vector3[] cps = new Vector3[sizeVec*2];
		
		cps[0] = new Vector3(-3, 3, -3);
		cps[1] = new Vector3(0, 5, 3);
		cps[2] = new Vector3(4, 5, 3);
		cps[3] = new Vector3(3, 3, -3);
		cps[4] = cps[0];
		cps[5] = cps[1];
		cps[6] = cps[2];
		cps[7] = cps[3];
		spline.control_points = cps;
		current_t = 0;
		
		object_list = new List<GameObject>();
		status_list = new List<Object_Status>();
		
		boid_list = new List<Boid>();
		
		//create multiple boids
		for(int i = 0; i < boidCount; i++)
		{
			
			GameObject newBoid = Instantiate(GameObject.Find("simpleboid"));;
			
			newBoid.transform.parent = gameObject.transform;
			
			newBoid.transform.position += new Vector3(Random.Range(0,5),Random.Range(0,5),Random.Range(0,5));
			
			newBoid.name = "boid No. " + i.ToString();
			object_list.Add(newBoid);
			
			boid_list.Add(new Boid(newBoid.transform.position));
		}
	}
	
	// Update is called once per frame
	void Update () {
		
		GameObject Centre = GameObject.Find("Cube_Home");
		Vector3 CentrePos = Centre.transform.position;
		
		current_t += Speed;
		if(current_t > sizeVec) current_t = current_t - sizeVec;
		
		Vector3 curr_pos = EvaluateAt(current_t);
		Vector3 next_pos = EvaluateAt(current_t + Speed);
		
		gameObject.transform.position = curr_pos;
		
		Vector3 forward = next_pos - curr_pos;
		if(forward.sqrMagnitude > 1e-5)
		{
			forward.Normalize();
			
			gameObject.transform.forward = forward;
			
			for(int i = 0; i < boid_list.Count; i++){
				boid_list[i].run(boid_list);
				
				
				object_list[i].transform.rotation = Quaternion.LookRotation(GetComponent<Rigidbody>().velocity);//Advanced Req 1
				
				object_list[i].transform.position += boid_list[i].pos/10000;
				
				Vector3 saveObjPos = object_list[i].transform.position;
				
				if(saveObjPos.y < 0){
					saveObjPos.y = 0;
				}
				
				if(saveObjPos.y > 20){
					saveObjPos.y = 20;
				}
				
				if(saveObjPos.x > CentrePos.x+10){
					saveObjPos.x = CentrePos.x+10;
				}
				
				if(saveObjPos.x < CentrePos.x-10){
					saveObjPos.x = CentrePos.x-10;
				}
				
				if(saveObjPos.z > CentrePos.z+10){
					saveObjPos.z = CentrePos.z+10;
				}
				
				if(saveObjPos.z < CentrePos.z-10){
					saveObjPos.z = CentrePos.z-10;
				}
				
				object_list[i].transform.position = saveObjPos;
				
			}
			
		}
	}
	
	Vector3 EvaluateAt(float t)
	{
		if(t > sizeVec) t -= sizeVec;
		return spline.Sample(t);
	}
}
>>>>>>> 17e2c9f0015b84508c0980fb762a587738dc55ae
