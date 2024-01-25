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

mat3 setCamera( in vec3 ro, in vec3 ta, float cr )
{
    vec3 cw = normalize(ta-ro);
    vec3 cp = vec3(sin(cr), cos(cr),0.0);
    vec3 cu = normalize( cross(cw,cp) );
    vec3 cv = normalize( cross(cu,cw) );
    return mat3( cu, cv, cw );
}

float map(in vec3 approach){    
    vec3 rot = vec3(sin(.3), cos(.1), 0.);
    vec3 top = normalize(cross(vec3(1.,0.,0.),rot));
    vec3 mid = normalize(cross(rot,vec3(0.,1.,0.)));
    vec3 bot = normalize(cross(rot,mid));
    mat3 rot_mat = mat3(
    	top, mid, bot
    );
    
    mat3 rot_mat_b = setCamera(normalize(-approach), vec3(0.), u_time/5.);
    
    vec3 offset = rot_mat_b*rot_mat*approach;
    
    return max(fIcosahedron(approach, 1.0),fIcosahedron(offset, 1.));
}


float castRay( in vec3 ro, in vec3 rd)
{
    float t = 0.;
    
    for( int i=0; i<100; i++ )
    {
   		float res = map( ro+rd*t);
   		if (res < 0.001)
            return t;
    	t += res;
        if (t > 5.)
            break;
    }

    return 1000.;
}



vec3 GetNormal(in vec3 from)
{
    vec2 delta = vec2(.01,.0);
    vec3 direction = map(from) - 
        vec3(map(from-delta.xyy), map(from-delta.yxy), map(from-delta.yyx));
    return normalize(direction);
}

void main() {
    vec2 st = gl_FragCoord.xy/u_resolution.xy -.5;
    
    float clearing = -3.0;
	vec3 ro = vec3(-sin(u_time/2.),cos(u_time/2.), clearing);
    vec3 ta = vec3(0.,0.,0.);
    mat3 ca = setCamera( ro, ta, u_time/2. );
    vec3 rd = ca * normalize(vec3(st.xy,1.14));
    
    vec3 color = vec3(0);
    float depth = castRay(ro, rd);
    vec3 source = vec3(10.,-10.,0.);
    vec3 key_l = vec3(-10., -13., 9.);
    if (depth<100.0){
        color = vec3(0.799,0.192,1.000);
    	vec3 point = ro+rd*depth;
    	vec3 world_normal = GetNormal(point);
    	vec3 normal = ro*world_normal;
        float edge_light = dot(normal+1.,source)/10.;
        float fill_light = dot(normal+1., key_l)/(length(key_l-point));
        //color = (normal+1.)/2.;
        color *= max(vec3(fill_light), color)/1.4;
        color -= min(vec3(edge_light), color)/1.3;
        
	}
    gl_FragColor = vec4(color,1.0);
}