// -*- mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// --------------------------------------------------------------------------
//                   OpenMS Mass Spectrometry Framework
// --------------------------------------------------------------------------
//  Copyright (C) 2003-2011 -- Oliver Kohlbacher, Knut Reinert
//
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 2.1 of the License, or (at your option) any later version.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
// --------------------------------------------------------------------------
// $Maintainer: Florian Zeller $
// $Authors: Lukas Mueller, Markus Mueller $
// --------------------------------------------------------------------------
//
///////////////////////////////////////////////////////////////////////////
//
//  written by Lukas N Mueller, 30.3.05
//  Lukas.Mueller@imsb.biol.ethz.ch
//  Group of Prof. Ruedi Aebersold, IMSB, ETH Hoenggerberg, Zurich
//
//  Ported to OpenMS by Florian Zeller, florian.zeller@bsse.ethz.ch
//  December 2010
//


#ifndef _LC_MS_H
#define _LC_MS_H

#include <string>
#include <vector>
#include <map>

namespace OpenMS
{

typedef feature LC_MS_FEATURE;

class OPENMS_DLLAPI LC_MS{

  
  ////////////////////////////////////////////////
  // declaration of the private members:
  
private:
  
  // name of the spectra:
  std::string spec_name;
  
  // vector of object feature:
  std::vector<LC_MS_FEATURE> feature_list;
  
  // a unique specrum id to identify a spectrum:
  int spectrum_id;
  
  // MASTER RUN ID:
  int MASTER_ID;
  
  // the LC-MS raw data names and their IDs
  std::map<int, std::string> raw_spec_names;
  
  // alignment error:
  std::map<double, std::pair<double,double> > ALIGNMENT_ERROR;
 
 ////////////////////////////////////////////////
 // declaration of the public members:
 
public:
    
    static double MINIMAL_PEP_PROPHET_THERSHOLD;
  static double PEP_PROPHET_THERSHOLD;

    
    // class destructor
   ~LC_MS();
 
  // class constructor
  LC_MS(std::string);
  LC_MS();
  // copy constructor
  LC_MS(const LC_MS*);
  
  // copy constructor
  LC_MS(const LC_MS&);
  
  // show the content of the spectra
  void show_info();
  
  // copy constructor:
  LC_MS& operator=(const LC_MS&);
  
  // sort the features according their parent mass:
  void order_by_mass();
 
  // function to compare the feature mass:
  float compare_feature_mass(const void*,const void*);
 
  
  // this structure provides the function to compare
  // in the sorting algorithm:
  struct OPERATOR_MZ{
    // provide the compare function for sort:
    bool operator()(const feature A,const feature B) const{
      // check if they have same mass
      if(A.MONO_MZ == B.MONO_MZ){
        return A.TR < B.TR;
      }
      else{
        return A.MONO_MZ < B.MONO_MZ;
      }
    }
  };
  
  
  // this structure provides the function to compare
  // in the sorting algorithm:
  struct OPERATOR_FeatureCompare{
    // provide the compare function for sort:
    bool operator()(const feature A,const feature B) const{
      // check if they have same mass
      if(A.feature_ID == B.feature_ID){
        return true;
      }
      else{
        return false;
      }
    }
  };
  
  
  
  
    
  // tag the feature with the spectrum id:
  void tag_peaks_with_spectrum_ID(){
    std::vector<LC_MS_FEATURE>::iterator p = feature_list.begin();
    while(p != feature_list.end()){
      (*p).set_spectrum_ID(get_spectrum_ID());
      p++;
    }
  };
  
  // count the number of common peaks of a given number of LC-MS:
  int get_nb_common_peaks( int);
  
  
 
  ///////////////////////////////
  // start here all the get / set
  // function to access the
  // variables of the class
  
  // get the whole feature list:
  void clear_feature_list(){return feature_list.clear();};
  std::vector<LC_MS_FEATURE> get_feature_list(){return feature_list;};
  std::vector<LC_MS_FEATURE>* get_feature_list_reference(){return &feature_list;};
  bool check_feature_list_empty(){return feature_list.empty();};
  
  // access end /start of list:
  std::vector<LC_MS_FEATURE>::iterator get_feature_list_begin(){return feature_list.begin();};
  std::vector<LC_MS_FEATURE>::iterator get_feature_list_end(){return feature_list.end();};
  
