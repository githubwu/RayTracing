//
//  main.cpp
//  RayTracer
//
//  Created by wujy on 2018/9/21.
//  Copyright © 2018年 wujy. All rights reserved.
//
#include "rtweekend.h"
#include "color.h"
#include "hittable_list.h"
#include "material.h"
#include "sphere.h"
#include <iostream>
#include <fstream>
#include "vec3.h"


using namespace std;

class camera{
public:
    camera(){
        lower_left_corner=vec3(-2.0,-1.0,-1.0);
        horizontal=vec3(4.0,0.0,0.0);
        vertical=vec3(0.0,2.0,0.0);
        origin=vec3(0.0,0.0,0.0);
    }
    ray get_ray(float u,float v){
        return ray(origin, lower_left_corner+u*horizontal+v*vertical-origin);
    }
    vec3 lower_left_corner;
    vec3 horizontal;
    vec3 vertical;
    vec3 origin;
};

double hit_sphere(const point3& center, double radius, const ray& r) {
    vec3 oc = r.origin() - center;
    auto a = r.direction().length_squared();
    auto half_b = dot(oc, r.direction()); // b = 2h 二元一次方程优化方法
    auto c = oc.length_squared() - radius*radius;
    auto discriminant = half_b*half_b - a*c;

    if (discriminant < 0) {
        return -1.0;
    } else {
        return (-half_b - sqrt(discriminant) ) / a;
    }
}

color ray_color(const ray& r, const hittable& world,int depth) {
    hit_record rec;
    if (depth <= 0)
        return color(0,0,0);
    
    if (world.hit(r, 0.01, infinity, rec)) {
        // diffuse material
//        point3 target = rec.p + random_in_hemisphere(rec.normal);
//        return 0.5 * ray_color(ray(rec.p, target - rec.p), world, depth-1);
        //metal
        ray scattered;
        color attenuation;
        if (rec.mat_ptr->scatter(r, rec, attenuation, scattered))
            return attenuation * ray_color(scattered, world, depth-1);
        return color(0,0,0);
    }
    
    vec3 unit_direction = unit_vector(r.direction());
    auto t = 0.5*(unit_direction.y() + 1.0);
    return (1.0-t)*color(1.0, 1.0, 1.0) + t*color(0.5, 0.7, 1.0);
}

int main() {
    // Image
    const auto aspect_ratio = 16.0 / 9.0;
    const int image_width = 400;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = 100;
    const int max_depth = 50;
    // World
    hittable_list world;
    auto material_ground = make_shared<lambertian>(color(0.8, 0.8, 0.0));
    auto material_center = make_shared<lambertian>(color(0.7, 0.3, 0.3));
    auto material_left   = make_shared<metal>(color(0.8, 0.8, 0.8), 0.5);
    auto material_right  = make_shared<metal>(color(0.8, 0.6, 0.2), 0.7);
    world.add(make_shared<sphere>(point3( 0.0, -100.5, -1.0), 100.0, material_ground));
    world.add(make_shared<sphere>(point3( 0.0,    0.0, -1.0),   0.5, material_center));
    world.add(make_shared<sphere>(point3(-1.0,    0.0, -1.0),   0.5, material_left));
    world.add(make_shared<sphere>(point3( 1.0,    0.0, -1.0),   0.5, material_right));
    // Camera
    camera cam;
    
    ofstream out("/Users/bytedance/Desktop/doing/tes1111t.ppm");
    out<<"P3\n"<<image_width<<" "<<image_height<<"\n255\n";
    // Render

    std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";

    for (int j = image_height-1; j >= 0; --j) {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) {
            color pixel_color(0, 0, 0);
            for (int s = 0; s < samples_per_pixel; ++s) {
                auto u = (i + random_double()) / (image_width-1);
                auto v = (j + random_double()) / (image_height-1);
                ray r = cam.get_ray(u, v);
                pixel_color += ray_color(r, world, max_depth);
            }
            write_color(out, pixel_color, samples_per_pixel);
        }
    }

    std::cerr << "\nDone.\n";
    out.close();
    return 0;
}
