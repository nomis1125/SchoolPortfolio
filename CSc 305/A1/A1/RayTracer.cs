<<<<<<< HEAD
﻿/*
Simon Walker V00865659

RayTracer.cs

CSC 305
*/

using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEditor;
using System;

public class RayTracer : MonoBehaviour
{

    public Texture2D texture_on_cube;

    Texture2D RayTracingResult;


    void Start()
    {

        Camera this_camera = gameObject.GetComponent<Camera>();
        Debug.Assert(this_camera);

        int pixel_width = this_camera.pixelWidth;
        int pixel_height = this_camera.pixelHeight;

        RayTracingResult = new Texture2D(pixel_width, pixel_height);
		
        //---
        Vector3 RayOrigin = Vector3.zero;
        Vector3 VPCenter = Vector3.forward;
        float ViewportWidth = 3;
        float ViewportHeight = ViewportWidth / pixel_width * pixel_height;

        float VPWidthHalf = (float)(ViewportWidth / 2);
        float PixelWidthHalf = (float)(pixel_width / 2);

        float VPHeightHalf = (float)(ViewportHeight / 2);
        float PixelHeightHalf = (float)(pixel_height / 2);
		
        Color BackgroundColor = Color.grey;
        Color AmbientColor = new Color(0.1f, 0.1f, 0);
		
		Color LightColor = Color.yellow;
		Vector3 LightDirection = new Vector3(1,1,-1);
		
        float diffuseStrength = 0.5f;
        float specularStrength = 0.2f;
        float specularPower = 8;
        //---

        Vector3 sphereCentre = new Vector3(0,0, 8);

        float radius = 3;

        #region Generate a black and white checker pattern
		
		for (int i = 0; i < pixel_width; ++i){
            for (int j = 0; j < pixel_height; ++j)
            {
				RayTracingResult.SetPixel(i, j, Color.grey);
				
				Vector3 RayDirection = VPCenter;
				RayDirection.x = (i - PixelWidthHalf) / (float)(PixelWidthHalf) * VPWidthHalf;
                RayDirection.y = (j - PixelHeightHalf) / (float)(PixelHeightHalf) * VPHeightHalf;
                RayDirection.Normalize();

                //Check Ray Intersection o=origin of ray; c = sphere centre; p = intersection; d = direction
                Vector3 oc = sphereCentre - RayOrigin;
                
                float og = Vector3.Dot(oc, RayDirection);
				float radiusSquare = radius * radius;
				float ocSquare = Vector3.Dot(oc, oc);
				
				float cg = (float)Math.Sqrt(ocSquare - (og * og));
				
                if (cg < radius)
                {
					//Find position of intersection
                    float pg = (float)Math.Sqrt(radiusSquare - (float)Math.Pow(cg, 2));

                    float op = og - pg;
                    Vector3 intersection = RayOrigin + (op * RayDirection);

					Vector3 intersect_normal = intersection - sphereCentre;
					intersect_normal.Normalize();
					
					Color pixelColor = AmbientColor;
					
					float diffuse = Vector3.Dot(intersect_normal, LightDirection) * diffuseStrength;
					
					pixelColor += diffuse * LightColor;
					
					Vector3 view = (-1) * RayDirection;
					
					Vector3 half = view + LightDirection;
					
					float blinn = Vector3.Dot(half, intersect_normal);
					float specular = (float)Math.Pow(blinn, specularPower) * specularStrength;
					
					pixelColor += specular * LightColor;

                    RayTracingResult.SetPixel(i, j, pixelColor);
                }
			}
		}
        #endregion

        //Please put your own ray tracing code here
        RayTracingResult.Apply();


    }

    private void OnRenderImage(RenderTexture source, RenderTexture destination)
    {
        //Show the generated ray tracing image on screen
        Graphics.Blit(RayTracingResult, destination);
    }
=======
﻿/*
Simon Walker V00865659

RayTracer.cs

CSC 305
*/

using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEditor;
using System;

public class RayTracer : MonoBehaviour
{

    public Texture2D texture_on_cube;

    Texture2D RayTracingResult;


    void Start()
    {

        Camera this_camera = gameObject.GetComponent<Camera>();
        Debug.Assert(this_camera);

        int pixel_width = this_camera.pixelWidth;
        int pixel_height = this_camera.pixelHeight;

        RayTracingResult = new Texture2D(pixel_width, pixel_height);
		
        //---
        Vector3 RayOrigin = Vector3.zero;
        Vector3 VPCenter = Vector3.forward;
        float ViewportWidth = 3;
        float ViewportHeight = ViewportWidth / pixel_width * pixel_height;

        float VPWidthHalf = (float)(ViewportWidth / 2);
        float PixelWidthHalf = (float)(pixel_width / 2);

        float VPHeightHalf = (float)(ViewportHeight / 2);
        float PixelHeightHalf = (float)(pixel_height / 2);
		
        Color BackgroundColor = Color.grey;
        Color AmbientColor = new Color(0.1f, 0.1f, 0);
		
		Color LightColor = Color.yellow;
		Vector3 LightDirection = new Vector3(1,1,-1);
		
        float diffuseStrength = 0.5f;
        float specularStrength = 0.2f;
        float specularPower = 8;
        //---

        Vector3 sphereCentre = new Vector3(0,0, 8);

        float radius = 3;

        #region Generate a black and white checker pattern
		
		for (int i = 0; i < pixel_width; ++i){
            for (int j = 0; j < pixel_height; ++j)
            {
				RayTracingResult.SetPixel(i, j, Color.grey);
				
				Vector3 RayDirection = VPCenter;
				RayDirection.x = (i - PixelWidthHalf) / (float)(PixelWidthHalf) * VPWidthHalf;
                RayDirection.y = (j - PixelHeightHalf) / (float)(PixelHeightHalf) * VPHeightHalf;
                RayDirection.Normalize();

                //Check Ray Intersection o=origin of ray; c = sphere centre; p = intersection; d = direction
                Vector3 oc = sphereCentre - RayOrigin;
                
                float og = Vector3.Dot(oc, RayDirection);
				float radiusSquare = radius * radius;
				float ocSquare = Vector3.Dot(oc, oc);
				
				float cg = (float)Math.Sqrt(ocSquare - (og * og));
				
                if (cg < radius)
                {
					//Find position of intersection
                    float pg = (float)Math.Sqrt(radiusSquare - (float)Math.Pow(cg, 2));

                    float op = og - pg;
                    Vector3 intersection = RayOrigin + (op * RayDirection);

					Vector3 intersect_normal = intersection - sphereCentre;
					intersect_normal.Normalize();
					
					Color pixelColor = AmbientColor;
					
					float diffuse = Vector3.Dot(intersect_normal, LightDirection) * diffuseStrength;
					
					pixelColor += diffuse * LightColor;
					
					Vector3 view = (-1) * RayDirection;
					
					Vector3 half = view + LightDirection;
					
					float blinn = Vector3.Dot(half, intersect_normal);
					float specular = (float)Math.Pow(blinn, specularPower) * specularStrength;
					
					pixelColor += specular * LightColor;

                    RayTracingResult.SetPixel(i, j, pixelColor);
                }
			}
		}
        #endregion

        //Please put your own ray tracing code here
        RayTracingResult.Apply();


    }

    private void OnRenderImage(RenderTexture source, RenderTexture destination)
    {
        //Show the generated ray tracing image on screen
        Graphics.Blit(RayTracingResult, destination);
    }
>>>>>>> 17e2c9f0015b84508c0980fb762a587738dc55ae
}