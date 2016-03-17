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

void Utility::ChainList::push_front(const int value)
{
  Node* tmp = first;
  first = new Node(value);
  first->next = tmp;

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

int Utility::ChainList::remove_element(const int val)
{
  if(!first)
    return -1;

  int valIndex = 0;

  Node** tmp = &first;
  while((*tmp)->value != val)
  {
    tmp = &((*tmp)->next);
    valIndex++;
    if(!*tmp)
      return -1;
  }

  Node* aux = *tmp;
  *tmp = (*tmp)->next;
  delete aux;

  size--;

  return valIndex;
}

void Utility::ChainList::remove_element_at(const unsigned int index)
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

void Utility::ChainList::to_array(int* a)const
{
  for(unsigned int i=0;i<size;i++)
    a[i] = (*this)[i];
}

Utility::Node::Node()
{
  next = nullptr;
}

Utility::Node::Node(const int v)
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
