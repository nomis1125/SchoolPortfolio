<<<<<<< HEAD
﻿Shader "Unlit/SphereShader"
{
	Properties
	{
		_MainTex ("Texture", 2D) = "white" {}
		_Tex1("Texture1", 2D) = "white"{}
		_Tex2("Texture2", 2D) = "white"{}
		_Tex3("Texture3", 2D) = "white"{}
		_Thresholds("Height Thresholds", vector) = (0, 0, 0, 0)
	}
	SubShader
	{
		Tags { "RenderType"="Opaque" }
		LOD 100

		Pass
		{
			CGPROGRAM
			#pragma vertex vert
			#pragma fragment frag
			
			#include "UnityCG.cginc"

			struct appdata
			{
				float4 vertex : POSITION;
				float2 uv : TEXCOORD0;
			};

			struct v2f
			{
				float2 uv : TEXCOORD0;
				float height : TEXCOORD1;
				UNITY_FOG_COORDS(1)
				float4 vertex : SV_POSITION;
			};

			sampler2D _MainTex;
			//added for
			sampler2D _Tex1;
			sampler2D _Tex2;
			sampler2D _Tex3;

			float4 _MainTex_ST;

			float4 _Thresholds;
			
			v2f vert (appdata v)
			{
				v2f o;
				o.vertex = UnityObjectToClipPos(v.vertex);
				o.height = v.vertex.y;
				o.uv = TRANSFORM_TEX(v.uv, _MainTex);

				return o;
			}

			fixed4 frag (v2f i) : SV_Target
			{
				// sample the texture
				fixed4 col0 = tex2D(_MainTex, i.uv);
				fixed4 col1 = tex2D(_Tex1, i.uv);
				fixed4 col2 = tex2D(_Tex2, i.uv);
				fixed4 col3 = tex2D(_Tex3, i.uv);

				if (i.height < _Thresholds.x) return col0;
				else if (i.height < _Thresholds.y) return col1;
				else if (i.height < _Thresholds.z) return col2;
				else return col3;
			}
			ENDCG
		}
	}
}
=======
﻿Shader "Unlit/SphereShader"
{
	Properties
	{
		_MainTex ("Texture", 2D) = "white" {}
		_Tex1("Texture1", 2D) = "white"{}
		_Tex2("Texture2", 2D) = "white"{}
		_Tex3("Texture3", 2D) = "white"{}
		_Thresholds("Height Thresholds", vector) = (0, 0, 0, 0)
	}
	SubShader
	{
		Tags { "RenderType"="Opaque" }
		LOD 100

		Pass
		{
			CGPROGRAM
			#pragma vertex vert
			#pragma fragment frag
			
			#include "UnityCG.cginc"

			struct appdata
			{
				float4 vertex : POSITION;
				float2 uv : TEXCOORD0;
			};

			struct v2f
			{
				float2 uv : TEXCOORD0;
				float height : TEXCOORD1;
				UNITY_FOG_COORDS(1)
				float4 vertex : SV_POSITION;
			};

			sampler2D _MainTex;
			//added for
			sampler2D _Tex1;
			sampler2D _Tex2;
			sampler2D _Tex3;

			float4 _MainTex_ST;

			float4 _Thresholds;
			
			v2f vert (appdata v)
			{
				v2f o;
				o.vertex = UnityObjectToClipPos(v.vertex);
				o.height = v.vertex.y;
				o.uv = TRANSFORM_TEX(v.uv, _MainTex);

				return o;
			}

			fixed4 frag (v2f i) : SV_Target
			{
				// sample the texture
				fixed4 col0 = tex2D(_MainTex, i.uv);
				fixed4 col1 = tex2D(_Tex1, i.uv);
				fixed4 col2 = tex2D(_Tex2, i.uv);
				fixed4 col3 = tex2D(_Tex3, i.uv);

				if (i.height < _Thresholds.x) return col0;
				else if (i.height < _Thresholds.y) return col1;
				else if (i.height < _Thresholds.z) return col2;
				else return col3;
			}
			ENDCG
		}
	}
}
>>>>>>> 17e2c9f0015b84508c0980fb762a587738dc55ae
