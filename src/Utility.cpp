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
    pop_front();
}

void Utility::ChainList::push_back(int value)
{
  Node** tmp = &first;
  while(*tmp)
    tmp = &((*tmp)->next);

  *tmp = new Node(value);
  size++;
}

void Utility::ChainList::pop_back()
{
  remove_element_at(size-1);
}

void Utility::ChainList::pop_front()
{
  remove_element_at(0);
}

void Utility::ChainList::remove_element(int val)
{
  Node** tmp = &first;
  while((*tmp)->value != val)
  {
    tmp = &((*tmp)->next);
    if(!*tmp)
      return;
  }

  Node* aux = *tmp;
  *tmp = (*tmp)->next;
  delete aux;

  size--;
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

int Utility::ChainList::to_array(int* a, int val)
{
  int res;
  for(int i=0;i<size;i++)
  {
    a[i] = (*this)[i];
    if(a[i] == val)
      res = i;
  }
  return res;
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
