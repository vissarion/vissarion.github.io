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
      Double_data() : x(10) { };

      Double_data(int i) : x(i) { };
      
      // used by binary search in fractional cascading level 
      // it only needs the first coordinate assigned to fr.casc.
      Double_data(double i) : x(10,i) { };
      
      double get_coordinate(int d) const{
        switch (d){
          case 1: return x[0]; break;
          case 2: return x[1]; break;
          case 3: return x[2]; break;
          case 4: return x[3]; break;
          case 5: return x[4]; break;
          case 6: return x[5]; break;
          case 7: return x[6]; break;
          case 8: return x[7]; break;
          case 9: return x[8]; break;
          case 10: return x[9]; break;
          default: return -1;
        }
      }
     
      void put_value(std::vector<double> x_) {
        x = x_;
      }
      
      typedef bool (*pt2CompFun)(const Double_data&, const Double_data&);

     
      static bool x0compare(const Double_data& lhs, const Double_data& rhs) {
        return lhs.x[0] < rhs.x[0];
      }
      static bool x1compare(const Double_data& lhs, const Double_data& rhs) {
        return lhs.x[1] < rhs.x[1];
      }
      static bool x2compare(const Double_data& lhs, const Double_data& rhs) {
        return lhs.x[2] < rhs.x[2];
      }
      static bool x3compare(const Double_data& lhs, const Double_data& rhs) {
        return lhs.x[3] < rhs.x[3];
      }
      static bool x4compare(const Double_data& lhs, const Double_data& rhs) {
        return lhs.x[4] < rhs.x[4];
      }
      static bool x5compare(const Double_data& lhs, const Double_data& rhs) {
        return lhs.x[5] < rhs.x[5];
      }
      static bool x6compare(const Double_data& lhs, const Double_data& rhs) {
        return lhs.x[6] < rhs.x[6];
      }
      static bool x7compare(const Double_data& lhs, const Double_data& rhs) {
        return lhs.x[7] < rhs.x[7];
      }
      static bool x8compare(const Double_data& lhs, const Double_data& rhs) {
        return lhs.x[8] < rhs.x[8];
      }
      static bool x9compare(const Double_data& lhs, const Double_data& rhs) {
        return lhs.x[9] < rhs.x[9];
      }

      static std::vector<pt2CompFun> comparray;

      static void comp_array_init(){
        comparray[0] = &x0compare;
        comparray[1] = &x1compare;
        comparray[2] = &x2compare;
        comparray[3] = &x3compare;
        comparray[4] = &x4compare;
        comparray[5] = &x5compare;
        comparray[6] = &x6compare;
        comparray[7] = &x7compare;
        comparray[8] = &x8compare;
        comparray[9] = &x9compare;
      } 
      static pt2CompFun get_comp(int d){
        return comparray[d];
      }
 
    private:
      std::vector<double> x; 
};
//initialiation of static members
typedef bool (*pt2CompFun)(const Double_data&, const Double_data&);
std::vector<pt2CompFun> Double_data::comparray(10);



