<<<<<<< HEAD
﻿using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEditor;
using System;

public class RayTracerImg : MonoBehaviour {

    public Texture2D texture_on_cube;
	
	public Texture2D _texture;

    Texture2D RayTracingResult;

  
    void Start () {

        Camera this_camera = gameObject.GetComponent<Camera>();
        Debug.Assert(this_camera);

        int pixel_width = this_camera.pixelWidth;
        int pixel_height = this_camera.pixelHeight;

        RayTracingResult = new Texture2D(pixel_width, pixel_height);
		
		Color BackgroundColor = Color.grey;

        #region Generate a black and white checker pattern
        
        for (int i = 0; i < pixel_width; ++ i)
            for (int j = 0; j < pixel_height; ++ j)
            {
               // RayTracingResult.SetPixel(i, j, BackgroundColor);
            }
        #endregion

        //Please put your own ray tracing code here
		
		//_____
		Vector3 RayOrigin = Vector3.zero;
        Vector3 VPCenter = Vector3.forward;
        float ViewportWidth = 3;
        float ViewportHeight = ViewportWidth / pixel_width * pixel_height;

        float VPWidthHalf = ViewportWidth / 2;
        int PixelWidthHalf = pixel_width / 2;

        float VPHeightHalf = ViewportHeight / 2;
        int PixelHeightHalf = pixel_height / 2;
		
		//_____
		
		List<Vector3> vertices = new List<Vector3>();
        vertices.Add(new Vector3(-4, -2.8f, 10)); //0 - a
        vertices.Add(new Vector3(-4, 2.8f, 10));  //1 - b
        vertices.Add(new Vector3(0, -2.8f, 9));   //2 - c
        vertices.Add(new Vector3(0, 2.8f, 9));    //3
        vertices.Add(new Vector3(4, -2.8f, 10));  //4
        vertices.Add(new Vector3(4, 2.8f, 10));   //5
		
		List<int> indices = new List<int>();
        //triangle 1
        indices.Add(0);
        indices.Add(1);
        indices.Add(2);
        //triangle 2
        indices.Add(2);
        indices.Add(1);
        indices.Add(3);
        //triangle 3
        indices.Add(2);
        indices.Add(3);
        indices.Add(5);
        //triangle 4
        indices.Add(2);
        indices.Add(5);
        indices.Add(4);
		
		//Texture2D texture;
		
		List<Vector2> UVs = new List<Vector2>();
        UVs.Add(new Vector2(0, 0));
        UVs.Add(new Vector2(0, 1));
        UVs.Add(new Vector2(1, 0));
        UVs.Add(new Vector2(1, 1));
        UVs.Add(new Vector2(0, 0));
        UVs.Add(new Vector2(0, 1));
		
		for(int i = 0; i < pixel_width; ++i){
			for(int j = 0; j < pixel_height; ++j){
				
				Color pixColor = Color.gray;
				
				Vector3 RayDirection = VPCenter;
				
				RayDirection.x = (i - PixelWidthHalf) / (float)(PixelWidthHalf) * VPWidthHalf;
                RayDirection.y = (j - PixelHeightHalf) / (float)(PixelHeightHalf) * VPHeightHalf;
                RayDirection.Normalize();
				
				for(int triangle = 0; triangle < 4; triangle++){
					if(triangle == 0){
						Vector3 vA = vertices[0];
						Vector3 vB = vertices[1];
						Vector3 vC = vertices[2];
					
						vA.Normalize();
						vB.Normalize();
						vC.Normalize();
					
						Vector3 N = Vector3.Cross((vA-vB), (vA-vC));
					
						float D = Vector3.Dot(N, vA);
						float nd = Vector3.Dot(N, RayDirection);
						if(nd != 0){
							float t = ((Vector3.Dot(N, RayOrigin)+D)/nd);
							
							Vector3 p = RayOrigin + t *RayDirection;
						
							Vector3 edge0 = vB - vA;
							Vector3 edge1 = vC - vB;
							Vector3 edge2 = vA - vC;
							
							Vector3 C0 = p - vA;
							Vector3 C1 = p - vB;
							Vector3 C2 = p - vC;
							
							Vector3 C0Val = Vector3.Cross(edge0, C0);
							Vector3 C1Val = Vector3.Cross(edge1, C1);
							Vector3 C2Val = Vector3.Cross(edge2, C2);
							
							float Value1 = C1Val.magnitude / N.magnitude;
							float Value2 = C2Val.magnitude / N.magnitude;
							float Value3 = C0Val.magnitude / N.magnitude;
							
							if(Vector3.Dot(N, Vector3.Cross(edge0, C0)) > 0 && Vector3.Dot(N, Vector3.Cross(edge1, C1)) > 0 && Vector3.Dot(N, Vector3.Cross(edge2, C2)) > 0){
								//inside triangle;
								
								int texWidth = _texture.width;
								int texHeight = _texture.height;
								Vector2 map = (Value1 * UVs[0]) + (Value2 * UVs[1]) + (Value3 * UVs[2]);
								
								Color currentColor = _texture.GetPixel((int)(map.x * texWidth), (int)(map.y * texHeight));
								
								pixColor = currentColor;
							}
						}
					}
					
					if(triangle == 1){
						Vector3 vA = vertices[2];
						Vector3 vB = vertices[1];
						Vector3 vC = vertices[3];
					
						vA.Normalize();
						vB.Normalize();
						vC.Normalize();
					
						Vector3 N = Vector3.Cross((vA-vB), (vA-vC));
					
						float D = Vector3.Dot(N, vA);
						float nd = Vector3.Dot(N, RayDirection);
						if(nd != 0){
							float t = ((Vector3.Dot(N, RayOrigin)+D)/nd);
							
							Vector3 p = RayOrigin + t *RayDirection;
						
							Vector3 edge0 = vB - vA;
							Vector3 edge1 = vC - vB;
							Vector3 edge2 = vA - vC;
							
							Vector3 C0 = p - vA;
							Vector3 C1 = p - vB;
							Vector3 C2 = p - vC;
							
							Vector3 C0Val = Vector3.Cross(edge0, C0);
							Vector3 C1Val = Vector3.Cross(edge1, C1);
							Vector3 C2Val = Vector3.Cross(edge2, C2);
							
							float Value1 = C1Val.magnitude / N.magnitude;
							float Value2 = C2Val.magnitude / N.magnitude;
							float Value3 = C0Val.magnitude / N.magnitude;
							
							if(Vector3.Dot(N, Vector3.Cross(edge0, C0)) > 0 && Vector3.Dot(N, Vector3.Cross(edge1, C1)) > 0 && Vector3.Dot(N, Vector3.Cross(edge2, C2)) > 0){
								//inside triangle;
								
								int texWidth = _texture.width;
								int texHeight = _texture.height;
								Vector2 map = (Value1 * UVs[0]) + (Value2 * UVs[1]) + (Value3 * UVs[2]);
								
								Color currentColor = _texture.GetPixel((int)(map.x * texWidth), (int)(map.y * texHeight));
								
								pixColor = currentColor;
							}
						}
					}
					
					if(triangle == 2){
						Vector3 vA = vertices[2];
						Vector3 vB = vertices[3];
						Vector3 vC = vertices[5];
					
						vA.Normalize();
						vB.Normalize();
						vC.Normalize();
					
						Vector3 N = Vector3.Cross((vA-vB), (vA-vC));
					
						float D = Vector3.Dot(N, vA);
						float nd = Vector3.Dot(N, RayDirection);
						if(nd != 0){
							float t = ((Vector3.Dot(N, RayOrigin)+D)/nd);
							
							Vector3 p = RayOrigin + t *RayDirection;
						
							Vector3 edge0 = vB - vA;
							Vector3 edge1 = vC - vB;
							Vector3 edge2 = vA - vC;
							
							Vector3 C0 = p - vA;
							Vector3 C1 = p - vB;
							Vector3 C2 = p - vC;
							
							Vector3 C0Val = Vector3.Cross(edge0, C0);
							Vector3 C1Val = Vector3.Cross(edge1, C1);
							Vector3 C2Val = Vector3.Cross(edge2, C2);
							
							float Value1 = C1Val.magnitude / N.magnitude;
							float Value2 = C2Val.magnitude / N.magnitude;
							float Value3 = C0Val.magnitude / N.magnitude;
							
							if(Vector3.Dot(N, Vector3.Cross(edge0, C0)) > 0 && Vector3.Dot(N, Vector3.Cross(edge1, C1)) > 0 && Vector3.Dot(N, Vector3.Cross(edge2, C2)) > 0){
								//inside triangle;
								
								int texWidth = _texture.width;
								int texHeight = _texture.height;
								Vector2 map = (Value1 * UVs[0]) + (Value2 * UVs[1]) + (Value3 * UVs[2]);
								
								Color currentColor = _texture.GetPixel((int)(map.x * texWidth), (int)(map.y * texHeight));
								
								pixColor = currentColor;
							}
						}
					}
					if(triangle == 3){
						Vector3 vA = vertices[2];
						Vector3 vB = vertices[5];
						Vector3 vC = vertices[4];
					
						vA.Normalize();
						vB.Normalize();
						vC.Normalize();
					
						Vector3 N = Vector3.Cross((vA-vB), (vA-vC));
					
						float D = Vector3.Dot(N, vA);
						float nd = Vector3.Dot(N, RayDirection);
						if(nd != 0){
							float t = ((Vector3.Dot(N, RayOrigin)+D)/nd);
							
							Vector3 p = RayOrigin + t *RayDirection;
						
							Vector3 edge0 = vB - vA;
							Vector3 edge1 = vC - vB;
							Vector3 edge2 = vA - vC;
							
							Vector3 C0 = p - vA;
							Vector3 C1 = p - vB;
							Vector3 C2 = p - vC;
							
							Vector3 C0Val = Vector3.Cross(edge0, C0);
							Vector3 C1Val = Vector3.Cross(edge1, C1);
							Vector3 C2Val = Vector3.Cross(edge2, C2);
							
							float Value1 = C1Val.magnitude / N.magnitude;
							float Value2 = C2Val.magnitude / N.magnitude;
							float Value3 = C0Val.magnitude / N.magnitude;
							
							if(Vector3.Dot(N, Vector3.Cross(edge0, C0)) > 0 && Vector3.Dot(N, Vector3.Cross(edge1, C1)) > 0 && Vector3.Dot(N, Vector3.Cross(edge2, C2)) > 0){
								//inside triangle;
								
								int texWidth = _texture.width;
								int texHeight = _texture.height;
								Vector2 map = (Value1 * UVs[0]) + (Value2 * UVs[1]) + (Value3 * UVs[2]);
								
								Color currentColor = _texture.GetPixel((int)(map.x * texWidth), (int)(map.y * texHeight));
								
								pixColor = currentColor;
							}
						}
					}
				}
				RayTracingResult.SetPixel(i, j, pixColor);
			}
		}
		
		
        RayTracingResult.Apply();
       
       
    }

