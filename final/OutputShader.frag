#define TAU 6.28318530718

#define FAR 25.
#define ITER 128
#define QUA .001
#define NORK 5e-4

#define REF 0.56
#define RITER 16
#define RRFB .001

#define BRG 0.83
uniform float iTime;
uniform vec2 iResolution;
uniform int iFrame;
uniform vec4 iMouse;
struct ray {
    vec3 origin;
    vec3 direction;
};
float map(vec3 p)
{
float d = FAR;
d = min(d, length(p-vec3(0.75,vec2(.0)))-1.);	// center ball
d = min(d, 5. - abs(p.x));						// left & right wall
d = min(d, 3. - abs(p.y - 2.));					// floor & ceil
d = min(d, 5. - abs(p.z));						// front & back wall
return d;
}

vec3 normal(vec3 p, float k)
{
vec2 e = vec2(.0,k);
return normalize(vec3(
map(p) - map(p - e.yxx),
map(p) - map(p - e.xyx),
map(p) - map(p - e.xxy)
));
}

float tracer(vec3 ro, vec3 rd)
{
float t = .0, d;
for(int i=0; i < ITER; i++){
d = map(ro + rd*t);
if (d < QUA*t || t > FAR) break;
t += d;
}
return t + QUA;
}

float hard_shadow(vec3 sp, vec3 lp, int iter)
{
vec3 p, ld = normalize(sp - lp);

float t = .0;
for(int i = 0; i < iter; i++)
{
p = lp + ld * t;
float m = map(p);
if (m < .0 || t > FAR) break;
t += m;
}

return max(1. - distance(p, sp), 0.);
}

vec3 color(vec3 sp, vec3 sn)
{
vec3 lp = vec3(-1, 1, -2);
vec3 ld = normalize(lp - sp);

float diff = max(dot(sn,ld),0.);
float shd = hard_shadow(sp, lp, 30);

vec3 col = (sn.xyz+1.)/2.;
return col * diff * shd;
}

void main()
{
    vec3 horizontal =   vec3(-1.0, 0.0, 0.0);
    vec3 vertical   =   vec3(0.0, -1.0, 0.0);
    vec3 origin   =  vec3(.2 + cos(iTime * 2.) * .05, sin(iTime) * .01,-.1 - sin(iTime) * .1);
    vec3 center    =  vec3(0.0, 0.0, -1.0);
    vec2 uv = (2.*gl_FragCoord.xy-iResolution.xy)/iResolution.y;
    // 			if(fract((iTime-5.)/20.) < 0.5) uv.x *= -1.0;

    vec3 ro = vec3(abs(10. - mod(-5.,20.)) - 5., .0, -3.);
    vec3 rd = normalize(vec3(uv,2.));

    float t = tracer(ro,rd);
    vec3 sp = ro + rd*t;
    vec3 sn = normal(sp,NORK);

    vec3 col = color(-sp, -sn);
//     vec3 col = color(-())
    for(int i=0; i<RITER; i++)
    {
        if(t > FAR) break;

        rd = normalize(reflect(rd, sn));
        ro = sp + rd * RRFB;

        t = tracer(ro, rd);

        sp = ro + rd * (t - RRFB);
        sn = normal(sp,NORK);
//         ray r = ray(origin,
//             center +
//             uv.x * horizontal
//              + uv.y * vertical
//              + vec3((cos(float(i)/float(N) * 3.1415) * 0.001), (sin(float(i)/float(N) * 3.1415) * 0.001),0.0)
//         );

        col += color(sp, sn) * pow(REF, float(i) + 1.);
//         col += color(r, sn) * pow(REF, float(i) + 1.);
                                      }

    gl_FragColor = BRG * vec4(col,1.);
}
