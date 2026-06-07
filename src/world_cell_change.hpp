#pragma once

class Particle;

struct WorldCellChange {
        int x;
        int y;
        const Particle* particle;
};
