#include <cmath> // Incluye esta biblioteca en lugar de <corecrt_math.h>
#include "Point.h"
#include <string>
#include <vector>
#include <sstream> // Asegúrate de incluir esta biblioteca
#include <SDL/include/SDL_stdinc.h>
#include <PugiXml/src/pugixml.hpp>

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
    // Calcula el angulo en radianes usando la funcion atan2
    double angle_rad = atan2(dir.y, dir.x);

    // Convierte el angulo de radianes a grados
    double angle_deg = angle_rad * 180.0 / M_PI;

    // Aseguate de que el angulo est?en el rango [0, 360)
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

inline pugi::xml_node find_child_by_attribute(pugi::xml_node parent, const char* name, const char* attr_name, const char* attr_value) {
    for (pugi::xml_node node : parent.children(name)) {
        if (std::strcmp(node.attribute(attr_name).value(), attr_value) == 0) {
            return node;
        }
    }
    return pugi::xml_node();
}


inline float easeOutBounce(float t) {
    if (t < 1 / 2.75) {
        return 7.5625 * t * t;
    }
    else if (t < 2 / 2.75) {
        t -= 1.5 / 2.75;
        return 7.5625 * t * t + 0.75;
    }
    else if (t < 2.5 / 2.75) {
        t -= 2.25 / 2.75;
        return 7.5625 * t * t + 0.9375;
    }
    else {
        t -= 2.625 / 2.75;
        return 7.5625 * t * t + 0.984375;
    }
}

inline float easeOutElastic(float t) {
    float c4 = (2 * M_PI) / 3;
    return t == 0 ? 0 : t == 1 ? 1 : pow(2, -10 * t) * sin((t * 10 - 0.75) * c4) + 1;
}

inline float linear(float t) { return t; }
inline float easeInQuad(float t) { return t * t; }
inline float easeInCubic(float t) { return t * t * t;}
inline float easeOutQuad(float t) { return t * (2 - t); }
inline float easeOutCubic(float t) { t--; return t * t * t + 1;}
inline float easeInOutQuad(float t) { return t < 0.5 ? 2 * t * t : -1 + (4 - 2 * t) * t; }
inline float easeInOutCubic(float t) { return t < 0.5 ? 4 * t * t * t : (t - 1) * (2 * t - 2) * (2 * t - 2) + 1; }
