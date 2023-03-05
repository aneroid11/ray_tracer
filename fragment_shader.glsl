#version 330 core

out vec4 frag_color;

in vec3 pl_pos;
in vec3 ray_direction;

const float FAR = 1000000000.0;
const float ZERO_SUBSTITUTE = 0.001;
const int NUM_SPHERE_OBJECTS = 4;
const int NUM_HORIZONTAL_PLANES = 2;
const int NUM_VERTICAL_PLANES_PERP_X = 2;
const int NUM_LIGHTS = 1;
const vec3 CAM_POS = vec3(0.0, 0.0, 0.0);
const float DIFFUSE_LIGHT = 0.1;

struct material {
	vec4 cl;
	int specular;
	float reflective;
};

struct sphere_object {
	vec3 position;
	float radius;
	material m;
};

struct horizontal_plane_object {
	float y;
	material m;
};

struct vertical_plane_perp_x {
	float x;
	material m;
};

struct light {
	vec3 position;
	float intensity;
};

sphere_object objects[NUM_SPHERE_OBJECTS];
horizontal_plane_object horizontal_planes[NUM_HORIZONTAL_PLANES];
vertical_plane_perp_x vertical_planes_perp_x[NUM_VERTICAL_PLANES_PERP_X];
light lights[NUM_LIGHTS];

void init_spheres() {
	material m;
	m.cl = vec4(0.6, 0.8, 0.2, 1.0);
	m.reflective = 0.3;
	m.specular = 50;
	
	objects[0].position = vec3(-1.0, 0.0, 10.0);
	objects[0].radius = 0.5;
	objects[0].m = m;
	
	objects[1].position = vec3(1.0, 0.0, 10.0);
	objects[1].radius = 0.5;
	objects[1].m = m;
	
	objects[2].position = vec3(0.0, -1.0, 10.0);
	objects[2].radius = 0.5;
	objects[2].m = m;
	
	objects[3].position = pl_pos + vec3(0.0, 0.22, 0.0);
	objects[3].radius = 0.2;
	objects[3].m = m;
	objects[3].m.cl = vec4(1.0, 0.0, 0.0, 1.0);
	objects[3].m.reflective = 0.0;
}

void init_horizontal_planes() {
	horizontal_planes[0].m.cl = vec4(1.0, 0.0, 0.5, 1.0);
	horizontal_planes[0].m.specular = -1;
	horizontal_planes[0].m.reflective = 0.0;
	horizontal_planes[0].y = 3.0f;
	
	horizontal_planes[1].m.cl = vec4(1.0, 0.0, 0.5, 1.0);
	horizontal_planes[1].m.specular = -1;
	horizontal_planes[1].m.reflective = 0.0;
	horizontal_planes[1].y = -1.0f;
}

void init_vertical_planes_perp_x() {
	vertical_planes_perp_x[0].x = 3.0;
	vertical_planes_perp_x[0].m.cl = vec4(0.0, 1.0, 0.0, 1.0);
	vertical_planes_perp_x[0].m.specular = -1;
	vertical_planes_perp_x[0].m.reflective = 0.4;
	
	vertical_planes_perp_x[1].x = -3.0;
	vertical_planes_perp_x[1].m.cl = vec4(1.0, 0.0, 0.0, 1.0);
	vertical_planes_perp_x[1].m.specular = -1;
	vertical_planes_perp_x[1].m.reflective = 0.0;
}

void init_lights() {
	lights[0].position = vec3(0.0f, 0.0f, 10.0f);
	lights[0].intensity = (1 - DIFFUSE_LIGHT) / NUM_LIGHTS;
	
	//lights[1].position = vec3(0.0f, 0.0f, 20.0f);
	//lights[1].intensity = (1 - DIFFUSE_LIGHT) / NUM_LIGHTS;
}

bool ray_intersects_sphere(vec3 r0, vec3 r1, sphere_object s, out float t, out vec3 normal_dir) {
	float a = r0.x - s.position.x;
	float b = r1.x - r0.x;
	float c = r0.y - s.position.y;
	float d = r1.y - r0.y;
	float e = r0.z - s.position.z;
	float f = r1.z - r0.z;
	
	float coef_a = b*b + d*d + f*f;
	float coef_b = 2*a*b + 2*c*d + 2*e*f;
	float coef_c = a*a + c*c + e*e - s.radius * s.radius;
	
	float discriminant = coef_b*coef_b - 4*coef_a*coef_c;
	
	if (discriminant < 0) {
		return false;
	}
	
	float t0, t1;
	t0 = (-coef_b - sqrt(discriminant)) / (2 * coef_a); 
	t1 = (-coef_b + sqrt(discriminant)) / (2 * coef_a); 
	
	if (t1 < ZERO_SUBSTITUTE) {
		return false;
	}
	if (t0 < ZERO_SUBSTITUTE) {
		if (t1 < ZERO_SUBSTITUTE) {
			return false;
		}
		
		t = t1;
		vec3 point = r0 + (r1 - r0) * t;
		normal_dir = point - s.position;
		return true;
	}
	
	t = t0;
	vec3 point = r0 + (r1 - r0) * t;
	normal_dir = point - s.position;
	return true;
}

bool ray_intersects_horizontal_plane(vec3 r0, vec3 r1, horizontal_plane_object hp, out float t, out vec3 normal_dir) {
	float a = (hp.y - r0.y) / (r1.y - r0.y);
	
	if (a < ZERO_SUBSTITUTE) {
		t = FAR;
		return false;
	}
	
	if (r1.y > r0.y) {
		normal_dir = vec3(0.0f, -1.0f, 0.0f);		
	} else {
		normal_dir = vec3(0.0f, 1.0f, 0.0f);
	}
	
	t = a;
	return true;
}

