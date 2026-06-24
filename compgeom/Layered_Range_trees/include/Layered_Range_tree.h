// Layered Range Trees 
//
// Copyright (c) 2011 Vissarion Fisikopoulos
//
// Licensed under GNU LGPL.3, see LICENCE file


#include <vector>
#include <iterator>
#include <algorithm>
#include <cassert>

// the tree classes
template <class C_Data, class T> class Layered_range_tree;
template <class C_Data, class T> class Layered_range_tree_node;
template <class C_Data> class Last_range_tree;
template <class C_Data> class Associated_structure;

// class with the fractional cascading structure nodes
template <class C_Data>
class Associated_structure_node {
 public:  
  friend class Associated_structure <C_Data>;
  friend class Last_range_tree <C_Data>;
  friend class Layered_range_tree_node<C_Data, Associated_structure<C_Data> >;

 public:
   //constructors
   Associated_structure_node()
   : data(0), substructure(NULL), leftstructure(NULL), rightstructure(NULL), index(0){ }
 
   Associated_structure_node(double input)
   : data(input), substructure(NULL), leftstructure(NULL), rightstructure(NULL), index(0) { }

  
   bool operator < (const Associated_structure_node<C_Data>& hs){
     return this->data.get_coordinate(1) < hs.data.get_coordinate(1);
   }

 private:
   C_Data data;
   Associated_structure<C_Data> *substructure;
   Associated_structure_node<C_Data> *leftstructure; 
   Associated_structure_node<C_Data> *rightstructure;
   int index;
};

// class with the fractional cascading structure 
template <class C_Data>
class Associated_structure { 
  
  typedef Associated_structure_node<C_Data> Assoc_node;
  typedef std::vector<Assoc_node> Associated_vector;  
  typedef typename Associated_vector::iterator Associated_vector_iterator;
  typedef std::vector<C_Data> Input_data;
  typedef typename std::vector<C_Data>::iterator Input_data_iterator;


  public:
    //constructors
    Associated_structure(int n) : size(n), assoc_array(n) {
      for (int i = 0; i < n; i++)
        assoc_array[i].index = i;
    };
    
    void delete_structure() {
      assoc_array.clear();
    }    
    
    void print() const{
      typename Associated_vector::const_iterator p;
      p = assoc_array.begin();
      std::cout << "[";
      //std::cout << p->data;
      while (p != assoc_array.end()){
        std::cout << " " << p->data.get_coordinate(1);
        std::cout << "[";
        if (p->leftstructure != NULL)
          std::cout << p->leftstructure->data.get_coordinate(1) << " (" << p->leftstructure->index << ") "; 
        else std::cout << "NULL ";
        if (p->rightstructure != NULL)
          std::cout << p->rightstructure->data.get_coordinate(1) << " (" << p->rightstructure->index << ") ";
        else std::cout << "NULL";
        std::cout << "]";
        p++;
      }
      std::cout << "]";
    }
    
    //destructor
    ~Associated_structure() {
      assoc_array.clear();
      std::cout << "~Associated_structure():an associated structure deleted!" 
                << std::endl;
    }
     
    void build_struct(Input_data_iterator begin, Input_data_iterator end, int d) {
      Input_data_iterator p = begin;
      int i = 0;
      while (p != end){
        assoc_array[i++].data = *p++;
      }
    }

    void put_frac_casc(int node, Assoc_node* frac_node, bool left) {
      if (left){
        assoc_array[node].leftstructure = frac_node;
        //assoc_array[node].leftstructure_index = index;
      }
      else {
        assoc_array[node].rightstructure = frac_node;
        //assoc_array[node].rightstructure_index = index;
      }  
    }
    
    Assoc_node* get_frac_casc(int node) {
      return &assoc_array[node];
    } 

    //TODO: clear s 
    Associated_vector_iterator assoc_binary_search(double i) {
      Assoc_node *s = new Assoc_node(i);
      return std::lower_bound(assoc_array.begin(), assoc_array.end(), *s);
    }

    Associated_vector_iterator get_end(){
      return assoc_array.end();
    }

