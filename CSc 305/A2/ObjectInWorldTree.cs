<<<<<<< HEAD
﻿using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ObjectInWorldTree : MonoBehaviour {
	
	// Use this for initialization
	void Start () {
		GameObject origHouse = GameObject.Find("lowpoly_tree_001");
		Vector3 posHouse = origHouse.transform.position;
		Quaternion rotHouse = origHouse.transform.rotation;
		
		GameObject obj = GameObject.Find("GameObject");
		Grider cs = obj.GetComponent<Grider>();
		
		//Obtain Y
		int convZ = Mathf.FloorToInt(posHouse.z);
		int convX = Mathf.FloorToInt(posHouse.x);
		
		if(convZ < 1){
			convZ = 1;
		}
		
		float step = 1f / cs.xSize;
		
		int frqBase = cs.frqBase;
		
		float newY = cs.Perlin2D(convX, convZ, step, frqBase);
		
		if(newY < 0f)
		{
			newY = 0f;
		}
		
		origHouse.transform.position = new Vector3(posHouse.x, newY*cs.maxHeight, posHouse.z);
		
		//Obtain Rotation
		
		//x
		float plus1X = cs.Perlin2D(convX+1, convZ, step, frqBase);//A
		float minus1X = cs.Perlin2D(convX-1, convZ, step, frqBase);//B
		
		Vector2 rAnglex = new Vector2(convX+1, minus1X);//C
		
		float sqrdXABx = Mathf.Pow((convX+1)-(convX-1), 2);//A poistion
		float sqrdYABx = Mathf.Pow((plus1X)-(minus1X), 2);//B position
		
		float distABx = Mathf.Sqrt(sqrdXABx+sqrdYABx);
		
		
		float sqrdXACx = Mathf.Pow((convX+1)-(rAnglex.x), 2);//A poistion
		float sqrdYACx = Mathf.Pow((plus1X)-(rAnglex.y), 2);//C position
		
		float distACx = Mathf.Sqrt(sqrdXACx+sqrdYACx);
		
		float degreex = Mathf.Asin(distACx/distABx);
		
		//z
		float plus1Z = cs.Perlin2D(convX, convZ+1, step, frqBase);
		float minus1Z = cs.Perlin2D(convX, convZ-1, step, frqBase);
		
		
		Vector2 rAnglez = new Vector2(convZ+1, minus1Z);//C
		
		float sqrdXABz = Mathf.Pow((convZ+1)-(convZ-1), 2);//A poistion
		float sqrdYABz = Mathf.Pow((plus1Z)-(minus1Z), 2);//B position
		
		float distABz = Mathf.Sqrt(sqrdXABz+sqrdYABz);
		
		
		float sqrdXACz = Mathf.Pow((convZ+1)-(rAnglez.x), 2);//A poistion
		float sqrdYACz = Mathf.Pow((plus1Z)-(rAnglez.y), 2);//C position
		
		float distACz = Mathf.Sqrt(sqrdXACz+sqrdYACz);
		
		float degreez = Mathf.Asin(distACz/distABz);
		
		origHouse.transform.rotation = new Quaternion(degreex+rotHouse.x, rotHouse.y, degreez+rotHouse.z, rotHouse.w);
		
	}
=======
﻿using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ObjectInWorldTree : MonoBehaviour {
	
	// Use this for initialization
	void Start () {
		GameObject origHouse = GameObject.Find("lowpoly_tree_001");
		Vector3 posHouse = origHouse.transform.position;
		Quaternion rotHouse = origHouse.transform.rotation;
		
		GameObject obj = GameObject.Find("GameObject");
		Grider cs = obj.GetComponent<Grider>();
		
		//Obtain Y
		int convZ = Mathf.FloorToInt(posHouse.z);
		int convX = Mathf.FloorToInt(posHouse.x);
		
		if(convZ < 1){
			convZ = 1;
		}
		
		float step = 1f / cs.xSize;
		
		int frqBase = cs.frqBase;
		
		float newY = cs.Perlin2D(convX, convZ, step, frqBase);
		
		if(newY < 0f)
		{
			newY = 0f;
		}
		
		origHouse.transform.position = new Vector3(posHouse.x, newY*cs.maxHeight, posHouse.z);
		
		//Obtain Rotation
		
		//x
		float plus1X = cs.Perlin2D(convX+1, convZ, step, frqBase);//A
		float minus1X = cs.Perlin2D(convX-1, convZ, step, frqBase);//B
		
		Vector2 rAnglex = new Vector2(convX+1, minus1X);//C
		
		float sqrdXABx = Mathf.Pow((convX+1)-(convX-1), 2);//A poistion
		float sqrdYABx = Mathf.Pow((plus1X)-(minus1X), 2);//B position
		
		float distABx = Mathf.Sqrt(sqrdXABx+sqrdYABx);
		
		
		float sqrdXACx = Mathf.Pow((convX+1)-(rAnglex.x), 2);//A poistion
		float sqrdYACx = Mathf.Pow((plus1X)-(rAnglex.y), 2);//C position
		
		float distACx = Mathf.Sqrt(sqrdXACx+sqrdYACx);
		
		float degreex = Mathf.Asin(distACx/distABx);
		
		//z
		float plus1Z = cs.Perlin2D(convX, convZ+1, step, frqBase);
		float minus1Z = cs.Perlin2D(convX, convZ-1, step, frqBase);
		
		
		Vector2 rAnglez = new Vector2(convZ+1, minus1Z);//C
		
		float sqrdXABz = Mathf.Pow((convZ+1)-(convZ-1), 2);//A poistion
		float sqrdYABz = Mathf.Pow((plus1Z)-(minus1Z), 2);//B position
		
		float distABz = Mathf.Sqrt(sqrdXABz+sqrdYABz);
		
		
		float sqrdXACz = Mathf.Pow((convZ+1)-(rAnglez.x), 2);//A poistion
		float sqrdYACz = Mathf.Pow((plus1Z)-(rAnglez.y), 2);//C position
		
		float distACz = Mathf.Sqrt(sqrdXACz+sqrdYACz);
		
		float degreez = Mathf.Asin(distACz/distABz);
		
		origHouse.transform.rotation = new Quaternion(degreex+rotHouse.x, rotHouse.y, degreez+rotHouse.z, rotHouse.w);
		
	}
>>>>>>> 17e2c9f0015b84508c0980fb762a587738dc55ae
}