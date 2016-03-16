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
      Node(const int v);
    };

    struct ChainList
    {
      unsigned int size;
      struct Node* first;

      ChainList();
      virtual ~ChainList();
      void push_front(const int value);
      void pop_back();
      void pop_front();
      void remove_element(const int val);
      void remove_element_at(const unsigned int index);
      int to_array(int* a, const int val)const;
      int operator [](const int index)const;
    };

    Utility();
    virtual ~Utility();
  protected:
  private:
};

#endif // UTILITY_H