    void report_structure(Assoc_node& assoc_v, 
                          Input_data& result2,
                          double y_2) {
      Associated_vector_iterator p = assoc_array.begin() + assoc_v.index;
      while (p != assoc_array.end() && (*p).data.get_coordinate(1) <= y_2) {
        result2.push_back((*p++).data);
      }  
    } 
    
    void push_data(Input_data_iterator input_data, int i, int d){
      assoc_array[i].data = *input_data;
    }
    
  private:
    int size;
    Associated_vector assoc_array;
};

//
// class with definitions of range trees nodes
//
template <class C_Data, class T>
class Layered_range_tree_node {
  
  //template <C_Data,T> friend class Layered_range_tree;
  friend class Layered_range_tree <C_Data, T>;
  friend class Last_range_tree <C_Data>;

  public:
    // constructor   
    Layered_range_tree_node()
    : data(0), substructure(NULL) { }
    
    // not used
    Associated_structure<C_Data>* get_substructure(){
      return static_cast<Associated_structure<C_Data>*>(substructure);
    }

  private:
    double data;
    T* substructure;
    typename std::vector<C_Data>* dData;
};

//
// the class with the definitions of the last dimension tree
//
template <class C_Data>
class Last_range_tree {

  typedef Layered_range_tree_node<C_Data, Associated_structure<C_Data> > Tree_node;
  typedef typename std::vector<Tree_node> Tree_vector;
  typedef typename std::vector<C_Data>::iterator Input_data;
   
  public:
    //constructors
    Last_range_tree(int n) : size(2*n-1), main_tree(2*n-1) {
    }
    
    //destructor
    ~Last_range_tree() {
      for (int i; i < size; i++) {
        main_tree[i].substructure->delete_structure();
      }
      main_tree.clear();
    }
    
    void delete_tree() {
      for (int i; i < size; i++) {
        main_tree[i].substructure->delete_structure();
      }
      main_tree.clear();
    }

    // print tree's vector
    void print_tree_vector() const {
      typename Tree_vector::const_iterator p;
      p = main_tree.begin()+size/2;
      while (p != main_tree.end()){
        std::cout << p->data << " ";
        p->substructure->print();
        p++; 
      }
    }//print_tree()
    

    //print the main tree
    void print_tree() const {
      typename Tree_vector::const_iterator p;
      p = main_tree.begin();
      std::cout << std::endl << "last tree:" << std::endl;
      std::cout << "* ";
      int i = 1, j = 0;
      while (p != main_tree.end()){
        std::cout << p->data << "{";
        p->substructure->print();
        std::cout << "} ";
        if (i++ == pow(2,j)) {
	  std::cout << std::endl << "|-";
          //p->substructure->print();
          std::cout << std::endl << "| ";
          i = 1;
          j++;
        }//if
        p++;
  

      }//while
      std::cout << "---end_of_tree---" << std::endl;
    }//print_tree()
    
