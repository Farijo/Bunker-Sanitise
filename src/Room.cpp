  #include "Room.h"
  #include "Utility.h"
  #include <stdlib.h>
  #include <list>
  #include <cmath>

  Room::IntersectionPoint::IntersectionPoint()
  {
    position = sf::Vector2f(-1.0,-1.0);
    wall = -1;
  }

  Room::IntersectionPoint::IntersectionPoint(double x, double y, int d, int w)
  {
    position = sf::Vector2f(x,y);
    distance = d;
    wall = w;
  }

  Room::IntersectionPoint::IntersectionPoint(const sf::Vector2f& pos, int d, int w)
  {
    position = pos;
    distance = d;
    wall = w;
  }

  bool Room::IntersectionPoint::operator <(const struct IntersectionPoint& b)const
  {
    return distance<b.distance;
  }

  Room::LinearEquation::LinearEquation()
  {

  }

  Room::LinearEquation::LinearEquation(const sf::Vector2f& point1, const sf::Vector2f& point2)
  {
    if(point1.x == point2.x)
    {
      a = 1.0;
      b = 0.0;
      c = -point1.x;
    }
    else
    {
      a = (point2.y-point1.y)/(point1.x-point2.x);
      b = 1.0;
      c = -a*point1.x - point1.y;
    }
    type = DROITE;
  }

  Room::LinearEquation::LinearEquation(const sf::Vector2f& extrem1, const sf::Vector2f& extrem2, types t) : LinearEquation(extrem1, extrem2)
  {
    if(t != DROITE)
    {
      type = t;
      extremity1 = extrem1;
      extremity2 = extrem2;
    }
  }

  bool Room::LinearEquation::operator ==(const struct LinearEquation& other)const
  {
    return (this->a == other.a)&&(this->b == other.b)&&(this->c == other.c);
  }

  bool Room::LinearEquation::operator !=(const struct LinearEquation& other)const
  {
    return !(*this == other);
  }

  bool Room::LinearEquation::findIntersection(const struct LinearEquation& other, sf::Vector2f& intersection)const
  {
    if(this->b == other.b)
    {
      if(this->a == other.a)
      {
        if(this->c != other.c)  // parallèles mais distinctes
          return false;
        else                    // parallèles et confondues
        {
          if(this->type == DROITE)
          {
            if(other.type == DROITE)
            {
              if(this->b == 0)
              {
                intersection.x = -this->c;
                intersection.y = 0.0;
              }
              else
              {
                intersection.x = 0.0;
                intersection.y = -this->c/this->b;
              }
            }
            else
            {
              intersection = other.extremity1;
            }
          }
          else if(this->type == DEMIDROITE)
          {
            if(other.type == DROITE)
            {
              intersection = this->extremity1;
            }
            else if(other.type == DEMIDROITE)
            {
              if(((this->extremity1.x < this->extremity2.x) == (this->extremity1.x < other.extremity1.x))&&((this->extremity1.y < this->extremity2.y) == (this->extremity1.y < other.extremity1.y)))
              {
                intersection = other.extremity1;
              }
              else if(((other.extremity1.x < other.extremity2.x) == (other.extremity1.x < this->extremity1.x))&&((other.extremity1.y < other.extremity2.y) == (other.extremity1.y < this->extremity1.y)))
              {
                intersection = this->extremity1;
              }
              else
              {
                return false;
              }
            }
            else  // other.type == SEGMENT
            {
              if(((this->extremity1.x < this->extremity2.x) == (this->extremity1.x < other.extremity1.x))&&((this->extremity1.y < this->extremity2.y) == (this->extremity1.y < other.extremity1.y)))
              {
                if(((this->extremity1.x < this->extremity2.x) == (this->extremity1.x < other.extremity2.x))&&((this->extremity1.y < this->extremity2.y) == (this->extremity1.y < other.extremity2.y)))
                {
                  float x_diff1 = this->extremity1.x-other.extremity1.x, x_diff2 = this->extremity1.x-other.extremity2.x;
                  float y_diff1 = this->extremity1.y-other.extremity1.y, y_diff2 = this->extremity1.x-other.extremity2.y;

                  if(x_diff1*x_diff1+y_diff1*y_diff1 < x_diff2*x_diff2+y_diff2*y_diff2)
                  {
                    intersection = other.extremity1;
                  }
                  else
                  {
                    intersection = other.extremity2;
                  }
                }
                else
                {
                  intersection = other.extremity1;
                }
              }
              else if(((this->extremity1.x < this->extremity2.x) == (this->extremity1.x < other.extremity2.x))&&((this->extremity1.y < this->extremity2.y) == (this->extremity1.y < other.extremity2.y)))
              {
                intersection = other.extremity2;
              }
              else
              {
                return false;
              }
            }
          }
          else  // this->type == SEGMENT
          {
            return false;
            // TODO
          }
          return true;
        }
      }
      else                      // this->b == other.b && this->a != other.a  ==> les deux droites sont de la forme ( y=ax+b avec les deux a différents )
      {
        intersection.x = (other.c - this->c)/(this->a - other.a);
        intersection.y = (-this->a*intersection.x - this->c)/this->b;
      }
    }
    else                        // this->b != other.b  ==>  une des deux droites est verticale ( x=? )
    {
      if(this->b == 0)
      {
        intersection.x = -this->c;
        intersection.y = (-other.a*intersection.x - other.c)/other.b;
      }
      else
      {
        intersection.x = -other.c;
        intersection.y = (-this->a*intersection.x - this->c)/this->b;
      }
    }


    float thisMinX, thisMaxX, thisMinY, thisMaxY, otherMinX, otherMaxX, otherMinY, otherMaxY;

    if(this->extremity1.x < this->extremity2.x)
    {
      thisMinX = this->extremity1.x - epsilon;
      thisMaxX = this->extremity2.x + epsilon;
    }
    else
    {
      thisMinX = this->extremity2.x - epsilon;
      thisMaxX = this->extremity1.x + epsilon;
    }

    if(this->extremity1.y < this->extremity2.y)
    {
      thisMinY = this->extremity1.y - epsilon;
      thisMaxY = this->extremity2.y + epsilon;
    }
    else
    {
      thisMinY = this->extremity2.y - epsilon;
      thisMaxY = this->extremity1.y + epsilon;
    }

    if(other.extremity1.x < other.extremity2.x)
    {
      otherMinX = other.extremity1.x - epsilon;
      otherMaxX = other.extremity2.x + epsilon;
    }
    else
    {
      otherMinX = other.extremity2.x - epsilon;
      otherMaxX = other.extremity1.x + epsilon;
    }

    if(other.extremity1.y < other.extremity2.y)
    {
      otherMinY = other.extremity1.y - epsilon;
      otherMaxY = other.extremity2.y + epsilon;
    }
    else
    {
      otherMinY = other.extremity2.y - epsilon;
      otherMaxY = other.extremity1.y + epsilon;
    }

    return (((this->type == SEGMENT)&&(thisMinX <= intersection.x)&&(intersection.x <= thisMaxX)&&(thisMinY <= intersection.y)&&(intersection.y <= thisMaxY)) ||
    ((this->type == DEMIDROITE)&&((this->extremity1.x < intersection.x) == (this->extremity1.x < this->extremity2.x))&&((this->extremity1.y < intersection.y) == (this->extremity1.y < this->extremity2.y))) ||
    (this->type == DROITE)) &&
    (((other.type == SEGMENT)&&(otherMinX <= intersection.x)&&(intersection.x <= otherMaxX)&&(otherMinY <= intersection.y)&&(intersection.y <= otherMaxY)) ||
    ((other.type == DEMIDROITE)&&((other.extremity1.x < intersection.x) == (other.extremity1.x < other.extremity2.x))&&((other.extremity1.y < intersection.y) == (other.extremity1.y < other.extremity2.y))) ||
    (other.type == DROITE));
  }

  bool Room::LinearEquation::pointBelongToEquation(const sf::Vector2f& point)const
  {
    if(a == 0)
    {
      if((-c < point.y-epsilon)||(point.y+epsilon < -c))
        return false;
      else
      {
        double thisMinX, thisMaxX;
        if(this->extremity1.x < this->extremity2.x)
        {
          thisMinX = this->extremity1.x - epsilon;
          thisMaxX = this->extremity2.x + epsilon;
        }
        else
        {
          thisMinX = this->extremity2.x - epsilon;
          thisMaxX = this->extremity1.x + epsilon;
        }
        return (((this->type == SEGMENT)&&(thisMinX <= point.x)&&(point.x <= thisMaxX)) ||
                ((this->type == DEMIDROITE)&&((this->extremity1.x < point.x) == (this->extremity1.x < this->extremity2.x))) ||
                 (this->type == DROITE));
      }
    }
    else if(b == 0)
    {
      if((-c < point.x-epsilon)||(point.x+epsilon < -c))
        return false;
      else
      {
        double thisMinY, thisMaxY;
        if(this->extremity1.y < this->extremity2.y)
        {
          thisMinY = this->extremity1.y - epsilon;
          thisMaxY = this->extremity2.y + epsilon;
        }
        else
        {
          thisMinY = this->extremity2.y - epsilon;
          thisMaxY = this->extremity1.y + epsilon;
        }
        return (((this->type == SEGMENT)&&(thisMinY <= point.y)&&(point.y <= thisMaxY)) ||
                ((this->type == DEMIDROITE)&&((this->extremity1.y < point.y) == (this->extremity1.y < this->extremity2.y))) ||
                 (this->type == DROITE));
      }
    }
    else
    {
      double realValue = (-b*point.y-c)/a;
      return (point.x-epsilon <= realValue)&&(realValue <= point.x+epsilon);
    }
    return false;
  }

  Room::Room()
  {
    tex = new sf::Texture();
    tex->loadFromFile("sol.png");
    tex->copyToImage().createMaskFromColor(sf::Color::Black, 50);
  }

  Room::~Room()
  {
      for(int i=0;i<hauteur;i++)
      {
          delete[] roomArchitecture[i];
      }
      delete[] roomArchitecture;
      delete tex;
  }

  void Room::generateRandomRoom()
  {
      hauteur = HAUTEUR_MIN+rand()%HAUTEUR_VARIANCE;
      largeur = 0;

      int architecture[hauteur];

      for(int i=0;i<hauteur;i++)
      {
          architecture[i]=LARGEUR_MIN+rand()%LARGEUR_VARIANCE;
          if(architecture[i]>largeur)
              largeur = architecture[i];
      }

      roomArchitecture = new directions *[hauteur];
      for(int i=0;i<hauteur;i++)
      {
          roomArchitecture[i] = new directions[largeur];
          for(int j=0;j<largeur;j++)
              roomArchitecture[i][j]=INACCESSIBLE;
      }

      int offset[hauteur];
      int corner[hauteur][2];

      for(int i=0;i<hauteur;i++)
      {
          offset[i]=rand()%(largeur-architecture[i]+1);

          corner[i][0]=offset[i];
          corner[i][1]=offset[i]+architecture[i];

          for(int j=0;j<architecture[i];j++)
          {
              roomArchitecture[i][offset[i]+j]=UNSET;
          }
      }

      vertices.push_back(sf::Vector2u(corner[0][1],0));
      vertices.push_back(sf::Vector2u(corner[0][1],1));
      for(int i=1;i<hauteur;i++)
      {
        if(offset[i]+architecture[i] == offset[i-1]+architecture[i-1])
        {
          vertices.pop_back();
          vertices.push_back(sf::Vector2u(corner[i][1],i+1));
        }
        else
        {
          vertices.push_back(sf::Vector2u(corner[i][1],i));
          vertices.push_back(sf::Vector2u(corner[i][1],i+1));
        }
      }

      vertices.push_back(sf::Vector2u(corner[hauteur-1][0],hauteur));
      vertices.push_back(sf::Vector2u(corner[hauteur-1][0],hauteur-1));
      for(int i=hauteur-2;i>=0;i--)
      {
        if(offset[i] == offset[i+1])
        {
          vertices.pop_back();
          vertices.push_back(sf::Vector2u(corner[i][0],i));
        }
        else
        {
          vertices.push_back(sf::Vector2u(corner[i][0],i+1));
          vertices.push_back(sf::Vector2u(corner[i][0],i));
        }
      }

      //loadSpecificRoom();
  }

  void Room::loadSpecificRoom()
  {
    hauteur = 4;
    largeur = 4;

    roomArchitecture = new directions *[hauteur];
    for(int i=0;i<hauteur;i++)
    {
        roomArchitecture[i] = new directions[largeur];
        for(int j=0;j<largeur;j++)
            roomArchitecture[i][j]=INACCESSIBLE;
    }

    roomArchitecture[0][0] = UNSET;
    roomArchitecture[0][1] = UNSET;
    roomArchitecture[0][2] = UNSET;
    roomArchitecture[0][3] = UNSET;

    roomArchitecture[1][1] = UNSET;
    roomArchitecture[1][2] = UNSET;
    roomArchitecture[1][3] = UNSET;

    roomArchitecture[2][2] = UNSET;
    roomArchitecture[2][3] = UNSET;

    roomArchitecture[3][0] = UNSET;
    roomArchitecture[3][1] = UNSET;

    vertices.clear();
    vertices.push_back(sf::Vector2u(4,0));
    vertices.push_back(sf::Vector2u(4,3));
    vertices.push_back(sf::Vector2u(2,3));
    vertices.push_back(sf::Vector2u(2,4));
    vertices.push_back(sf::Vector2u(0,4));
    vertices.push_back(sf::Vector2u(0,3));
    vertices.push_back(sf::Vector2u(2,3));
    vertices.push_back(sf::Vector2u(2,2));
    vertices.push_back(sf::Vector2u(1,2));
    vertices.push_back(sf::Vector2u(1,1));
    vertices.push_back(sf::Vector2u(0,1));
    vertices.push_back(sf::Vector2u(0,0));

    /*roomArchitecture[0][0] = UNSET;
    roomArchitecture[0][1] = UNSET;

    roomArchitecture[1][2] = UNSET;
    roomArchitecture[1][3] = UNSET;

    roomArchitecture[2][0] = UNSET;
    roomArchitecture[2][1] = UNSET;

    roomArchitecture[3][2] = UNSET;
    roomArchitecture[3][3] = UNSET;

    vertices.clear();
    vertices.push_back(sf::Vector2u(2,0));
    vertices.push_back(sf::Vector2u(2,1));
    vertices.push_back(sf::Vector2u(4,1));
    vertices.push_back(sf::Vector2u(4,2));
    vertices.push_back(sf::Vector2u(2,2));
    vertices.push_back(sf::Vector2u(2,3));
    vertices.push_back(sf::Vector2u(4,3));
    vertices.push_back(sf::Vector2u(4,4));

    vertices.push_back(sf::Vector2u(2,4));
    vertices.push_back(sf::Vector2u(2,3));
    vertices.push_back(sf::Vector2u(0,3));
    vertices.push_back(sf::Vector2u(0,2));
    vertices.push_back(sf::Vector2u(2,2));
    vertices.push_back(sf::Vector2u(2,1));
    vertices.push_back(sf::Vector2u(0,1));
    vertices.push_back(sf::Vector2u(0,0));*/

    /*roomArchitecture[0][0] = UNSET;
    roomArchitecture[0][1] = UNSET;
    roomArchitecture[0][2] = UNSET;
    roomArchitecture[0][3] = UNSET;

    roomArchitecture[1][1] = UNSET;
    roomArchitecture[1][2] = UNSET;

    roomArchitecture[2][0] = UNSET;
    roomArchitecture[2][1] = UNSET;
    roomArchitecture[2][2] = UNSET;
    roomArchitecture[2][3] = UNSET;

    vertices.clear();
    vertices.push_back(sf::Vector2u(4,0));
    vertices.push_back(sf::Vector2u(4,1));
    vertices.push_back(sf::Vector2u(3,1));
    vertices.push_back(sf::Vector2u(3,2));
    vertices.push_back(sf::Vector2u(4,2));
    vertices.push_back(sf::Vector2u(4,3));

    vertices.push_back(sf::Vector2u(0,3));
    vertices.push_back(sf::Vector2u(0,2));
    vertices.push_back(sf::Vector2u(1,2));
    vertices.push_back(sf::Vector2u(1,1));
    vertices.push_back(sf::Vector2u(0,1));
    vertices.push_back(sf::Vector2u(0,0));*/
  }

  void Room::resetPath()
  {
      for(int i=0;i<hauteur;i++)
      {
          for(int j=0;j<largeur;j++)
          {
              if(roomArchitecture[i][j] != INACCESSIBLE)
                  roomArchitecture[i][j] = UNSET;
          }
      }
  }

  void Room::pathTo(int x, int y)
  {
      if((x<0)||(x>=hauteur)||(y<0)||(y>=largeur)||(roomArchitecture[x][y] == INACCESSIBLE))
          return;

      std::queue<std::pair<int,int> > file;
      file.push(std::make_pair(x,y));

      roomArchitecture[x][y]=DEPART;

      while(!file.empty())
      {
          x = file.front().first;
          y = file.front().second;
          file.pop();

          if((x>0) && (roomArchitecture[x-1][y] == UNSET))
          {
              roomArchitecture[x-1][y] = BOT;
              file.push(std::make_pair(x-1,y));
          }
          if((x<hauteur-1) && (roomArchitecture[x+1][y] == UNSET))
          {
              roomArchitecture[x+1][y] = TOP;
              file.push(std::make_pair(x+1,y));
          }
          if((y>0) && (roomArchitecture[x][y-1] == UNSET))
          {
              roomArchitecture[x][y-1] = RIGHT;
              file.push(std::make_pair(x,y-1));
          }
          if((y<largeur-1) && (roomArchitecture[x][y+1] == UNSET))
          {
              roomArchitecture[x][y+1] = LEFT;
              file.push(std::make_pair(x,y+1));
          }
      }
  }

  void Room::drawShape(sf::RenderWindow& window)const
  {
    int size_x=window.getSize().x/(LARGEUR_MIN+LARGEUR_VARIANCE-1), size_y=window.getSize().y/(HAUTEUR_MIN+HAUTEUR_VARIANCE-1);
    sf::RectangleShape tmp(sf::Vector2f(size_x,size_y));

    for(int i=0;i<hauteur;i++)
    {
          for(int j=0;j<largeur;j++)
          {
              if(roomArchitecture[i][j] != INACCESSIBLE)
              {
                  switch(roomArchitecture[i][j])
                  {
                  case UNSET:
                      tmp.setFillColor(sf::Color::White);
                      break;
                  case INACCESSIBLE:
                      tmp.setFillColor(sf::Color::Black);
                      break;
                  case DEPART:
                      tmp.setFillColor(sf::Color::Green);
                      break;
                  case LEFT:
                      tmp.setFillColor(sf::Color::Blue);
                      break;
                  case RIGHT:
                      tmp.setFillColor(sf::Color::Magenta);
                      break;
                  case TOP:
                      tmp.setFillColor(sf::Color::Yellow);
                      break;
                  case BOT:
                      tmp.setFillColor(sf::Color::Cyan);
                      break;
                  default:
                      break;
                  }
                  tmp.setPosition(j*size_x,i*size_y);
                  tmp.setTexture(tex);
                  window.draw(tmp);
              }
          }
      }
  }

  void Room::draw(sf::RenderWindow& window)const
  {
      drawShape(window);

      int size_x=window.getSize().x/(LARGEUR_MIN+LARGEUR_VARIANCE-1), size_y=window.getSize().y/(HAUTEUR_MIN+HAUTEUR_VARIANCE-1);

      static const int delta=1;
      static int mousx=0;
      static int mousy=0;
      static bool left, right, up, down;

      sf::Vector2f mouse_pos = window.mapPixelToCoords(sf::Mouse::getPosition(window), window.getView());
      mouse_pos = sf::Vector2f(mousx, mousy);
      //printf("%f %f\n",mouse_pos.x,mouse_pos.y);

      if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Left)){left=true;}
      if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Right)){right=true;}
      if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Up)){up=true;}
      if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Down)){down=true;}

      if (left&&sf::Keyboard::isKeyPressed(sf::Keyboard::Left)){mousx-=delta;left=false;}
      if (right&&sf::Keyboard::isKeyPressed(sf::Keyboard::Right)){mousx+=delta;right=false;}
      if (up&&sf::Keyboard::isKeyPressed(sf::Keyboard::Up)){mousy-=delta;up=false;}
      if (down&&sf::Keyboard::isKeyPressed(sf::Keyboard::Down)){mousy+=delta;down=false;}

      if (left&&sf::Keyboard::isKeyPressed(sf::Keyboard::Q))mousx-=delta;
      if (right&&sf::Keyboard::isKeyPressed(sf::Keyboard::D))mousx+=delta;
      if (up&&sf::Keyboard::isKeyPressed(sf::Keyboard::Z))mousy-=delta;
      if (down&&sf::Keyboard::isKeyPressed(sf::Keyboard::S))mousy+=delta;

      sf::Vector2u* realSizeVertex = createRealSizeVertex(size_x, size_y);
      struct LinearEquation* wall = createWall(realSizeVertex);

      if(isPointInRoom(mouse_pos, wall, size_x, size_y))
      {
        std::vector<IntersectionPoint> ver = triangleCorner(mouse_pos, realSizeVertex, wall);

        /*sf::CircleShape ss(10);
        ss.setOrigin(10,10);
        ss.setPosition(mouse_pos);
        ss.setFillColor(sf::Color::Blue);
        window.draw(ss);*/

        int fanSize = ver.size()+2;

        sf::CircleShape ss(10);
        ss.setOrigin(10,10);
        sf::Vertex polygon[fanSize];
        polygon[0] = sf::Vertex(mouse_pos, sf::Color(255,0,0,80));
        for(unsigned int i=0;i<ver.size();i++)
        {
          polygon[i+1] = sf::Vertex(ver[i].position, sf::Color(255,0,0,80));
          ss.setPosition(ver[i].position);
          ss.setFillColor(sf::Color::Yellow);
          if(i==0) ss.setFillColor(sf::Color::Blue);
          window.draw(ss);
        }
        polygon[fanSize-1] = polygon[1];

        window.draw(polygon, fanSize, sf::TrianglesFan);


        sf::Vertex linesd[ver.size()*2];
        for(unsigned int i=0;i<ver.size();i++)
        {
          linesd[i*2] = sf::Vertex(mouse_pos, sf::Color::Blue);
          linesd[i*2+1] = sf::Vertex(ver[i].position, sf::Color::Blue);
        }
        window.draw(linesd, ver.size()*2, sf::Lines);

        unsigned int beginRoomPoints, beginLightPoints = 1, numAllVertex = vertices.size()+ver.size();
        sf::Vector2f allVertex[numAllVertex];
        bool vertexEnlightened[numAllVertex];

        /*int index = 0;
        for(int i=0;i<vertices.size();i++)
        {
          allVertex[index] = vertices[i];
          vertexEnlightened[index] = false;
          for(int j=0;ver[j].wall == i;j++)
          {

          }
        }*/

        //printf("%d %d %d, %f %f %d\n",vertices.size(),realSizeVertex[(ver[0].wall+1)%vertices.size()].x,realSizeVertex[(ver[0].wall+1)%vertices.size()].y,ver[0].position.x,ver[0].position.y,ver[0].wall);

        int lastWall = ver[0].wall, index = 0;
        sf::Vector2f diff;
        allVertex[0] = ver[index++].position;
        vertexEnlightened[0] = true;
        for(int i=1;i<numAllVertex;i++)
        {
          while(lastWall == ver[index].wall)
          {
            diff = allVertex[i-1]-ver[index].position;
            if((fabs(diff.x)==0)&&(fabs(diff.y)==0))
              vertexEnlightened[i-1] = true;

            allVertex[i] = ver[index].position;
            vertexEnlightened[i] = true;
            i++;
            index++;
          }
          lastWall = (lastWall+1)%vertices.size();
          diff = allVertex[i-1]-sf::Vector2f(realSizeVertex[lastWall]);
          vertexEnlightened[i] = false;
          if((fabs(diff.x)==0)&&(fabs(diff.y)==0))
            vertexEnlightened[i] = vertexEnlightened[i-1];
          allVertex[i] = sf::Vector2f(realSizeVertex[lastWall]);
        }

        bool affichage=sf::Keyboard::isKeyPressed(sf::Keyboard::Return);
        for(int i=0;i<numAllVertex;i++)
        {
          if(affichage)printf("%d %f %f %d\n",i,allVertex[i].x,allVertex[i].y, vertexEnlightened[i]);
        }

        /*for(unsigned int i=vertices.size()-1;i>=0;i--)
        {
          if(wall[i].pointBelongToEquation(ver[0].position))
          {
            beginRoomPoints = i;
            break;
          }
        }

        allVertex[0] = ver[0].position;
        vertexEnlightened[0] = true;

        for(unsigned int i=1;i<numAllVertex;i++)
        {
          if(wall[beginRoomPoints].pointBelongToEquation(ver[beginLightPoints].position))
          {
            allVertex[i] = ver[beginLightPoints].position;
            vertexEnlightened[i] = true;
            beginLightPoints = (beginLightPoints+1)%ver.size();
          }
          else
          {
            allVertex[i] = wall[beginRoomPoints].extremity2;
            sf::Vector2f diff = allVertex[i]-allVertex[i-1];
            if(fabs(diff.x)<epsilon && fabs(diff.y)<epsilon)
              i--;
            else
              vertexEnlightened[i] = false;
            beginRoomPoints = (beginRoomPoints+1)%vertices.size();
          }
          if(i>ver.size())
          {
            sf::Vector2f diff = allVertex[0]-allVertex[i];
            if(fabs(diff.x)<epsilon && fabs(diff.y)<epsilon)
            {
              numAllVertex=i;
              break;
            }
          }
        }*/

        // TODO incorrect : tester sur des grandes valeurs

        //bool affichage = false;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))affichage=true;
        affichage=false;
        for(unsigned int i=0;i<numAllVertex;i++)
          if(affichage)printf("%d %d %f %f\n",i,vertexEnlightened[i],allVertex[i].x,allVertex[i].y);
        if(affichage)printf("\n\n");

        std::vector<std::vector<sf::Vector2f> > shapeList = polygonList(allVertex, vertexEnlightened, numAllVertex);

        const sf::Color ombre(80,80,80,100);
        for(unsigned int i=0;i<shapeList.size();i++)
        {
          if(shapeList[i].size() == 3)
          {
            sf::Vertex point[shapeList[i].size()];
            point[0] = sf::Vertex(shapeList[i][0], ombre);
            point[1] = sf::Vertex(shapeList[i][1], ombre);
            point[2] = sf::Vertex(shapeList[i][2], ombre);
            window.draw(point, 3, sf::Triangles);
          }
          else
          {
            int nbTriVertice = (shapeList[i].size()-2)*3;
            sf::Vertex point[nbTriVertice];
            for(int j=0;j<nbTriVertice;j++)
              point[j].color = j/3 == 0 ? sf::Color(100,0,0,200) : j/3 == 1 ? sf::Color(0,100,0,200) : j/3 == 2 ? sf::Color(0,0,100,200) : j/3 == 3 ? sf::Color(100,0,100,200) : j/3 == 4 ? sf::Color(0,100,100,200) : sf::Color(100,100,0,200);
            nbTriVertice = triangulateShape(shapeList[i], point);
            window.draw(point, nbTriVertice, sf::Triangles);
          }
        }
      }

      delete[] realSizeVertex;
      delete[] wall;
  }

  bool angle(sf::Vector3f i,sf::Vector3f j)
  {
    return i.z<j.z;
  }

  sf::Vector2u* Room::createRealSizeVertex(int size_x, int size_y)const
  {
    sf::Vector2u* res = new sf::Vector2u[vertices.size()];

    for(unsigned int i=0;i<vertices.size();i++)
      res[i] = sf::Vector2u(vertices[i].x*size_x, vertices[i].y*size_y);

    return res;
  }

  struct Room::LinearEquation* Room::createWall(const sf::Vector2u* realSizeVertex)const
  {
    struct LinearEquation* res = new struct LinearEquation[vertices.size()];

    unsigned int lastIndex = vertices.size()-1;
    for(unsigned int i=0;i<lastIndex;i++)
    {
      res[i] = LinearEquation(sf::Vector2f(realSizeVertex[i].x,realSizeVertex[i].y), sf::Vector2f(realSizeVertex[i+1].x,realSizeVertex[i+1].y), LinearEquation::SEGMENT);
    }
    res[lastIndex] = LinearEquation(sf::Vector2f(realSizeVertex[lastIndex].x,realSizeVertex[lastIndex].y), sf::Vector2f(realSizeVertex[0].x,realSizeVertex[0].y), LinearEquation::SEGMENT);

    return res;
  }

  std::vector<std::vector<sf::Vector2f> > Room::polygonList(const sf::Vector2f* vertex, const bool* vertexLight, const int nbVertex)
  {
    std::vector<std::vector<sf::Vector2f> > res;

    for(int i=1;i<nbVertex;i++)
    {
      if(!vertexLight[i])
      {
        res.push_back(std::vector<sf::Vector2f>());
        res.back().push_back(vertex[i-1]);
        while(!vertexLight[i] && i<nbVertex)
        {
          res.back().push_back(vertex[i]);
          i++;
        }
        res.back().push_back(vertex[i%nbVertex]);
      }
    }

    return res;
  }

  std::vector<Room::IntersectionPoint> Room::triangleCorner(const sf::Vector2f mouse_pos, const sf::Vector2u* realSizeVertex, const struct LinearEquation* wall)const
  {
    sf::Vector3f verticesOrdered[vertices.size()];

    double mouseAngle = atan2(mouse_pos.y,-mouse_pos.x);
    for(unsigned int i=0;i<vertices.size();i++)
    {
      verticesOrdered[i] = sf::Vector3f(realSizeVertex[i].x,realSizeVertex[i].y, 0);
      verticesOrdered[i].z= mouseAngle - atan2(mouse_pos.y-verticesOrdered[i].y,verticesOrdered[i].x-mouse_pos.x);
    }

    std::sort(&verticesOrdered[0], &verticesOrdered[vertices.size()], angle);

    std::vector<struct IntersectionPoint> intersectionList[vertices.size()];

    // pour tous les angles de la salle
    for(unsigned int i=0;i<vertices.size();i++)
    {
      struct LinearEquation mouseVertice(mouse_pos, sf::Vector2f(verticesOrdered[i].x,verticesOrdered[i].y), LinearEquation::DEMIDROITE);

      for(unsigned int j=0;j<vertices.size();j++)
      {
        sf::Vector2f intersect;
        if((mouseVertice != wall[j]) && (mouseVertice.findIntersection(wall[j], intersect)))
        {
          int x_diff = mouse_pos.x-intersect.x;
          int y_diff = mouse_pos.y-intersect.y;
          intersectionList[i].push_back(IntersectionPoint(intersect, x_diff*x_diff+y_diff*y_diff, j));
          //if (bbb)printf("%d %f %f %d %d\n",i,intersect.x,intersect.y,x_diff*x_diff+y_diff*y_diff,j);
        }
      }
      std::sort(intersectionList[i].begin(), intersectionList[i].end());
    }
    return collisionGestion(vertices.size(), intersectionList);
  }

  std::vector<Room::IntersectionPoint> Room::collisionGestion(const unsigned int length, const std::vector<struct IntersectionPoint>* intersectionList)
  {
    std::vector<IntersectionPoint> res;
    res.reserve(length);

    bool validWall[length];
    for(unsigned int i=0;i<length;i++)
      validWall[i]=false;

    int firstCollision = length-1;
    while(intersectionList[firstCollision].empty())
    {
      firstCollision = (firstCollision+1)%length;
      if(firstCollision == (int)length-1)
        return res;
    }
    validWall[intersectionList[firstCollision].front().wall]=true;
    if(pow(intersectionList[firstCollision].front().position.x-intersectionList[firstCollision][1].position.x, 2)+
       pow(intersectionList[firstCollision].front().position.y-intersectionList[firstCollision][1].position.y, 2)<10)
      validWall[intersectionList[firstCollision][1].wall]=true;

    for(unsigned int i=0;i<length;i++)
    {
      if(intersectionList[i].empty())
      {
        continue;
      }

      int pointBefore = (i==0) ? (length-1) : (i-1);

      int otherWall=intersectionList[i].front().wall;
      bool nextWallisCorner=false;
      if(pow(intersectionList[i].front().position.x-intersectionList[i][1].position.x, 2)+
         pow(intersectionList[i].front().position.y-intersectionList[i][1].position.y, 2)<10)
      {
        otherWall=intersectionList[i][1].wall;
        nextWallisCorner=true;
      }

      if((!validWall[intersectionList[i].front().wall])&&(!validWall[otherWall]))
      {
        bool found=false;
        for(unsigned int j=0;j<intersectionList[i].size();j++)
        {
          if(validWall[intersectionList[i][j].wall])
          {
            res.push_back(intersectionList[i][j]);
            found=true;
            break;
          }
        }

        if(!found)
        {
          for(unsigned int j=0;j<length;j++)
            validWall[j]=false;
          validWall[intersectionList[i].front().wall]=true;
          if(nextWallisCorner)
            validWall[intersectionList[i][1].wall]=true;

          for(unsigned int j=0;j<intersectionList[pointBefore].size();j++)
          {
            if(validWall[intersectionList[pointBefore][j].wall])
            {
              res.push_back(intersectionList[pointBefore][j]);
              found=true;
              break;
            }
          }
        }
        if(!found)
        {
          for(unsigned int j=0;j<intersectionList[i].size();j++)
          {
            for(unsigned int k=0;k<intersectionList[pointBefore].size();k++)
            {
              if(intersectionList[i][j].wall==intersectionList[pointBefore][k].wall)
              {
                res.push_back(intersectionList[pointBefore][k]);
                res.push_back(intersectionList[i][j]);
                found=true;
                break;
              }
            }
            if(found)
              break;
          }
        }
      }
      res.push_back(intersectionList[i].front());
      for(unsigned int j=0;j<length;j++)
        validWall[j]=false;
      validWall[intersectionList[i].front().wall]=true;
      if(nextWallisCorner)
        validWall[intersectionList[i][1].wall]=true;
    }
    return res;
  }

  bool Room::isPointInRoom(const sf::Vector2f& point, struct LinearEquation* wall, int size_x, int size_y)const
  {
    if(((int)point.x%size_x == 0)||((int)point.y%size_y == 0))
    {
      for(unsigned int i=0;i<vertices.size();i++)
      {
        if(wall[i].pointBelongToEquation(point))
          return false;
      }
    }

    sf::Vector2i intPoint = sf::Vector2i(point);

    sf::IntRect win(sf::Vector2i(1,1),sf::Vector2i(largeur*size_x-1,hauteur*size_y-1));

    return win.contains(intPoint)&&(roomArchitecture[(intPoint.y)/size_y][(intPoint.x)/size_x] != INACCESSIBLE);
  }

  int Room::triangulateShape(const std::vector<sf::Vector2f>& angles, sf::Vertex* verticeTrianguled)
  {
    const bool REFLEX = false, CONVEX = true;
    bool angleType[angles.size()];
    Utility::ChainList ears, remainingVertices;
    for(int i=angles.size()-1;i>=0;i--)
      remainingVertices.push_front(i);

    const int lastindex = angles.size()-1;
    angleType[0] = (angles[1].x-angles[0].x)*(angles[lastindex].y-angles[0].y) > (angles[lastindex].x-angles[0].x)*(angles[1].y-angles[0].y);
    for(int i=1;i<lastindex;i++)
      angleType[i] = (angles[i+1].x-angles[i].x)*(angles[i-1].y-angles[i].y) > (angles[i-1].x-angles[i].x)*(angles[i+1].y-angles[i].y);
    angleType[lastindex] = (angles[0].x-angles[lastindex].x)*(angles[angles.size()-2].y-angles[lastindex].y) > (angles[angles.size()-2].x-angles[lastindex].x)*(angles[0].y-angles[lastindex].y);

    for(int i=angles.size()-1;i>=0;i--)
    {
      if(angleType[i] == CONVEX)
      {
        const int first = (i+2)%angles.size();
        const int iprev = (i+angles.size()-1)%angles.size();
        const int inext = (i+1)%angles.size();
        ears.push_front(i);
        for(int j=first;j!=iprev;j=(j+1)%angles.size())
        {
          if(isPointInTriangle(angles[j], angles[iprev], angles[i], angles[inext]))
          {
            ears.pop_front();
            break;
          }
        }
      }
    }

    int triActual = 0;
    while((remainingVertices.size() > 3)&&(!ears.empty()))
    {
      const int deletetedVertice = remainingVertices.remove_element(ears.front());
      verticeTrianguled[triActual++].position = angles[ears.front()];
      ears.pop_front();

      int actualVertices[remainingVertices.size()];
      remainingVertices.to_array(actualVertices);


      const int prevprevVertice = (deletetedVertice-2+remainingVertices.size())%remainingVertices.size();
      const int prevVertice = deletetedVertice-1 < 0 ? remainingVertices.size()-1 : deletetedVertice-1;
      const int nextVertice = deletetedVertice == (int)remainingVertices.size() ? 0 : deletetedVertice;
      const int nextnextVertice = (deletetedVertice+1)%remainingVertices.size();

      verticeTrianguled[triActual++].position = angles[actualVertices[prevVertice]];
      verticeTrianguled[triActual++].position = angles[actualVertices[nextVertice]];

      if(angleType[actualVertices[prevVertice]] == REFLEX)
      {
        angleType[actualVertices[prevVertice]] = (angles[actualVertices[nextVertice]].x-angles[actualVertices[prevVertice]].x)*(angles[actualVertices[prevprevVertice]].y-angles[actualVertices[prevVertice]].y) > (angles[actualVertices[prevprevVertice]].x-angles[actualVertices[prevVertice]].x)*(angles[actualVertices[nextVertice]].y-angles[actualVertices[prevVertice]].y);
      }
      if(angleType[actualVertices[prevVertice]] == CONVEX)
      {
        ears.remove_element(actualVertices[prevVertice]);
        ears.push_front(actualVertices[prevVertice]);

        for(int j=nextnextVertice;j!=prevprevVertice;j=(j+1)%remainingVertices.size())
        {
          if(isPointInTriangle(angles[actualVertices[j]], angles[actualVertices[prevprevVertice]], angles[actualVertices[prevVertice]], angles[actualVertices[nextVertice]]))
          {
            ears.pop_front();
            break;
          }
        }
      }


      if(angleType[actualVertices[nextVertice]] == REFLEX)
      {
        angleType[actualVertices[nextVertice]] = (angles[actualVertices[nextnextVertice]].x-angles[actualVertices[nextVertice]].x)*(angles[actualVertices[prevVertice]].y-angles[actualVertices[nextVertice]].y) > (angles[actualVertices[prevVertice]].x-angles[actualVertices[nextVertice]].x)*(angles[actualVertices[nextnextVertice]].y-angles[actualVertices[nextVertice]].y);
      }
      if(angleType[actualVertices[nextVertice]] == CONVEX)
      {
        ears.remove_element(actualVertices[nextVertice]);
        ears.push_front(actualVertices[nextVertice]);

        for(int j=(nextnextVertice+1)%remainingVertices.size();j!=prevVertice;j=(j+1)%remainingVertices.size())
        {
          if(isPointInTriangle(angles[actualVertices[j]], angles[actualVertices[prevVertice]], angles[actualVertices[nextVertice]], angles[actualVertices[nextnextVertice]]))
          {
            ears.pop_front();
            break;
          }
        }
      }
    }

    if(remainingVertices.size() == 3)
    {
      int actualVertices[remainingVertices.size()];
      remainingVertices.to_array(actualVertices);
      verticeTrianguled[triActual++].position = angles[actualVertices[0]];
      verticeTrianguled[triActual++].position = angles[actualVertices[1]];
      verticeTrianguled[triActual++].position = angles[actualVertices[2]];
    }
    else  // Le polygone n'a plus d'oreille, faire une fonction qui divise les vertices restant en plusieurs polygones
    {

    }
    return triActual;
  }

  bool Room::isPointInTriangle(const sf::Vector2f point, const sf::Vector2f t1, const sf::Vector2f t2, const sf::Vector2f t3)
  {
    double x1, y1, x2, y2, x3, y3;
    x1 = t1.x - point.x;
    y1 = t1.y - point.y;
    x2 = t2.x - point.x;
    y2 = t2.y - point.y;
    x3 = t3.x - point.x;
    y3 = t3.y - point.y;

    return (x1*y2 >= x2*y1)&&(x2*y3 >= x3*y2)&&(x3*y1 >= x1*y3);
  }