  // add a new feature to the list:
  void add_feature( feature* IN){
    
    if( IN->get_feature_ID() == -1 ){
      IN->set_feature_ID( feature_list.size() );
    }
    feature_list.push_back( *IN );
    IN = NULL;
  };
  
  // remove a feature from the LC/MS run by ID:
  void remove_feature_by_ID(feature*);
  void remove_feature_by_ID(int);
  // remove a feature from teh LC/MS run:
  void remove_feature(feature*);
  void remove_feature(int i){
    if( i < int(feature_list.size()) ){
      feature_list.erase( feature_list.begin() + i );
    }
  }
  
  // remove a feature by iterator and return the iterator to the next element
  std::vector< LC_MS_FEATURE >::iterator remove_feature_from_list(std::vector< LC_MS_FEATURE >::iterator IN){return feature_list.erase(IN);}; 
  
  // get number of feature added:
  unsigned int get_nb_features(){return feature_list.size();};
  
  std::string get_spec_name(){return spec_name;};
  void set_spec_name(std::string IN){spec_name = IN;};
  
  // set / get spectrum id:
  int get_spectrum_ID(){return spectrum_id;};
  void set_spectrum_ID(int IN){spectrum_id = IN;};
  
  // set the id of all features
  void setFeatureLCMSID();
 
  // search the list of feature for the one with input ID:
  feature* find_feature_by_ID(int);
  
  // access the raw data names:
  void remove_raw_spec_name(int ID){raw_spec_names.erase( ID );};
  void add_raw_spec_name(int ID, std::string name){raw_spec_names.insert( make_pair(ID, name) );};
  bool check_raw_spec_name_empty(){return raw_spec_names.empty();};
  std::map<int, std::string>::iterator get_raw_spec_name_start(){return raw_spec_names.begin();};
  std::map<int, std::string>::iterator get_raw_spec_name_end(){return raw_spec_names.end();};
  std::map<int, std::string> get_raw_spec_name_map(){ return raw_spec_names;};
  int get_nb_raw_specs(){ return raw_spec_names.size();};
  std::string get_raw_spec_name(int ID){ std::map<int, std::string>::iterator p = raw_spec_names.find(ID); 
    if( p == raw_spec_names.end() ){
      return "";
    }
    return (*p).second;
  };
  // compare the LC/MS runs names 
  bool check_LCMS_name( std::string );


  // check if this LC/MS ID is present in the raw LC/MS runs 
  bool find_LC_MS_by_ID( int );
  
  // add the raw spectrum map:
  void add_raw_spec_name_map(std::map<int, std::string> IN){ 
    std::map<int, std::string>::iterator p = IN.begin(); 
    while(p != IN.end()){
      int ID = (*p).first;
      std::map<int, std::string >::iterator F = raw_spec_names.find( ID );
      if( F != raw_spec_names.end() ){
        ID += raw_spec_names.size();
      }
      raw_spec_names.insert( make_pair( ID ,(*p).second) );
      p++;
    }
  };
  
  // counts the number of ms features, which contain MS2 info:
  int get_nb_identified_features(){
    int count = 0;
    std::vector< LC_MS_FEATURE >::iterator P = get_feature_list_begin();
    while(P != get_feature_list_end()){
      if( (*P).get_MS2_info() )
        count++;
      P++;
    }
    return count;
  };
  
  // counts the number of ms features, which contain MS2 info (no thresholding)
  int get_nb_identified_features( double PepProb_T){
    int count = 0;
    std::vector<LC_MS_FEATURE >::iterator P = get_feature_list_begin();
    while(P != get_feature_list_end()){
      if( (*P).get_MS2_info( PepProb_T ) )
        count++;
      P++;
    }
    return count;
  };
  
   //////////////////////////////////
  // access the alignment error:
  // save an error:
  void add_alignment_error(double TR, double ERROR_UP, double ERROR_DOWN){ 
    std::pair<double,double> tmp(ERROR_UP,ERROR_DOWN);
    ALIGNMENT_ERROR.insert(std::pair<double, std::pair<double,double> >(TR, tmp));
  };
  
  // get alignment error at specific TR:
  void get_alignment_error(double,  double*, double*);
  
  // access MASTER run ID:
  void set_MASTER_ID(int IN){ MASTER_ID = IN;};
  int get_MASTER_ID(){ return MASTER_ID;};
};

}

#endif
