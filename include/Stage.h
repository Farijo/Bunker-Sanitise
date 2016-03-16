#ifndef STAGE_H
#define STAGE_H

#include "Room.h"

const double UI_RATIO = 0.2;

class Stage
{
    public:
        Stage();
        Stage(int rps);
        virtual ~Stage();

        void draw(sf::RenderWindow& window)const;
    protected:
        void drawMiniMap(sf::RenderWindow& window)const;

        int roomPerSide;
        Room* rooms;
};

#endif // STAGE_H