    private void OnRenderImage(RenderTexture source, RenderTexture destination)
    {
        //Show the generated ray tracing image on screen
        Graphics.Blit(RayTracingResult, destination);
    }
=======
﻿using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEditor;
using System;

public class RayTracerImg : MonoBehaviour {

    public Texture2D texture_on_cube;
	
	public Texture2D _texture;

    Texture2D RayTracingResult;

  
    void Start () {

        Camera this_camera = gameObject.GetComponent<Camera>();
        Debug.Assert(this_camera);

        int pixel_width = this_camera.pixelWidth;
        int pixel_height = this_camera.pixelHeight;

        RayTracingResult = new Texture2D(pixel_width, pixel_height);
		
		Color BackgroundColor = Color.grey;

        #region Generate a black and white checker pattern
        
        for (int i = 0; i < pixel_width; ++ i)
            for (int j = 0; j < pixel_height; ++ j)
            {
               // RayTracingResult.SetPixel(i, j, BackgroundColor);
            }
        #endregion

        //Please put your own ray tracing code here
		
		//_____
		Vector3 RayOrigin = Vector3.zero;
        Vector3 VPCenter = Vector3.forward;
        float ViewportWidth = 3;
        float ViewportHeight = ViewportWidth / pixel_width * pixel_height;

        float VPWidthHalf = ViewportWidth / 2;
        int PixelWidthHalf = pixel_width / 2;

        float VPHeightHalf = ViewportHeight / 2;
        int PixelHeightHalf = pixel_height / 2;
		
		//_____
		
		List<Vector3> vertices = new List<Vector3>();
        vertices.Add(new Vector3(-4, -2.8f, 10)); //0 - a
        vertices.Add(new Vector3(-4, 2.8f, 10));  //1 - b
        vertices.Add(new Vector3(0, -2.8f, 9));   //2 - c
        vertices.Add(new Vector3(0, 2.8f, 9));    //3
        vertices.Add(new Vector3(4, -2.8f, 10));  //4
        vertices.Add(new Vector3(4, 2.8f, 10));   //5
		
		List<int> indices = new List<int>();
        //triangle 1
        indices.Add(0);
        indices.Add(1);
        indices.Add(2);
        //triangle 2
        indices.Add(2);
        indices.Add(1);
        indices.Add(3);
        //triangle 3
        indices.Add(2);
        indices.Add(3);
        indices.Add(5);
        //triangle 4
        indices.Add(2);
        indices.Add(5);
        indices.Add(4);
		
		//Texture2D texture;
		
		List<Vector2> UVs = new List<Vector2>();
        UVs.Add(new Vector2(0, 0));
        UVs.Add(new Vector2(0, 1));
        UVs.Add(new Vector2(1, 0));
        UVs.Add(new Vector2(1, 1));
        UVs.Add(new Vector2(0, 0));
        UVs.Add(new Vector2(0, 1));
		
		for(int i = 0; i < pixel_width; ++i){
			for(int j = 0; j < pixel_height; ++j){
				
				Color pixColor = Color.gray;
				
				Vector3 RayDirection = VPCenter;
				
				RayDirection.x = (i - PixelWidthHalf) / (float)(PixelWidthHalf) * VPWidthHalf;
                RayDirection.y = (j - PixelHeightHalf) / (float)(PixelHeightHalf) * VPHeightHalf;
                RayDirection.Normalize();
				
				for(int triangle = 0; triangle < 4; triangle++){
					if(triangle == 0){
						Vector3 vA = vertices[0];
						Vector3 vB = vertices[1];
						Vector3 vC = vertices[2];
					
						vA.Normalize();
						vB.Normalize();
						vC.Normalize();
					
						Vector3 N = Vector3.Cross((vA-vB), (vA-vC));
					
						float D = Vector3.Dot(N, vA);
						float nd = Vector3.Dot(N, RayDirection);
						if(nd != 0){
							float t = ((Vector3.Dot(N, RayOrigin)+D)/nd);
							
							Vector3 p = RayOrigin + t *RayDirection;
						
							Vector3 edge0 = vB - vA;
							Vector3 edge1 = vC - vB;
							Vector3 edge2 = vA - vC;
							
							Vector3 C0 = p - vA;
							Vector3 C1 = p - vB;
							Vector3 C2 = p - vC;
							
							Vector3 C0Val = Vector3.Cross(edge0, C0);
							Vector3 C1Val = Vector3.Cross(edge1, C1);
							Vector3 C2Val = Vector3.Cross(edge2, C2);
							
							float Value1 = C1Val.magnitude / N.magnitude;
							float Value2 = C2Val.magnitude / N.magnitude;
							float Value3 = C0Val.magnitude / N.magnitude;
							
							if(Vector3.Dot(N, Vector3.Cross(edge0, C0)) > 0 && Vector3.Dot(N, Vector3.Cross(edge1, C1)) > 0 && Vector3.Dot(N, Vector3.Cross(edge2, C2)) > 0){
								//inside triangle;
								
								int texWidth = _texture.width;
								int texHeight = _texture.height;
								Vector2 map = (Value1 * UVs[0]) + (Value2 * UVs[1]) + (Value3 * UVs[2]);
								
								Color currentColor = _texture.GetPixel((int)(map.x * texWidth), (int)(map.y * texHeight));
								
								pixColor = currentColor;
							}
						}
					}
					
					if(triangle == 1){
						Vector3 vA = vertices[2];
						Vector3 vB = vertices[1];
						Vector3 vC = vertices[3];
					
						vA.Normalize();
						vB.Normalize();
						vC.Normalize();
					
						Vector3 N = Vector3.Cross((vA-vB), (vA-vC));
					
						float D = Vector3.Dot(N, vA);
						float nd = Vector3.Dot(N, RayDirection);
						if(nd != 0){
							float t = ((Vector3.Dot(N, RayOrigin)+D)/nd);
							
							Vector3 p = RayOrigin + t *RayDirection;
						
							Vector3 edge0 = vB - vA;
							Vector3 edge1 = vC - vB;
							Vector3 edge2 = vA - vC;
							
							Vector3 C0 = p - vA;
							Vector3 C1 = p - vB;
							Vector3 C2 = p - vC;
							
							Vector3 C0Val = Vector3.Cross(edge0, C0);
							Vector3 C1Val = Vector3.Cross(edge1, C1);
							Vector3 C2Val = Vector3.Cross(edge2, C2);
							
							float Value1 = C1Val.magnitude / N.magnitude;
							float Value2 = C2Val.magnitude / N.magnitude;
							float Value3 = C0Val.magnitude / N.magnitude;
							
							if(Vector3.Dot(N, Vector3.Cross(edge0, C0)) > 0 && Vector3.Dot(N, Vector3.Cross(edge1, C1)) > 0 && Vector3.Dot(N, Vector3.Cross(edge2, C2)) > 0){
								//inside triangle;
								
								int texWidth = _texture.width;
								int texHeight = _texture.height;
								Vector2 map = (Value1 * UVs[0]) + (Value2 * UVs[1]) + (Value3 * UVs[2]);
								
								Color currentColor = _texture.GetPixel((int)(map.x * texWidth), (int)(map.y * texHeight));
								
								pixColor = currentColor;
							}
						}
					}
					
					if(triangle == 2){
						Vector3 vA = vertices[2];
						Vector3 vB = vertices[3];
						Vector3 vC = vertices[5];
					
						vA.Normalize();
						vB.Normalize();
						vC.Normalize();
					
						Vector3 N = Vector3.Cross((vA-vB), (vA-vC));
					
						float D = Vector3.Dot(N, vA);
						float nd = Vector3.Dot(N, RayDirection);
						if(nd != 0){
							float t = ((Vector3.Dot(N, RayOrigin)+D)/nd);
							
							Vector3 p = RayOrigin + t *RayDirection;
						
							Vector3 edge0 = vB - vA;
							Vector3 edge1 = vC - vB;
							Vector3 edge2 = vA - vC;
							
							Vector3 C0 = p - vA;
							Vector3 C1 = p - vB;
							Vector3 C2 = p - vC;
							
							Vector3 C0Val = Vector3.Cross(edge0, C0);
							Vector3 C1Val = Vector3.Cross(edge1, C1);
							Vector3 C2Val = Vector3.Cross(edge2, C2);
							
							float Value1 = C1Val.magnitude / N.magnitude;
							float Value2 = C2Val.magnitude / N.magnitude;
							float Value3 = C0Val.magnitude / N.magnitude;
							
							if(Vector3.Dot(N, Vector3.Cross(edge0, C0)) > 0 && Vector3.Dot(N, Vector3.Cross(edge1, C1)) > 0 && Vector3.Dot(N, Vector3.Cross(edge2, C2)) > 0){
								//inside triangle;
								
								int texWidth = _texture.width;
								int texHeight = _texture.height;
								Vector2 map = (Value1 * UVs[0]) + (Value2 * UVs[1]) + (Value3 * UVs[2]);
								
								Color currentColor = _texture.GetPixel((int)(map.x * texWidth), (int)(map.y * texHeight));
								
								pixColor = currentColor;
							}
						}
					}
					if(triangle == 3){
						Vector3 vA = vertices[2];
						Vector3 vB = vertices[5];
						Vector3 vC = vertices[4];
					
						vA.Normalize();
						vB.Normalize();
						vC.Normalize();
					
						Vector3 N = Vector3.Cross((vA-vB), (vA-vC));
					
						float D = Vector3.Dot(N, vA);
						float nd = Vector3.Dot(N, RayDirection);
						if(nd != 0){
							float t = ((Vector3.Dot(N, RayOrigin)+D)/nd);
							
							Vector3 p = RayOrigin + t *RayDirection;
						
							Vector3 edge0 = vB - vA;
							Vector3 edge1 = vC - vB;
							Vector3 edge2 = vA - vC;
							
							Vector3 C0 = p - vA;
							Vector3 C1 = p - vB;
							Vector3 C2 = p - vC;
							
							Vector3 C0Val = Vector3.Cross(edge0, C0);
							Vector3 C1Val = Vector3.Cross(edge1, C1);
							Vector3 C2Val = Vector3.Cross(edge2, C2);
							
							float Value1 = C1Val.magnitude / N.magnitude;
							float Value2 = C2Val.magnitude / N.magnitude;
							float Value3 = C0Val.magnitude / N.magnitude;
							
							if(Vector3.Dot(N, Vector3.Cross(edge0, C0)) > 0 && Vector3.Dot(N, Vector3.Cross(edge1, C1)) > 0 && Vector3.Dot(N, Vector3.Cross(edge2, C2)) > 0){
								//inside triangle;
								
								int texWidth = _texture.width;
								int texHeight = _texture.height;
								Vector2 map = (Value1 * UVs[0]) + (Value2 * UVs[1]) + (Value3 * UVs[2]);
								
								Color currentColor = _texture.GetPixel((int)(map.x * texWidth), (int)(map.y * texHeight));
								
								pixColor = currentColor;
							}
						}
					}
				}
				RayTracingResult.SetPixel(i, j, pixColor);
			}
		}
		
		
        RayTracingResult.Apply();
       
       
    }

    private void OnRenderImage(RenderTexture source, RenderTexture destination)
    {
        //Show the generated ray tracing image on screen
        Graphics.Blit(RayTracingResult, destination);
    }
>>>>>>> 17e2c9f0015b84508c0980fb762a587738dc55ae
}