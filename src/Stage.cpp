#include "Stage.h"

Stage::Stage()
{

}

Stage::Stage(int rps)
{
    roomPerSide = rps;

    int totalRoom = rps*rps;
    rooms = new Room[totalRoom];

    for(int i=0;i<totalRoom;i++)
        rooms[i].generateRandomRoom();
}

Stage::~Stage()
{
    delete[] rooms;
}

void Stage::drawMiniMap(sf::RenderWindow& window)const
{
  sf::View v = window.getView();
  double scaledSize = 1.0/roomPerSide;
  for(int i=0;i<roomPerSide;i++)
  {
    for(int j=0;j<roomPerSide;j++)
    {
      v.setViewport(sf::FloatRect(j*UI_RATIO*scaledSize, 1-UI_RATIO+UI_RATIO*i*scaledSize, UI_RATIO*scaledSize, UI_RATIO*scaledSize));
      window.setView(v);
      rooms[i*roomPerSide+j].drawShape(window);
    }
  }

  v.setViewport(sf::FloatRect(0, 1-UI_RATIO, UI_RATIO, UI_RATIO));
  window.setView(v);

  static float grf=0;
  grf+=0.006;
  sf::Vertex radar[] =
  {
    sf::Vertex(sf::Vector2f(window.getSize().x/2.0, window.getSize().y/2.0), sf::Color::Red),
    sf::Vertex(sf::Vector2f(window.getSize().x/2+window.getSize().x*(cos(grf))/2.0, window.getSize().y/2+window.getSize().y*(sin(grf))/2.0), sf::Color::Red)
  };

  window.draw(radar, 2, sf::Lines);
}

void Stage::draw(sf::RenderWindow& window)const
{
  sf::View v = window.getView();
  v.setViewport(sf::FloatRect(UI_RATIO, 0, 1-UI_RATIO, 1-UI_RATIO));
  window.setView(v);
  rooms[0].draw(window);

  drawMiniMap(window);
}
