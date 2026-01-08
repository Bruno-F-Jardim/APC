#pragma once
#include <array>
#include <vector>
#include <string>
#include <iostream>

struct Vec3 {
    double x{0};
    double y{0};
    double z{0};

    Vec3() = default;
    Vec3(double x_, double y_, double z_) : x(x_), y(y_), z(z_) {}
};

class Body {
public:
    std::string name;
    double mass;
    Vec3 position;
    Vec3 velocity;

    Body(const std::string& n, double m, const Vec3& pos, const Vec3& vel)
        : name(n), mass(m), position(pos), velocity(vel) {}

    Body() : name(""), mass(0), position(), velocity() {}

    void print() const {
        std::cout << "Name: " << name << "\n";
        std::cout << "Mass: " << mass << "\n";
        std::cout << "Position: (" << position.x << ", " << position.y << ", " << position.z << ")\n";
        std::cout << "Velocity: (" << velocity.x << ", " << velocity.y << ", " << velocity.z << ")\n";
    }
};

struct SoABodies {
    // ---------------------------------------------------------
    // "Hot" Data: Used frequently in the physics loop (integrateStep)
    // ---------------------------------------------------------
    
    // Positions (Split into component vectors for easy SIMD loading)
    std::vector<double> pos_x;
    std::vector<double> pos_y;
    std::vector<double> pos_z;

    // Masses (Needed for force calculation)
    std::vector<double> mass;

    // Velocities (Used in the update step)
    std::vector<double> vel_x;
    std::vector<double> vel_y;
    std::vector<double> vel_z;

    // Accelerations (Scratchpad for the current step)
    // Keeping this as a member member avoids re-allocating memory every time step
    std::vector<double> acc_x;
    std::vector<double> acc_y;
    std::vector<double> acc_z;

    // ---------------------------------------------------------
    // "Cold" Data: Rarely used (e.g., only for I/O or logging)
    // ---------------------------------------------------------
    std::vector<std::string> name;

    // ---------------------------------------------------------
    // Helper Methods
    // ---------------------------------------------------------

    // Reserve memory to prevent re-allocations during initialization
    void reserve(size_t n) {
        pos_x.reserve(n); pos_y.reserve(n); pos_z.reserve(n);
        vel_x.reserve(n); vel_y.reserve(n); vel_z.reserve(n);
        acc_x.reserve(n); acc_y.reserve(n); acc_z.reserve(n);
        mass.reserve(n);
        name.reserve(n);
    }

    // Convert an existing AoS list to this SoA layout
    void loadFromAoS(const std::vector<Body>& bodies) {
        size_t n = bodies.size();
        reserve(n);

        for (const auto& b : bodies) {
            push_back(b);
        }
    }

    void push_back(const Body& b) {
        pos_x.push_back(b.position.x);
        pos_y.push_back(b.position.y);
        pos_z.push_back(b.position.z);

        vel_x.push_back(b.velocity.x);
        vel_y.push_back(b.velocity.y);
        vel_z.push_back(b.velocity.z);

        mass.push_back(b.mass);
        name.push_back(b.name);
        
        // Initialize acceleration to 0
        acc_x.push_back(0.0);
        acc_y.push_back(0.0);
        acc_z.push_back(0.0);
    }

    size_t size() const { return pos_x.size(); }
};