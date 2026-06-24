// Layered Range Trees 
//
// Copyright (c) 2011 Vissarion Fisikopoulos
//
// Licensed under GNU LGPL.3, see LICENCE file

//
// a class with the definition of sample input data
// and their comparision operators
//
struct Double_data {
    public:
      Double_data() : x(0), y(0) { };

      Double_data(double i) : x(i), y(i) { };

      double get_coordinate(int d) const{
        switch (d){
          case 1: return x; break;
          case 2: return y; break;
         default: return -1;
        }
      }
     
      void put_value(double x_, double y_) {
        x = x_; y = y_;
      }
      
      typedef bool (*pt2CompFun)(const Double_data&, const Double_data&);

     
      static bool xcompare(const Double_data& lhs, const Double_data& rhs) {
        return lhs.x < rhs.x;
      }
      static bool ycompare(const Double_data& lhs, const Double_data& rhs) {
        return lhs.y < rhs.y;
      }

      static std::vector<pt2CompFun> comparray;

      static void comp_array_init(){
        comparray[0] = &xcompare;
        comparray[1] = &ycompare;
      } 
      static pt2CompFun get_comp(int d){
        return comparray[d];
      }
 
    private:
      double x;
      double y;
};
//initialiation of static members
typedef bool (*pt2CompFun)(const Double_data&, const Double_data&);
std::vector<pt2CompFun> Double_data::comparray(2);

