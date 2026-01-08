#include "outputWritter.hpp"
#include <iostream>
#include <iomanip>

OutputWriter::OutputWriter(const std::string& filename)
    : outFile_(filename), stepCounter_(0)
{
    if (!outFile_.is_open()) {
        std::cerr << "Error opening file: " << filename << "\n";
    } else {
        std::cout << "Created output TXT file: " << filename << "\n";
    }
}

OutputWriter::~OutputWriter() {
    if (outFile_.is_open()) outFile_.close();
}

void OutputWriter::writeStep(const SoABodies& bodies, double t) {
    if (!outFile_.is_open()) return;

    outFile_ << "# Step " << stepCounter_ << ", Time: " << t << " s\n";
    outFile_ << "Name Mass Px Py Pz Vx Vy Vz\n";

    size_t n = bodies.size();
    for (size_t i = 0; i < n; ++i) {
        outFile_ << std::setw(10) << bodies.name[i] << " "
                 << std::scientific << std::setprecision(6)
                 << bodies.mass[i] << " "
                 << bodies.pos_x[i] << " " << bodies.pos_y[i] << " " << bodies.pos_z[i] << " "
                 << bodies.vel_x[i] << " " << bodies.vel_y[i] << " " << bodies.vel_z[i] << "\n";
    }

    outFile_ << "\n";
    ++stepCounter_;
}
