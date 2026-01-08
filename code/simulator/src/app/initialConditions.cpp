#include "initialConditions.hpp"
#include <cmath>
#include <random>
#include <iostream>

namespace {

std::random_device rd;
std::mt19937 gen(rd());

Body generateBody(const std::string& name, double mass, double distance, double velocity) {
    return Body{name, mass, Vec3{distance, 0.0, 0.0}, Vec3{0.0, velocity, 0.0}};
}

Body generateRandomSmallBody(size_t index) {
    // Constants
    const double AU = 1.496e11;
    const double G = 6.67430e-11;
    const double M_sun = 1.989e30;

    // Distance 0.3 AU to 1 AU
    std::uniform_real_distribution<> distDist(0.3*AU, 1.0*AU);
    double distance = distDist(gen);

    // Orbital velocity (circular orbit)
    double velocity = std::sqrt(G * M_sun / distance);

    // Random angles
    std::uniform_real_distribution<> angleTheta(0, 2*M_PI);
    std::uniform_real_distribution<> anglePhi(0, M_PI);
    double theta = angleTheta(gen);
    double phi = anglePhi(gen);

    // Random perturbation for velocity z-component
    std::uniform_real_distribution<> velPert(-0.001*velocity, 0.001*velocity);

    Vec3 pos{
        distance * std::cos(theta),
        distance * std::sin(theta),
        distance * std::sin(phi) * 0.1
    };
    Vec3 vel{
        -velocity * std::sin(theta),
        velocity * std::cos(theta),
        velPert(gen)
    };

    // Random mass between 1e10 and 1e18 kg
    std::uniform_real_distribution<> massDist(1e10, 1e18);
    double mass = massDist(gen);

    return Body{"Asteroid_" + std::to_string(index), mass, pos, vel};
}

} // anonymous namespace

