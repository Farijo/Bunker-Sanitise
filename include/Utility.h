#ifndef UTILITY_H
#define UTILITY_H


class Utility
{
  public:

    struct Node
    {
      int value;
      struct Node* next;

      Node();
      Node(int v);
    };

    struct ChainList
    {
      unsigned int size;
      struct Node* first;

      ChainList();
      virtual ~ChainList();
      void puch_back(int value);
      void remove_element_at(unsigned int index);
      int operator [](const int index)const;
    };

    Utility();
    virtual ~Utility();
  protected:
  private:
};

#endif // UTILITY_H