    // builds the main tree from the container, which the iterators index   
    void build_tree(Input_data begin,
                    Input_data end,
                    int n, int vector_place, int d) {

       if (n == 1) { //leaf
         main_tree[vector_place].data = (*begin).get_coordinate(d);
         main_tree[vector_place].dData = new std::vector<C_Data>(1);
         (*main_tree[vector_place].dData)[0] = (*begin);
         main_tree[vector_place].substructure = new Associated_structure<C_Data>(n);
         main_tree[vector_place].substructure->build_struct(begin, end, d-1);
         //main_tree[vector_place].substructure->print(); 
       } 
       else {
         
         main_tree[vector_place].substructure = new Associated_structure<C_Data>(n); 
         
         main_tree[vector_place].data = (*(begin+(n/2-1))).get_coordinate(d);
         build_tree(begin, end-(n/2), 
                    n/2, left_child(vector_place), d);
         build_tree(begin+(n/2), end,
                    n/2, right_child(vector_place), d);
         
         main_tree[vector_place].dData = new std::vector<C_Data> (n);

         std::merge(main_tree[left_child(vector_place)].dData->begin(),
                    main_tree[left_child(vector_place)].dData->end(),
                    main_tree[right_child(vector_place)].dData->begin(),
                    main_tree[right_child(vector_place)].dData->end(),  
                    main_tree[vector_place].dData->begin(), 
                    *(C_Data::get_comp(0)));
         typename std::vector<C_Data>::iterator kl;
         
         main_tree[vector_place].substructure 
                 ->build_struct(main_tree[vector_place].dData->begin(), 
                                main_tree[vector_place].dData->end(), 
                                d-1);
         
         // FRACTIONAL CASCADING //
         //
         C_Data median = *(begin+(n/2-1));
          
         int i=0, j=0, k=0;
         for (Input_data p = main_tree[vector_place].dData->begin();
                         p != main_tree[vector_place].dData->end(); p++){
           if ((*p).get_coordinate(d) <= median.get_coordinate(d)) {
             main_tree[vector_place].substructure->put_frac_casc(
                  k,
                  main_tree[left_child(vector_place)].substructure->get_frac_casc(i),  
                  true); 
             if (j != n/2) {//if not, then the right structure is full so point to null 
               main_tree[vector_place].substructure->put_frac_casc(
                  k,
                  main_tree[right_child(vector_place)].substructure->get_frac_casc(j),
                  false);
             }
             i++; k++;
           }
           else{  
             if (i != n/2) {//if not, then the left structure is full so point to null
               main_tree[vector_place].substructure->put_frac_casc(
                  k,
                  main_tree[left_child(vector_place)].substructure->get_frac_casc(i),  
                  true); 
             }
             main_tree[vector_place].substructure->put_frac_casc(
                  k,
                  main_tree[right_child(vector_place)].substructure->get_frac_casc(j), 
                  false);
             j++; k++; 
           }
         }//for 
       
       }//else not leaf    
    }

    //last tree prebuild not used
    void pre_build_tree(std::vector<C_Data>& input) {
      //std::sort(input.begin(), input.end(), C_Data::get_comp(d));
      //std::sort(input.begin(), input.end(), T::ycompare);
      
    } 
  
  private:
    // returns the index of the node in which 
    // the paths (from root) of x_1, x_2 splits 
    int find_split_node(double x_1, double x_2) {
      int split = 0;
      while ((!is_leaf(split)) && (x_1 > main_tree[split].data 
                         || x_2 <= main_tree[split].data)) {
        split = main_tree[split].data >= x_1 
              ? left_child(split) : right_child(split);
      }
      return split;
    }
     
    // reports the subtree of the node with index v
    void report_subtree(int v, std::vector<double>& result) {
      if (!is_leaf(v)) { 
        report_subtree(left_child(v), result);
        report_subtree(right_child(v), result);
      }
      else
        result.push_back(main_tree[v].data);
    }

  public:
    // performs a range query in the last tree
    void range_query(C_Data from, C_Data to, int d, 
                     std::vector<C_Data>& result) {
      double x_1 = from.get_coordinate(d);
      double x_2 = to.get_coordinate(d);
      double y_1 = from.get_coordinate(d-1);
      double y_2 = to.get_coordinate(d-1);
      int split = find_split_node(x_1, x_2);
      typename std::vector<Associated_structure_node<C_Data> >::iterator assoc_split;
      assoc_split = main_tree[split].substructure->assoc_binary_search(y_1);
      if (assoc_split !=  main_tree[split].substructure->get_end()){
      //the answer of binary search is inside the index
      Associated_structure_node<C_Data> assoc_v = *assoc_split;

      if (is_leaf(split)) { 
        if (x_1 <= main_tree[split].data && main_tree[split].data <= x_2) {
            main_tree[split].substructure->report_structure(assoc_v, result, y_2); 
        }
      }
      else {
        //left path
        int v = left_child(split);
        assoc_v = *assoc_split;
        if (assoc_v.leftstructure != NULL)
          assoc_v = *(assoc_v.leftstructure);
        while (!is_leaf(v)) {
          if (x_1 <= main_tree[v].data) {
            if (assoc_v.rightstructure != NULL)
              main_tree[right_child(v)].substructure->
                      report_structure(*(assoc_v.rightstructure), result, y_2); 
            v = left_child(v);
            if (assoc_v.leftstructure != NULL)
              assoc_v = *(assoc_v.leftstructure);
          } 
          else {
            v = right_child(v); 
            if (assoc_v.rightstructure != NULL)
              assoc_v = *(assoc_v.rightstructure); 
          }          
        }
        if (x_1 <= main_tree[v].data && main_tree[v].data <= x_2) {
          main_tree[v].substructure->report_structure(assoc_v, result, y_2);
        } 

        //right path
        v = right_child(split);
        assoc_v = *assoc_split;
        if (assoc_v.rightstructure != NULL)
          assoc_v = *(assoc_v.rightstructure);
        while (!is_leaf(v)) {
          if (main_tree[v].data <= x_2) {
            if (assoc_v.leftstructure != NULL)
              main_tree[left_child(v)].substructure->
                            report_structure(*(assoc_v.leftstructure), result, y_2); 
            
            v = right_child(v);
            if (assoc_v.rightstructure != NULL)
              assoc_v = *(assoc_v.rightstructure);
          } 
          else {
            v = left_child(v);
            if (assoc_v.leftstructure != NULL)
              assoc_v = *(assoc_v.leftstructure);
          }           
        }
        if (x_1 <= main_tree[v].data && main_tree[v].data <= x_2) {
          main_tree[v].substructure->report_structure(assoc_v, result, y_2); 
        }
      }//else
     } else {
       std::cout << "error: binary search out of borders\n";
     }
    }//range_query