bool ray_intersects_vertical_plane_perp_x(vec3 r0, vec3 r1, vertical_plane_perp_x vp, out float t, out vec3 normal_dir) {
	float a = (vp.x - r0.x) / (r1.x - r0.x);
	
	if (a < ZERO_SUBSTITUTE) {
		t = FAR;
		return false;
	}
	
	if (r1.x > r0.x) {
		normal_dir = vec3(-1.0f, 0.0f, 0.0f);		
	} else {
		normal_dir = vec3(1.0f, 0.0f, 0.0f);
	}
	
	t = a;
	return true;
}

float compute_lighting(vec3 start, vec3 dir, vec3 normal_dir, int specular, float closest_object_t, vec3 object_to_camera) {
	float intensity = DIFFUSE_LIGHT;
	
	vec3 pos = start + dir * closest_object_t;
	vec3 normal = normalize(normal_dir);
	
	float t;
	vec3 n;
	
	for (int i = 0; i < NUM_LIGHTS; i++) {
		light current_light = lights[i];
		
		bool shadowed = false;
		
		for (int i = 0; i < NUM_SPHERE_OBJECTS; i++) {
			if (ray_intersects_sphere(pos, current_light.position, objects[i], t, n) && t < 1.0f) {
				shadowed = true;
				break;
			}
		}
		
		for (int i = 0; i < NUM_HORIZONTAL_PLANES; i++) {
			if (ray_intersects_horizontal_plane(pos, current_light.position, horizontal_planes[i], t, n) && t < 1.0f) {
				shadowed = true;
				break;
			}
		}
		
		for (int i = 0; i < NUM_VERTICAL_PLANES_PERP_X; i++) {
			if (ray_intersects_vertical_plane_perp_x(pos, current_light.position, vertical_planes_perp_x[i], t, n) && t < 1.0f) {
				shadowed = true;
				break;
			}
		}
		
		if (!shadowed) {
			vec3 light_dir = normalize(current_light.position - pos);
			
			float diff_coef = dot(normal, light_dir);
			if (diff_coef > 0) {
				intensity += current_light.intensity * diff_coef;
			}
			
			vec3 refl_dir = normalize(reflect(-light_dir, normal));
			object_to_camera = normalize(object_to_camera);
			
			if (specular > -1) {
				float refl_dir_dot_object_to_camera = dot(refl_dir, object_to_camera);
				
				if (refl_dir_dot_object_to_camera > 0.0) {
					float spec_coef = pow(refl_dir_dot_object_to_camera, specular);
					intensity += current_light.intensity * spec_coef;
				}
			} 
		}
	}
	
	if (intensity > 1.0) { return 1.0; }
	return intensity;
}

vec4 trace_ray(vec3 from, vec3 direction) {
	vec3 normal_dir;
	float closest_object_t = FAR;
	vec4 color;
	vec4 output_color = vec4(0, 0, 0, 0);
	int specular;
	float reflective = 0.0f;
	vec3 start = from;
	vec3 dir = direction;
	float local_color_intensity = 1.0f;
	
	for (int j = 0; j < 3; j++) {
		for (int i = 0; i < NUM_SPHERE_OBJECTS; i++) {
			float t;
			vec3 n;		
			
			if (ray_intersects_sphere(start, start + dir, objects[i], t, n)) {
				if (t < closest_object_t) {
					closest_object_t = t;
					normal_dir = n;
					color = objects[i].m.cl;
					specular = objects[i].m.specular;
					reflective = objects[i].m.reflective;
				}
			}
		}
		
		for (int i = 0; i < NUM_HORIZONTAL_PLANES; i++) {			
			float t;
			vec3 n;
			
			if (ray_intersects_horizontal_plane(start, start + dir, horizontal_planes[i], t, n)) {
				if (t < closest_object_t) {
					closest_object_t = t;
					normal_dir = n;
					color = horizontal_planes[i].m.cl;
					specular = horizontal_planes[i].m.specular;
					reflective = horizontal_planes[i].m.reflective;
				}
			}
		}
		
		for (int i = 0; i < NUM_VERTICAL_PLANES_PERP_X; i++) {			
			float t;
			vec3 n;
			
			if (ray_intersects_vertical_plane_perp_x(start, start + dir, vertical_planes_perp_x[i], t, n)) {
				if (t < closest_object_t) {
					closest_object_t = t;
					normal_dir = n;
					color = vertical_planes_perp_x[i].m.cl;
					specular = vertical_planes_perp_x[i].m.specular;
					reflective = vertical_planes_perp_x[i].m.reflective;
				}
			}
		}
		
		if (closest_object_t < FAR - ZERO_SUBSTITUTE) {
			if (reflective <= ZERO_SUBSTITUTE) {
				float light_intensity = compute_lighting(start, dir, normal_dir, specular, closest_object_t, -direction);
				output_color += color * light_intensity * local_color_intensity;
				break;
			}
			else { 
				float light_intensity = compute_lighting(start, dir, normal_dir, specular, closest_object_t, -direction);
				output_color += (1.0 - reflective) * color * light_intensity;
				local_color_intensity -= (1.0 - reflective);
				vec3 refl = reflect(dir, normalize(normal_dir));
				vec3 new_point = start + dir * closest_object_t;
				closest_object_t = FAR;
				start = new_point;
				dir = refl;
			}
		}
	}
	
	return output_color;
}

void main() {
	init_lights();
	init_spheres();
	init_horizontal_planes();
	init_vertical_planes_perp_x();
	frag_color = trace_ray(pl_pos, ray_direction);
}
