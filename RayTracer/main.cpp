//
//  main.cpp
//  RayTracer
//
//  Created by wujy on 2018/9/21.
//  Copyright © 2018年 wujy. All rights reserved.
//

#include <iostream>
#include <fstream>
#include "sphere.h"
#include "hitable_list.h"

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

vec3 random_in_unit_sphere(){
    vec3 p;
    do{
        p=2.0*vec3(drand48(),drand48(),drand48())-vec3(1,1,1);
    }while(p.squared_length()>=1.0);
    return p;
}

vec3 color(const ray& r,hitable* world){
    hit_record rec;
    if(world->hit(r, 0.001, MAXFLOAT, rec)){
        return 0.5*color(ray(rec.p,rec.normal+random_in_unit_sphere()), world);
    }else{
        vec3 unit_direction = unit_vector(r.direction());
        float t = 0.5*(unit_direction.y()+1.0);
        return (1.0-t)*vec3(1.0,1.0,1.0)+t*vec3(0.5,0.7,1.0);
    }
}

int main() {
    int nx = 200;
    int ny = 100;
    int ns=100;
    ofstream out("testimgttttt111111111ttt.ppm");
    out<<"P3\n"<<nx<<" "<<ny<<"\n255\n";
    
    hitable *list[2];
    list[0] = new sphere(vec3(0, 0, -1),0.5);
    list[1] = new sphere(vec3(0,-100.5,-1),100);
    hitable *world = new hitable_list(list,2);
    camera cam;
    for(int j=ny-1;j>=0;j--){
        for(int i=0;i<nx;i++){
            vec3 col(0,0,0);
            for(int s=0;s<ns;s++){
                float u=float(i+drand48())/float(nx);
                float v=float(j+drand48())/float(ny);
                ray r=cam.get_ray(u, v);
                vec3 p=r.point_at_parameter(2.0);
                col+=color(r,world);
            }
            col/=float(ns);
            col=vec3(sqrt(col[0]),sqrt(col[1]),sqrt(col[2]));   //gamma correction
            int ir=int(255.99*col[0]);
            int ig=int(255.99*col[1]);
            int ib=int(255.99*col[2]);
            out<<ir<<" "<<ig<<" "<<ib<<"\n";
        }
    }
    out.close();
    return 0;
}
