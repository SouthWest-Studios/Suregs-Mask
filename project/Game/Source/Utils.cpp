#include <cmath> // Incluye esta biblioteca en lugar de <corecrt_math.h>
#include "Point.h"
#include <string>
#include <vector>
#include <sstream> // Asegúrate de incluir esta biblioteca
#include <SDL/include/SDL_stdinc.h>

inline float dist(int ax, int ay, int bx, int by) {
    return sqrt(pow(ax - bx, 2) + pow(ay - by, 2));
}

inline float dist(iPoint a, iPoint b) {
    return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
}

inline fPoint getDirectionVector(iPoint a, iPoint b) {

    int dx = b.x - a.x;
    int dy = b.y - a.y;
    double magnitude = sqrt(dx * dx + dy * dy);
    double normalized_dx = dx / magnitude;
    double normalized_dy = dy / magnitude;

    return fPoint(normalized_dx, normalized_dy);
}

inline double GetAngleFromDirection(fPoint dir) {
    // Calcula el ángulo en radianes usando la función atan2
    double angle_rad = atan2(dir.y, dir.x);

    // Convierte el ángulo de radianes a grados
    double angle_deg = angle_rad * 180.0 / M_PI;

    // Asegúrate de que el ángulo esté en el rango [0, 360)
    if (angle_deg < 0) {
        angle_deg += 360.0;
    }

    return angle_deg;
}

inline std::vector<int> splitToInts(const std::string& s, char delimiter) {
    std::vector<int> tokens;
    std::stringstream ss(s);
    std::string token;
    while (std::getline(ss, token, delimiter)) {
        tokens.push_back(std::stoi(token));
    }
    return tokens;
}