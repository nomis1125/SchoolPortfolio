<<<<<<< HEAD
﻿using System.Collections;
using System.Collections.Generic;
using UnityEngine;

//water = 0.2, earth = 0.4, grass = 5, snow = 6

public class Grider : MonoBehaviour {

	public int xSize = 250;
	public int zSize = 250;
	
	public static int frequencyBase = 4;
	
	public int frqBase = frequencyBase;
	
	public int maxHeight = 20;
	
	private static float[] hash;
	
	private Vector2[] gradients = new Vector2[frequencyBase + 1];
	
	//f(t) = 6t^5-15t^4+10t^3
    float interpolation_function(float t)
    {
        float t_cubic = t * t * t;
        float t_square = t * t;

        return 6 * t_cubic * t_square - 15 * t_square * t_square + 10 * t_cubic;
    }
	
	public float[] ptNums;

	private Mesh mesh;
	public Vector3[] vertices;

	private void Awake () {
		Fill();
		
		Generate();
	}
	
	private void Fill(){
		
		int hashLength = (xSize+1) * (zSize+1);
		
		hash = new float[hashLength];
		
		for(int i = 0; i < hashLength; i ++){
			hash[i] = Random.Range(0f, 2f);//random.value()
		}
		
		//method 1. initialize random gradients with random numbers
        /*
        for (int i = 0; i < frequencyBase + 1; ++ i)
        {
            Vector2 rand_vector = new Vector2(Random.value * 2 - 1, Random.value * 2 - 1);
            gradients[i] = rand_vector.normalized;
        }*/
		
		
		//method 2. random pick from the 4 edge centers
        //gradients
        Vector2[] edge_centers = new Vector2[8];//4
        edge_centers[0] = (new Vector2(0, 1));
        edge_centers[1] = (new Vector2(0, -1));
        edge_centers[2] = (new Vector2(1, 0));
        edge_centers[3] = (new Vector2(-1, 0));
		//--
		edge_centers[4] = (new Vector2(1, 1)).normalized;
        edge_centers[5] = (new Vector2(1, -1)).normalized;
        edge_centers[6] = (new Vector2(1, 1)).normalized;
        edge_centers[7] = (new Vector2(-1, 1)).normalized;

        for (int i = 0; i < frequencyBase + 1; ++ i)
        {
            float roll = Random.value;
            if (roll < 0.13f) gradients[i] = edge_centers[0];
            else if (roll < 0.26f) gradients[i] = edge_centers[1];
            else if (roll < 0.39f) gradients[i] = edge_centers[2];
            else if	(roll < 0.52f) gradients[i] = edge_centers[3];
			
            else if (roll < 0.65f) gradients[i] = edge_centers[4];
            else if (roll < 0.78f) gradients[i] = edge_centers[5];
            else if	(roll < 0.91f) gradients[i] = edge_centers[6];
			else gradients[i] = edge_centers[7];
        }
	}

	private void Generate () {
		
		MeshFilter filter = gameObject.GetComponent<MeshFilter>();
		mesh = filter.mesh;

		vertices = new Vector3[(xSize + 1) * (zSize + 1)];
		Vector2[] uv = new Vector2[vertices.Length];
		Vector4[] tangents = new Vector4[vertices.Length];
		Vector4 tangent = new Vector4(1f, 0f, 0f, -1f);
		
		float step = 1f / xSize;
		
		ptNums = new float[(xSize+1) * (zSize+1)];
		
		for (int i = 0, z = 0; z <= zSize; z++) {
			
			for (int x = 0; x <= xSize; x++, i++) {
				
				float perlinObtain = Perlin2D(x, z, step, frequencyBase);
				
				if(perlinObtain < 0f)
				{
					perlinObtain = 0f;
				}
				
				ptNums[i] = perlinObtain;
				
				vertices[i] = new Vector3(x, perlinObtain*maxHeight, z);
				uv[i] = new Vector2((float)x / xSize, (float)z / zSize);
				tangents[i] = tangent;
			}
		}
		
		mesh.vertices = vertices;
		mesh.uv = uv;
		mesh.tangents = tangents;

		int[] triangles = new int[xSize * zSize * 6];
		
		for (int ti = 0, vi = 0, z = 0; z < zSize; z++, vi++) {
			for (int x = 0; x < xSize; x++, ti += 6, vi++) {
				triangles[ti] = vi;
				triangles[ti + 1] = vi + xSize + 1;
				triangles[ti + 2] = vi + 1;
				triangles[ti + 3] = vi + 1;
				triangles[ti + 4] = vi + xSize + 1;
				triangles[ti + 5] = vi + xSize + 2;
			}
		}
		mesh.triangles = triangles;
		mesh.RecalculateNormals();
	}
	
