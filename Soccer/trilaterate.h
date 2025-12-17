#ifndef TRILATERATE_UTILS_H
#define TRILATERATE_UTILS_H

#include "types.h"
#include <map>
#include <string>
#include <cmath>

const std::map<std::string, Punto> FIELD_FLAGS = {
    // Center and Goals
    {"f c", {0.0, 0.0}},   // centro del campo
    {"goal l", {-52.5, 0.0}}, // centro de la portería izquierda
    {"goal r", {52.5, 0.0}},  // centro de la portería derecha

    // Top flags (y = +34 + 5)
    {"f t l 50", {-50.0, 39.0}},
    {"f t l 40", {-40.0, 39.0}},
    {"f t l 30", {-30.0, 39.0}},
    {"f t l 20", {-20.0, 39.0}},
    {"f t l 10", {-10.0, 39.0}},
    {"f t 0", {0.0, 39.0}},
    {"f t r 10", {10.0, 39.0}},
    {"f t r 20", {20.0, 39.0}},
    {"f t r 30", {30.0, 39.0}},
    {"f t r 40", {40.0, 39.0}},
    {"f t r 50", {50.0, 39.0}},

    // Bottom flags (y = -34 - 5)
    {"f b l 50", {-50.0, -39.0}},
    {"f b l 40", {-40.0, -39.0}},
    {"f b l 30", {-30.0, -39.0}},
    {"f b l 20", {-20.0, -39.0}},
    {"f b l 10", {-10.0, -39.0}},
    {"f b 0", {0.0, -39.0}},
    {"f b r 10", {10.0, -39.0}},
    {"f b r 20", {20.0, -39.0}},
    {"f b r 30", {30.0, -39.0}},
    {"f b r 40", {40.0, -39.0}},
    {"f b r 50", {50.0, -39.0}},

    // Left side flags (x = -52.5 - 5)
    {"f l t 50", {-57.5, 34.0}},
    {"f l t 30", {-57.5, 30.0}},
    {"f l t 10", {-57.5, 10.0}},
    {"f l 0", {-57.5, 0.0}},
    {"f l b 10", {-57.5, -10.0}},
    {"f l b 30", {-57.5, -30.0}},
    {"f l b 50", {-57.5, -34.0}},

    // Right side flags (x = +52.5 + 5)
    {"f r t 50", {57.5, 34.0}},
    {"f r t 30", {57.5, 30.0}},
    {"f r t 10", {57.5, 10.0}},
    {"f r 0", {57.5, 0.0}},
    {"f r b 10", {57.5, -10.0}},
    {"f r b 30", {57.5, -30.0}},
    {"f r b 50", {57.5, -34.0}},

    // Penalty flags (virtual) — aproximados típicos
    {"f p l t", {-36.0, 18.0}},
    {"f p l b", {-36.0, -18.0}},
    {"f p r t", {36.0, 18.0}},
    {"f p r b", {36.0, -18.0}},

    // Goalpost virtual markers
    {"f g l t", {-52.5, 7.32 / 2}}, // alto izq portería (ancho ±7.32)
    {"f g l b", {-52.5, -7.32 / 2}},
    {"f g r t", {52.5, 7.32 / 2}},
    {"f g r b", {52.5, -7.32 / 2}}};

#endif