  private:
    // functions for index arithmetic
    int parent(int i) const {
      return (i != 0 ? (i-1)/2 : -1);
    }
    
    int left_child(int i) const {
      return (2*i+1 <= size ? 2*i+1 : -1);
    }
    
    int right_child(int i) const {
      return (2*i+2 <= size ? 2*i+2 : -1);
    }
    
    bool is_leaf(int i) const {
      return (i >= size/2 ? true : false);
    }
  private:
    int size;
    Tree_vector main_tree; 
};

//
// the class with the definitions of the range trees
//
template <class C_Data, class T>
class Layered_range_tree {

  typedef Layered_range_tree_node<C_Data,T> Tree_node;
  typedef typename std::vector<Tree_node> Tree_vector;
  typedef typename std::vector<C_Data>::iterator Input_data;
   
  public:
    //constructors
    Layered_range_tree(int n) : size(2*n-1), main_tree(2*n-1) { }
    
    //destructor
    ~Layered_range_tree() {
      this->delete_tree();
    }
    
    void delete_tree() {
      for (int i=0; i < size; i++) {
        main_tree[i].substructure->delete_tree();
      }
      main_tree.clear(); 
    }
    
    // print tree's vector
    void print_tree_vector() const {
      typename Tree_vector::const_iterator p;
      p = main_tree.begin()+size/2;
      while (p != main_tree.end()){
        std::cout << p++->data << " ";
      }
    }//print_tree()
    
    
    //print the main tree
    void print_tree() const {
      typename Tree_vector::const_iterator p;
      p = main_tree.begin();
      std::cout << std::endl << "main(or)high tree:" << std::endl;
      std::cout << "* ";
      int i = 1, j = 0;
      while (p != main_tree.end()){
        std::cout << p->data << "{ ";
        p->substructure->print_tree();
        std::cout << "} ";
        if (i++ == pow(2,j)) {
	  std::cout << std::endl << "|-";
          i = 1;
          j++;
        }//if
        p++;
      }//while
      std::cout << "---end_of_tree---" << std::endl;
    }//print_tree()
    