	public float Perlin2D(int x, int z, float step, int frequency)
    {
			int hashMask = xSize+1;
			int gradientMask = frequencyBase+1;
			//x
            float location_periodX = step * x / (1.0f / frequency);
            int cellX = Mathf.FloorToInt(location_periodX);
            float in_cell_locationX = location_periodX - cellX;//t0
			
			//z
			float location_periodZ = step * z / (1.0f / frequency);
            int cellZ = Mathf.FloorToInt(location_periodZ);
            float in_cell_locationZ = location_periodZ - cellZ;//t0
			
			//t1 = t0 - 1f
			//interpolation_function(t0) = t
			//float v0 = dot_prev * t0
			//float v1 = dot_next * t1
			//lerp(v0,v1,t) -> goes from [v0, v1] by a range of t if t=0 then v0 if t=1 then v1 if t=0.5 then midpoint
			
			cellX &= hashMask;
			cellZ &= hashMask;
			
			int hash0 = Mathf.FloorToInt(hash[cellX]);
			int hash1 = Mathf.FloorToInt(hash[cellX+1]);
			
			//x
            float dot_prevX = Vector2.Dot(gradients[Mathf.FloorToInt(hash[hash0 + cellZ]) & gradientMask], new Vector2(in_cell_locationX, in_cell_locationZ));//v00
			float dot_nextX = Vector2.Dot(gradients[Mathf.FloorToInt(hash[hash1+cellZ]) & gradientMask], new Vector2(in_cell_locationX - 1f, in_cell_locationZ));//v10
			//x
			float weightX = interpolation_function(in_cell_locationX);
            float retX = ((1 - weightX) * dot_prevX + weightX * dot_nextX);//same as lerp and the 2f makes it perlin
			//z
            float dot_prevZ = Vector2.Dot(gradients[Mathf.FloorToInt(hash[hash0+cellZ+1]) & gradientMask], new Vector2(in_cell_locationX, in_cell_locationZ-1f));//v01
			float dot_nextZ = Vector2.Dot(gradients[Mathf.FloorToInt(hash[hash1+cellZ+1]) & gradientMask], new Vector2(in_cell_locationX - 1f, in_cell_locationZ-1f));//v11
			//z
			float weightZ = interpolation_function(in_cell_locationZ);
            float retZ = ((1 - weightX) * dot_prevZ + weightX * dot_nextZ);//same as lerp and the 2f makes it perlin
			
			//end
			return ((1 - weightZ) * retX + weightZ * retZ)*Mathf.Sqrt(2f);//same as lerp and the 2f makes it perlin
			
			
	}
=======
﻿using System.Collections;
using System.Collections.Generic;
using UnityEngine;

//water = 0.2, earth = 0.4, grass = 5, snow = 6

public class Grider : MonoBehaviour {

	public int xSize = 250;
	public int zSize = 250;
	
	public static int frequencyBase = 4;
	
	public int frqBase = frequencyBase;
	
	public int maxHeight = 20;
	
	private static float[] hash;
	
	private Vector2[] gradients = new Vector2[frequencyBase + 1];
	
	//f(t) = 6t^5-15t^4+10t^3
    float interpolation_function(float t)
    {
        float t_cubic = t * t * t;
        float t_square = t * t;

        return 6 * t_cubic * t_square - 15 * t_square * t_square + 10 * t_cubic;
    }
	
	public float[] ptNums;

	private Mesh mesh;
	public Vector3[] vertices;

	private void Awake () {
		Fill();
		
		Generate();
	}
	
	private void Fill(){
		
		int hashLength = (xSize+1) * (zSize+1);
		
		hash = new float[hashLength];
		
		for(int i = 0; i < hashLength; i ++){
			hash[i] = Random.Range(0f, 2f);//random.value()
		}
		
		//method 1. initialize random gradients with random numbers
        /*
        for (int i = 0; i < frequencyBase + 1; ++ i)
        {
            Vector2 rand_vector = new Vector2(Random.value * 2 - 1, Random.value * 2 - 1);
            gradients[i] = rand_vector.normalized;
        }*/
		
		
		//method 2. random pick from the 4 edge centers
        //gradients
        Vector2[] edge_centers = new Vector2[8];//4
        edge_centers[0] = (new Vector2(0, 1));
        edge_centers[1] = (new Vector2(0, -1));
        edge_centers[2] = (new Vector2(1, 0));
        edge_centers[3] = (new Vector2(-1, 0));
		//--
		edge_centers[4] = (new Vector2(1, 1)).normalized;
        edge_centers[5] = (new Vector2(1, -1)).normalized;
        edge_centers[6] = (new Vector2(1, 1)).normalized;
        edge_centers[7] = (new Vector2(-1, 1)).normalized;

        for (int i = 0; i < frequencyBase + 1; ++ i)
        {
            float roll = Random.value;
            if (roll < 0.13f) gradients[i] = edge_centers[0];
            else if (roll < 0.26f) gradients[i] = edge_centers[1];
            else if (roll < 0.39f) gradients[i] = edge_centers[2];
            else if	(roll < 0.52f) gradients[i] = edge_centers[3];
			
            else if (roll < 0.65f) gradients[i] = edge_centers[4];
            else if (roll < 0.78f) gradients[i] = edge_centers[5];
            else if	(roll < 0.91f) gradients[i] = edge_centers[6];
			else gradients[i] = edge_centers[7];
        }
	}