SoABodies generateInitialConditions(size_t numberOfBodies) {
    SoABodies bodies;
    bodies.reserve(numberOfBodies);

    // --- Sun + planets ---
    bodies.push_back(Body{"Sun", 1.989e30, Vec3{0,0,0}, Vec3{0,0,0}});
    bodies.push_back(generateBody("Mercury", 3.301e23, 5.79e10, 47870));
    bodies.push_back(generateBody("Venus", 4.867e24, 1.082e11, 35020));
    bodies.push_back(generateBody("Earth", 5.972e24, 1.496e11, 29780));
    bodies.push_back(generateBody("Mars", 6.417e23, 2.279e11, 24130));
    bodies.push_back(generateBody("Jupiter", 1.898e27, 7.785e11, 13070));
    bodies.push_back(generateBody("Saturn", 5.683e26, 1.433e12, 9680));
    bodies.push_back(generateBody("Uranus", 8.681e25, 2.877e12, 6800));
    bodies.push_back(generateBody("Neptune", 1.024e26, 4.503e12, 5430));

    // --- Real moons ---
    struct MoonData { std::string name; double mass, distance, velocity; };
    std::vector<MoonData> moons = {
        {"Moon", 7.342e22, 3.84e8, 1022},
        {"Phobos", 1.07e16, 9.378e6, 2138},
        {"Deimos", 1.48e15, 2.3459e7, 1351},
        {"Io", 8.93e22, 4.217e8, 17320},
        {"Europa", 4.8e22, 6.711e8, 13740},
        {"Ganymede", 1.48e23, 1.07e9, 10880},
        {"Callisto", 1.08e23, 1.882e9, 8200},
        {"Titan", 1.345e23, 1.222e9, 5570},
        {"Rhea", 2.31e21, 5.27e8, 8480},
        {"Iapetus", 1.81e21, 3.56e9, 3260},
        {"Dione", 1.1e21, 3.77e8, 10160},
        {"Tethys", 6.17e20, 2.95e8, 11350},
        {"Enceladus", 1.08e20, 2.38e8, 12370},
        {"Mimas", 3.75e19, 1.86e8, 14200},
        {"Miranda", 6.59e19, 1.29e8, 6640},
        {"Ariel", 1.35e21, 1.91e8, 5560},
        {"Umbriel", 1.17e21, 2.66e8, 4660},
        {"Titania", 3.42e21, 4.36e8, 3640},
        {"Oberon", 3.0e21, 5.83e8, 3150},
        {"Triton", 2.14e22, 3.55e8, 4390}
    };

    for (const auto& moon : moons) {
        // Find parent planet position
        Vec3 parentPos{0,0,0};
        Vec3 parentVel{0,0,0};
        
        // Use direct vector access since we are building SoA
        // Be careful about indices. We assume the bodies are added in order above.
        // Sun=0, Mercury=1, Venus=2, Earth=3, Mars=4, Jupiter=5, Saturn=6, Uranus=7, Neptune=8
        
        if (moon.name == "Moon") { 
             parentPos = Vec3{bodies.pos_x[3], bodies.pos_y[3], bodies.pos_z[3]}; 
             parentVel = Vec3{bodies.vel_x[3], bodies.vel_y[3], bodies.vel_z[3]}; 
        } 
        else if (moon.name == "Phobos" || moon.name == "Deimos") { 
             parentPos = Vec3{bodies.pos_x[4], bodies.pos_y[4], bodies.pos_z[4]}; 
             parentVel = Vec3{bodies.vel_x[4], bodies.vel_y[4], bodies.vel_z[4]}; 
        }
        else if (moon.name == "Io" || moon.name == "Europa" || moon.name == "Ganymede" || moon.name == "Callisto") { 
             parentPos = Vec3{bodies.pos_x[5], bodies.pos_y[5], bodies.pos_z[5]}; 
             parentVel = Vec3{bodies.vel_x[5], bodies.vel_y[5], bodies.vel_z[5]}; 
        }
        else if (moon.name == "Titan" || moon.name == "Rhea" || moon.name == "Iapetus" || moon.name == "Dione" || moon.name == "Tethys" || moon.name == "Enceladus" || moon.name == "Mimas") { 
             parentPos = Vec3{bodies.pos_x[6], bodies.pos_y[6], bodies.pos_z[6]}; 
             parentVel = Vec3{bodies.vel_x[6], bodies.vel_y[6], bodies.vel_z[6]}; 
        }
        else if (moon.name == "Miranda" || moon.name == "Ariel" || moon.name == "Umbriel" || moon.name == "Titania" || moon.name == "Oberon") { 
             parentPos = Vec3{bodies.pos_x[7], bodies.pos_y[7], bodies.pos_z[7]}; 
             parentVel = Vec3{bodies.vel_x[7], bodies.vel_y[7], bodies.vel_z[7]}; 
        }
        else if (moon.name == "Triton") { 
             parentPos = Vec3{bodies.pos_x[8], bodies.pos_y[8], bodies.pos_z[8]}; 
             parentVel = Vec3{bodies.vel_x[8], bodies.vel_y[8], bodies.vel_z[8]}; 
        }

        double theta = ((double) rand() / RAND_MAX) * 2 * M_PI;
        Vec3 moonPos = {
            parentPos.x + moon.distance * cos(theta),
            parentPos.y + moon.distance * sin(theta),
            parentPos.z
        };

        Vec3 moonVel = {
            parentVel.x - moon.velocity * sin(theta),
            parentVel.y + moon.velocity * cos(theta),
            parentVel.z
        };
        bodies.push_back(Body{moon.name, moon.mass, moonPos, moonVel});
    }


    // --- Fill with random asteroids/comets ---
    size_t existingCount = bodies.size();
    size_t toAdd = (numberOfBodies > existingCount) ? (numberOfBodies - existingCount) : 0;

    for (size_t i = 0; i < toAdd; ++i) {
        bodies.push_back(generateRandomSmallBody(i+1));
    }

    return bodies;
}
