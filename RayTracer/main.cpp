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
#include <iostream>
#include <fstream>
#include "sphere.h"
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
    auto half_b = dot(oc, r.direction());
    auto c = oc.length_squared() - radius*radius;
    auto discriminant = half_b*half_b - a*c;

    if (discriminant < 0) {
        return -1.0;
    } else {
        return (-half_b - sqrt(discriminant) ) / a;
    }
}

color ray_color(const ray& r, const hittable& world) {
    hit_record rec;
    if (world.hit(r, 0, infinity, rec)) {
        return 0.5 * (rec.normal + color(1,1,1));
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
    // World
    hittable_list world;
    world.add(make_shared<sphere>(point3(0,0,-1), 0.5));
    world.add(make_shared<sphere>(point3(0,-100.5,-1), 100));
    // Camera
    camera cam;
    
    ofstream out("tes1111t.ppm");
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
                pixel_color += ray_color(r, world);
            }
            write_color(out, pixel_color, samples_per_pixel);
        }
    }

    std::cerr << "\nDone.\n";
    out.close();
    return 0;
}
