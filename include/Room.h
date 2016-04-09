#ifndef ROOM_H
#define ROOM_H

#include <SFML/Graphics.hpp>
#include <queue>

const int HAUTEUR_MIN = 4;
const int HAUTEUR_VARIANCE = 2;
const int LARGEUR_MIN = 2;
const int LARGEUR_VARIANCE = 4;
const double epsilon = 0.1;

class Room
{
    public:
        enum directions
        {
            UNSET,INACCESSIBLE,DEPART,LEFT,RIGHT,TOP,BOT
        };

        struct IntersectionPoint
        {
          sf::Vector2f position;
          int distance;
          int wall;

          IntersectionPoint();
          IntersectionPoint(double x, double y, int d, int w);
          IntersectionPoint(const sf::Vector2f& pos, int d, int w);
          bool operator <(const struct IntersectionPoint& b)const;
        };

        struct LinearEquation
        {
          enum types
          {
            DROITE, DEMIDROITE, SEGMENT
          };

          double a, b, c; // ax+by+c=0
          types type;
          sf::Vector2f extremity1, extremity2;

          LinearEquation();
          LinearEquation(const sf::Vector2f& point1, const sf::Vector2f& point2);
          LinearEquation(const sf::Vector2f& extrem1, const sf::Vector2f& extrem2, types t);
          bool operator ==(const struct LinearEquation& other)const;
          bool operator !=(const struct LinearEquation& other)const;
          bool findIntersection(const struct LinearEquation& other, sf::Vector2f& intersection)const;
          bool pointBelongToEquation(const sf::Vector2f& point)const;
        };

        Room();
        virtual ~Room();

        void generateRandomRoom();
        void loadSpecificRoom();
        void resetPath();
        void pathTo(const int x, const int y);
        void drawShape(sf::RenderWindow& window)const;
        void draw(sf::RenderWindow& window)const;
        void windowResize(const unsigned int width, const unsigned int height);
    protected:
        void createRealSizeVertex(sf::Vector2u* res)const;
        void createWall(const sf::Vector2u* realSizeVertex, struct LinearEquation* res)const;
        bool isPointInRoom(const sf::Vector2f& point, struct LinearEquation* wall)const;

        static std::vector<IntersectionPoint> triangleCorner(const sf::Vector2f mouse_pos, const sf::Vector2u* realSizeVertex, const struct LinearEquation* wall, const unsigned int nbVertex);
        static std::vector<std::vector<sf::Vector2f> > polygonList(const sf::Vector2f* vertex, const bool* vertexLight, const int nbVertex);
        static std::vector<IntersectionPoint> collisionGestion(const unsigned int length, const std::vector<struct IntersectionPoint>* intersectionList);
        static int triangulateShape(const std::vector<sf::Vector2f>& angles, sf::Vertex* verticeTrianguled);
        static bool isPointInTriangle(const sf::Vector2f point, const sf::Vector2f t1, const sf::Vector2f t2, const sf::Vector2f t3);

        sf::Texture* tex;
        int hauteur;
        int largeur;
        unsigned int size_x;
        unsigned int size_y;
        directions** roomArchitecture;
        std::vector<sf::Vector2u> vertices;
};

#endif // ROOM_H