    // builds the main tree from the container, which the iterators index   
    void build_tree(Input_data begin,
                    Input_data end,
                    int n, int vector_place, const int d) {

       if (n == 1) { //leaf
         main_tree[vector_place].data = (*begin).get_coordinate(d);
         main_tree[vector_place].dData = new std::vector<C_Data>(1);
         (*main_tree[vector_place].dData)[0] = (*begin);
         
         
         main_tree[vector_place].substructure = new T(n);
         main_tree[vector_place].substructure
                 ->build_tree(main_tree[vector_place].dData->begin(), 
                              main_tree[vector_place].dData->end(), 
                              n, 0, d-1);
       } 
       else {
           if (d > 2){
             main_tree[vector_place].substructure = new T(n);
           }

         build_tree(begin, end-(n/2),
                    n/2, left_child(vector_place), d);
         build_tree(begin+(n/2), end,
                    n/2, right_child(vector_place), d);

         main_tree[vector_place].data = (*(begin+(n/2-1))).get_coordinate(d);
         
         main_tree[vector_place].dData = new std::vector<C_Data> (n);
         
         std::merge(main_tree[left_child(vector_place)].dData->begin(),
                    main_tree[left_child(vector_place)].dData->end(),
                    main_tree[right_child(vector_place)].dData->begin(),
                    main_tree[right_child(vector_place)].dData->end(),  
                    main_tree[vector_place].dData->begin(), 
                    *(C_Data::get_comp(d-2)));
         
         main_tree[left_child(vector_place)].dData->clear();
         main_tree[right_child(vector_place)].dData->clear();
         
         main_tree[vector_place].substructure 
                 ->build_tree(main_tree[vector_place].dData->begin(), 
                              main_tree[vector_place].dData->end(), 
                              n, 0, d-1);
       }    
    }


    void pre_build_tree(std::vector<C_Data>& input, int d) {
      std::sort(input.begin(), input.end(), *(C_Data::get_comp(d-1)));
    } 
  
  private:  
    // returns the index of the node in which 
    // the paths (from root) of x_1, x_2 splits 
    int find_split_node(double x_1, double x_2) {
      int split = 0;
      while ((!is_leaf(split)) && (x_1 > main_tree[split].data 
                         || x_2 <= main_tree[split].data)) {
        split = main_tree[split].data >= x_1 
              ? left_child(split) : right_child(split);
      }
      return split;
    }
     
    // reports the subtree of the node with index v
    void report_subtree(int v, std::vector<double>& result) {
      if (!is_leaf(v)) { 
        report_subtree(left_child(v), result);
        report_subtree(right_child(v), result);
      }
      else
        result.push_back(main_tree[v].data);
    }
  
  public:
    // performs a range query
    void range_query(C_Data from, C_Data to, int d, 
                     std::vector<C_Data>& result) {
      double x_1 = from.get_coordinate(d);
      double x_2 = to.get_coordinate(d);
      int split = find_split_node(x_1, x_2);
 
      if (is_leaf(split)) { 
        if (x_1 <= main_tree[split].data && main_tree[split].data <= x_2) {
            if (d != 1)
          main_tree[split].substructure->range_query(from, to, d-1, result); 
        }
      }
      else {
        //left path
        int v = left_child(split);
        while (!is_leaf(v)) {
          if (x_1 <= main_tree[v].data) {
            if (d != 1)
            main_tree[right_child(v)].substructure->
                      range_query(from, to, d-1, result);
            v = left_child(v);
          } 
          else {
            v = right_child(v); 
          }          
        }
        if (x_1 <= main_tree[v].data && main_tree[v].data <= x_2) {
            if (d != 1)
          main_tree[v].substructure->range_query(from, to, d-1, result); 
        }
        //right path
        v = right_child(split);
        while (!is_leaf(v)) {
          if (main_tree[v].data <= x_2) {
            if (d != 1)
            main_tree[left_child(v)].substructure->
                              range_query(from, to, d-1, result);
            v = right_child(v);
          } 
          else {
            v = left_child(v);
          }           
        }
        if (x_1 <= main_tree[v].data && main_tree[v].data <= x_2) {
            if (d != 1)
          main_tree[v].substructure->range_query(from, to, d-1, result);
        }
      }//else
    }//range_query
  
 private:
    // functions for index arithmetic
    int parent(int i) const {
      return (i != 0 ? (i-1)/2 : -1);
    }
    
    int left_child(int i) const {
      return (2*i+1 <= size ? 2*i+1 : -1);
    }
    
    int right_child(int i) const {
      return (2*i+2 <= size ? 2*i+2 : -1);
    }
    
    bool is_leaf(int i) const {
      return (i >= size/2 ? true : false);
    }
  private:
    int size;
    Tree_vector main_tree; 
};


