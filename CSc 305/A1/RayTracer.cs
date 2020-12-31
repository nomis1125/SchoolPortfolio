<<<<<<< HEAD
﻿using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEditor;

public class RayTracer : MonoBehaviour {

    public Texture2D texture_on_cube;

    Texture2D RayTracingResult;

  
    void Start () {

        Camera this_camera = gameObject.GetComponent<Camera>();
        Debug.Assert(this_camera);

        int pixel_width = this_camera.pixelWidth;
        int pixel_height = this_camera.pixelHeight;

        RayTracingResult = new Texture2D(pixel_width, pixel_height);

        #region Generate a black and white checker pattern
        
        for (int i = 0; i < pixel_width; ++ i)
            for (int j = 0; j < pixel_height; ++ j)
            {
                int iblock = i / 50;
                int jblock = j / 50;
                RayTracingResult.SetPixel(i, j, 
                    (iblock + jblock) % 2 == 0 ? Color.black : Color.white);
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
}
=======
﻿using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEditor;

public class RayTracer : MonoBehaviour {

    public Texture2D texture_on_cube;

    Texture2D RayTracingResult;

  
    void Start () {

        Camera this_camera = gameObject.GetComponent<Camera>();
        Debug.Assert(this_camera);

        int pixel_width = this_camera.pixelWidth;
        int pixel_height = this_camera.pixelHeight;

        RayTracingResult = new Texture2D(pixel_width, pixel_height);

        #region Generate a black and white checker pattern
        
        for (int i = 0; i < pixel_width; ++ i)
            for (int j = 0; j < pixel_height; ++ j)
            {
                int iblock = i / 50;
                int jblock = j / 50;
                RayTracingResult.SetPixel(i, j, 
                    (iblock + jblock) % 2 == 0 ? Color.black : Color.white);
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
}
>>>>>>> 17e2c9f0015b84508c0980fb762a587738dc55ae
