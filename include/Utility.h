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
      void deleteNodeChain();
    };

    struct ChainList
    {
      private:
      unsigned int length;
      struct Node* first;

      public:
      ChainList();
      virtual ~ChainList();
      void push_front(const int value);
      void pop_back();
      void pop_front();
      int front()const;
      unsigned int size()const;
      bool empty()const;
      int remove_element(const int val);
      void remove_element_at(const unsigned int index);
      void to_array(int* a)const;
      int operator [](const int index)const;
    };

    Utility();
    virtual ~Utility();
  protected:
  private:
};

#endif // UTILITY_H
