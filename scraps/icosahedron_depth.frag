// Author:
// Title:

#ifdef GL_ES
precision mediump float;
#endif

#define TAU (2*PI)
#define PHI (1.618033988749895)
#define GDFVector0 vec3(1, 0, 0)
#define GDFVector1 vec3(0, 1, 0)
#define GDFVector2 vec3(0, 0, 1)

#define GDFVector3 normalize(vec3(1, 1, 1 ))
#define GDFVector4 normalize(vec3(-1, 1, 1))
#define GDFVector5 normalize(vec3(1, -1, 1))
#define GDFVector6 normalize(vec3(1, 1, -1))

#define GDFVector7 normalize(vec3(0, 1, PHI+1.))
#define GDFVector8 normalize(vec3(0, -1, PHI+1.))
#define GDFVector9 normalize(vec3(PHI+1., 0, 1))
#define GDFVector10 normalize(vec3(-PHI-1., 0, 1))
#define GDFVector11 normalize(vec3(1, PHI+1., 0))
#define GDFVector12 normalize(vec3(-1, PHI+1., 0))

#define fGDFBegin float d = 0.;

// Version with variable exponent.
// This is slow and does not produce correct distances, but allows for bulging of objects.
#define fGDFExp(v) d += pow(abs(dot(p, v)), e);

// Version with without exponent, creates objects with sharp edges and flat faces
#define fGDF(v) d = max(d, abs(dot(p, v)));

#define fGDFExpEnd return pow(d, 1./e) - r;
#define fGDFEnd return d - r;

// Primitives follow:

float fIcosahedron(vec3 p, float r) {
fGDFBegin
    fGDF(GDFVector3) fGDF(GDFVector4) fGDF(GDFVector5) fGDF(GDFVector6)
    fGDF(GDFVector7) fGDF(GDFVector8) fGDF(GDFVector9) fGDF(GDFVector10)
    fGDF(GDFVector11) fGDF(GDFVector12)
    fGDFEnd
}

uniform vec2 u_resolution;
uniform vec2 u_mouse;
uniform float u_time;

float map(in vec3 approach, in float scale){
    return fIcosahedron(approach, scale);
}


float castRay( in vec3 ro, in vec3 rd, in float scale )
{
    float t = 0.;
    
    for( int i=0; i<10; i++ )
    {
   		float res = map( ro+rd*t, scale);
   		if (res < 0.001)
            break;
    	t += res;
        if (t > 5.)
            break;
    }

    return t;
}

mat3 setCamera( in vec3 ro, in vec3 ta, float cr )
{
    vec3 cw = normalize(ta-ro);
    vec3 cp = vec3(sin(cr), cos(cr),0.0);
    vec3 cu = normalize( cross(cw,cp) );
    vec3 cv = normalize( cross(cu,cw) );
    return mat3( cu, cv, cw );
}


void main() {
    vec2 st = gl_FragCoord.xy/u_resolution.xy -.5;
    
    float clearing = -3.0;
	vec3 ro = vec3(-sin(u_time),cos(u_time), clearing);
    vec3 ta = vec3(0.,0.,0.);
    mat3 ca = setCamera( ro, ta, u_time );
    vec3 rd = ca * normalize(vec3(st.xy,1.14));
    
    float depth = abs(castRay(ro, rd, 1.)/clearing);
    gl_FragColor = vec4(depth,depth,depth,1.0);
}