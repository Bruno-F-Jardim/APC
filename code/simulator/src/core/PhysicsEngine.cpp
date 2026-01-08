#include "PhysicsEngine.hpp"
#include "Universe.hpp"
#include <vector>
#include <cmath>
#include <iostream>
#include <omp.h>

// -------------------- Vec3 helper functions --------------------
namespace {

Vec3 operator+(const Vec3& a, const Vec3& b) {
    return Vec3{a.x + b.x, a.y + b.y, a.z + b.z};
}

Vec3 operator-(const Vec3& a, const Vec3& b) {
    return Vec3{a.x - b.x, a.y - b.y, a.z - b.z};
}

Vec3 operator*(const Vec3& a, double s) {
    return Vec3{a.x * s, a.y * s, a.z * s};
}

Vec3& operator+=(Vec3& a, const Vec3& b) {
    a.x += b.x;
    a.y += b.y;
    a.z += b.z;
    return a;
}

} // anonymous namespace
// ---------------------------------------------------------------

void PhysicsEngine::advance(Universe& universe, double dt) {
    integrateStep(universe.getBodies(), dt);
}

void PhysicsEngine::integrateStep(SoABodies& bodies, double dt) {
    const double G = 6.67430e-11;
    size_t n = bodies.size();

    // Reset accelerations
    std::fill(bodies.acc_x.begin(), bodies.acc_x.end(), 0.0);
    std::fill(bodies.acc_y.begin(), bodies.acc_y.end(), 0.0);
    std::fill(bodies.acc_z.begin(), bodies.acc_z.end(), 0.0);

    for (size_t i = 0; i < n; ++i) {
        double ax = 0.0, ay = 0.0, az = 0.0;

        double px = bodies.pos_x[i];
        double py = bodies.pos_y[i];
        double pz = bodies.pos_z[i];

        #pragma omp simd reduction(+:ax, ay, az)
        for (size_t j = 0; j < n; ++j) {
            // if (i == j) continue; removed to optimize performance
            
            double rx = bodies.pos_x[j] - px;
            double ry = bodies.pos_y[j] - py;
            double rz = bodies.pos_z[j] - pz;
            
            double distSq = rx * rx + ry * ry + rz * rz + 1e-9;
            double inv_dist = 1.0 / std::sqrt(distSq);
            double inv_dist3 = inv_dist * inv_dist * inv_dist;
            double factor = G * bodies.mass[j] * inv_dist3;
            ax += factor * rx;
            ay += factor * ry;
            az += factor * rz;
            
        }
        bodies.acc_x[i] += ax;
        bodies.acc_y[i] += ay;
        bodies.acc_z[i] += az;
    }

    #pragma omp simd
    for (size_t i = 0; i < n; ++i) {
        bodies.vel_x[i] += bodies.acc_x[i] * dt;
        bodies.vel_y[i] += bodies.acc_y[i] * dt;
        bodies.vel_z[i] += bodies.acc_z[i] * dt;
        
        bodies.pos_x[i] += bodies.vel_x[i] * dt;
        bodies.pos_y[i] += bodies.vel_y[i] * dt;
        bodies.pos_z[i] += bodies.vel_z[i] * dt;
    }
}
