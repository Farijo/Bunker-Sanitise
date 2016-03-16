#include "Utility.h"
#include "stdio.h"

Utility::ChainList::ChainList()
{
  size = 0;
  first = nullptr;
}

Utility::ChainList::~ChainList()
{
  while(size > 0)
    remove_element_at(0);
}

void Utility::ChainList::puch_back(int value)
{
  Node** tmp = &first;
  while(*tmp)
    tmp = &((*tmp)->next);

  *tmp = new Node(value);
  size++;
}

void Utility::ChainList::remove_element_at(unsigned int index)
{
  if(index >= size)
    return;

  Node** tmp = &first;
  for(unsigned int i=0;i<index;i++)
  {
    if(!*tmp)
      return;
    tmp = &((*tmp)->next);
  }

  Node* aux = *tmp;
  *tmp = (*tmp)->next;
  delete aux;

  size--;
}

int Utility::ChainList::operator [](const int index)const
{
  Node* tmp = first;

  if((!tmp)||(size == 0))
    return -1;

  int delta, cyclicalIndex=index;
  if(index<0)
    delta = size;
  else if(index >= (int)size)
    delta = -size;

  while((cyclicalIndex<0)||((int)size<=cyclicalIndex))
    cyclicalIndex += delta;

  for(int i=0;i<cyclicalIndex;i++)
  {
    if(!tmp)
      return -1;
    tmp = tmp->next;
  }
  return tmp->value;
}

Utility::Node::Node()
{
  next = nullptr;
}

Utility::Node::Node(int v)
{
  value = v;
  next = nullptr;
}

Utility::Utility()
{
  //ctor
}

Utility::~Utility()
{
  //dtor
}