	private void Generate () {
		
		MeshFilter filter = gameObject.GetComponent<MeshFilter>();
		mesh = filter.mesh;

		vertices = new Vector3[(xSize + 1) * (zSize + 1)];
		Vector2[] uv = new Vector2[vertices.Length];
		Vector4[] tangents = new Vector4[vertices.Length];
		Vector4 tangent = new Vector4(1f, 0f, 0f, -1f);
		
		float step = 1f / xSize;
		
		ptNums = new float[(xSize+1) * (zSize+1)];
		
		for (int i = 0, z = 0; z <= zSize; z++) {
			
			for (int x = 0; x <= xSize; x++, i++) {
				
				float perlinObtain = Perlin2D(x, z, step, frequencyBase);
				
				if(perlinObtain < 0f)
				{
					perlinObtain = 0f;
				}
				
				ptNums[i] = perlinObtain;
				
				vertices[i] = new Vector3(x, perlinObtain*maxHeight, z);
				uv[i] = new Vector2((float)x / xSize, (float)z / zSize);
				tangents[i] = tangent;
			}
		}
		
		mesh.vertices = vertices;
		mesh.uv = uv;
		mesh.tangents = tangents;

		int[] triangles = new int[xSize * zSize * 6];
		
		for (int ti = 0, vi = 0, z = 0; z < zSize; z++, vi++) {
			for (int x = 0; x < xSize; x++, ti += 6, vi++) {
				triangles[ti] = vi;
				triangles[ti + 1] = vi + xSize + 1;
				triangles[ti + 2] = vi + 1;
				triangles[ti + 3] = vi + 1;
				triangles[ti + 4] = vi + xSize + 1;
				triangles[ti + 5] = vi + xSize + 2;
			}
		}
		mesh.triangles = triangles;
		mesh.RecalculateNormals();
	}
	
	public float Perlin2D(int x, int z, float step, int frequency)
    {
			int hashMask = xSize+1;
			int gradientMask = frequencyBase+1;
			//x
            float location_periodX = step * x / (1.0f / frequency);
            int cellX = Mathf.FloorToInt(location_periodX);
            float in_cell_locationX = location_periodX - cellX;//t0
			
			//z
			float location_periodZ = step * z / (1.0f / frequency);
            int cellZ = Mathf.FloorToInt(location_periodZ);
            float in_cell_locationZ = location_periodZ - cellZ;//t0
			
			//t1 = t0 - 1f
			//interpolation_function(t0) = t
			//float v0 = dot_prev * t0
			//float v1 = dot_next * t1
			//lerp(v0,v1,t) -> goes from [v0, v1] by a range of t if t=0 then v0 if t=1 then v1 if t=0.5 then midpoint
			
			cellX &= hashMask;
			cellZ &= hashMask;
			
			int hash0 = Mathf.FloorToInt(hash[cellX]);
			int hash1 = Mathf.FloorToInt(hash[cellX+1]);
			
			//x
            float dot_prevX = Vector2.Dot(gradients[Mathf.FloorToInt(hash[hash0 + cellZ]) & gradientMask], new Vector2(in_cell_locationX, in_cell_locationZ));//v00
			float dot_nextX = Vector2.Dot(gradients[Mathf.FloorToInt(hash[hash1+cellZ]) & gradientMask], new Vector2(in_cell_locationX - 1f, in_cell_locationZ));//v10
			//x
			float weightX = interpolation_function(in_cell_locationX);
            float retX = ((1 - weightX) * dot_prevX + weightX * dot_nextX);//same as lerp and the 2f makes it perlin
			//z
            float dot_prevZ = Vector2.Dot(gradients[Mathf.FloorToInt(hash[hash0+cellZ+1]) & gradientMask], new Vector2(in_cell_locationX, in_cell_locationZ-1f));//v01
			float dot_nextZ = Vector2.Dot(gradients[Mathf.FloorToInt(hash[hash1+cellZ+1]) & gradientMask], new Vector2(in_cell_locationX - 1f, in_cell_locationZ-1f));//v11
			//z
			float weightZ = interpolation_function(in_cell_locationZ);
            float retZ = ((1 - weightX) * dot_prevZ + weightX * dot_nextZ);//same as lerp and the 2f makes it perlin
			
			//end
			return ((1 - weightZ) * retX + weightZ * retZ)*Mathf.Sqrt(2f);//same as lerp and the 2f makes it perlin
			
			
	}
>>>>>>> 17e2c9f0015b84508c0980fb762a587738dc55ae
}