// Layered Range Trees 
//
// Copyright (c) 2011 Vissarion Fisikopoulos
//
// Licensed under GNU LGPL.3, see LICENCE file


#include <iostream>
#include <fstream>
#include <cmath>
#include <time.h>

#include <vector>
#include <iterator>
#include <algorithm>
#include <unistd.h>

#include "../include/Layered_Range_tree.h"
#include "../include/Layered_Range_tree_traits_10.h"



int main(int argc, char *argv[]){

  // the dimension of the tree (attention!!! must be correct)
  const int d = 10;
  
  // options of main program 
  int index, c, printree=0, printanswer=0, i, j;
  char * filename = NULL;
  opterr = 0;
  std::vector<double> vec(d);
  
  while ((c = getopt (argc, argv, "f:tah")) != -1)
    switch (c)
    {
      case 'f':
        std::cout << "  option '--file' enabled. Reading from file: " 
                  << optarg << std::endl;
	filename = optarg;
        break;
      case 't':
        std::cout << "  option '--printree' enabled.\n";
        printree = 1;
        break;
      case 'a':
        std::cout << "  option '--printanswer' enabled.\n";
        printanswer = 1;
        break;
      case 'h':
        printf("Usage:\n");
        printf(" %s -f Reads data from file\n",argv[0]);
        printf(" %s -t Prints tree\n",argv[0]);
        printf(" %s -h Display this help\n",argv[0]);
        return 0;
        break;
      case '?':
        if (optopt == 'f')
          fprintf (stderr, "Option -%c requires an argument.\n", optopt);
        else if (isprint (optopt))
          fprintf (stderr, "Unknown option `-%c'.\n", optopt);
        else
          fprintf (stderr, "Unknown option character `\\x%x'.\n", optopt);
             return 1;
        default:
          abort ();
    }
    
    for (index = optind; index < argc; index++)
       printf ("Non-option argument %s\n", argv[index]);

    std::cout << std::endl;

  
  // time variables
  clock_t start,end;


  // TEMPLATED DATA
  Double_data a;
  std::vector<Double_data> intvect;
  
  if (filename) { 
    //READ DATA FROM FILE
    std::ifstream indata; 
    double xin, yin, zin, win; // variables for input value

    indata.open(filename); // opens the file
    if(!indata) { // file couldn't be opened
      std::cerr << "Error: file " << filename 
                << " could not be opened" << std::endl;
      exit(1);
    }
    do{
      indata >> xin >> yin >> zin >> win;
      //indata >> xin >> yin >> zin;
     
      if ( !indata.eof() ){
        //a.put_value(xin, yin, zin, win, win);
        //a.put_value(xin, yin, zin);
        intvect.push_back(a);
      }
      //std::cout << xin << " ";
    } while ( !indata.eof() );
    indata.close();
    //End-of-file reached..
  } else { 
    //DEFAULT INPUT DATA
    for (i=1; i<= 16 ; i++){
      for (j=0; j<d; j++)
        vec[j] = i*(j+1);
      a.put_value(vec);
      intvect.push_back(a);
    }
  }  
  
  // initialization of the static vector of compare functions
  // of input data
  Double_data::comp_array_init();
  
  int size = intvect.size();
  
 
  //TEMPLATED TREE
   
  Layered_range_tree <Double_data, 
   Layered_range_tree <Double_data, 
    Layered_range_tree <Double_data, 
     Layered_range_tree <Double_data, 
      Layered_range_tree <Double_data, 
       Layered_range_tree <Double_data, 
        Layered_range_tree <Double_data, 
         Layered_range_tree <Double_data, 
          Last_range_tree <Double_data>  
          
         >
        >
       >
      >
     >
    >
   >
  > tree(size);
  // *****************************************************************
  // test
  //Double_data::i = 4;
  //std::cout << Double_data::get_comp(0)(*(intvect.begin()),*(intvect.end()));
  //std::sort(intvect.begin(), intvect.end(), *(Double_data::get_comp(1)));
  //std::vector<Double_data>::iterator it = intvect.begin();
  //while (it != intvect.end()){
    //std::cout << "(" << (*it).get_coordinate(1) << ","
    //          << (*it).get_coordinate(2) << ","
    //          << (*it).get_coordinate(3) << ") ";
    //it++;
 // }


  //DOUBLE TREE
  tree.pre_build_tree(intvect, d);
 
  std::cout << "building range tree structure ..." << std::endl;
  start = clock();
  tree.build_tree(intvect.begin(), intvect.end(), size, 0, d);  
  end = clock();
  std::cout << "Build time: " 
            << (double)((end-start)/(double)(CLOCKS_PER_SEC))  
            << std::endl;

  if (printree)
    tree.print_tree();


  // d-DIMENSIONAL QUERY
  // refer all the d-D points which lay inside the rectangular
  // formed by the points "from" and "to"
  std::vector<Double_data> outvect;
  
  Double_data from, to;
  for (i=0; i< d; i++)
    vec[i] = 1;
  from.put_value(vec);
  for (i=0; i< d; i++)
    vec[i] = pow(10,i+1);
  //vec[3] = 20;
  to.put_value(vec);
  //from.put_value(3,4,70);
  //to.put_value(28,9000,3000);
  
  std::cout << std::endl << "query question:[" << from.get_coordinate(1) << "," 
                                  << to.get_coordinate(1);
  for (i=2; i<=d; i++) 
    std::cout <<  "]x[" << from.get_coordinate(i) << "," << to.get_coordinate(i);
  std::cout << "]\n\n";

  std::cout << "performing the range query ..." << std::endl;
  start = clock();
  tree.range_query(from, to, d, outvect);
  end = clock();
 //(double)((time2-time1)/CLOCKS_PER_SEC)  
  std::cout << "Query time: " << (double)((end-start)/(double)(CLOCKS_PER_SEC))  << std::endl;
  std::cout << outvect.size() << " points reported.\n";
  // sort the answer
  std::cout << "Sorting the answer..." << std::endl;
  std::sort(outvect.begin(), outvect.end(), *(Double_data::get_comp(0)));
  
  if (printanswer){
    // PRINT ANSWER
    std::cout << "Answer:\n---------------\n"; 
    std::vector<double>::iterator q;
    std::vector<Double_data>::iterator q2 = outvect.begin();
    while (q2 != outvect.end()){
      std::cout << "(";
      for (i=1; i<=d; i++){
        std::cout << (*q2).get_coordinate(i);
        if (i<d) std::cout << ",";
      }
      std::cout << ") " << std::endl;
      q2++;
    }
  }
  std::cout << std::endl;
  return 0;
}
