#ifndef _LS_DYNA_READER_
#define _LS_DYNA_READER_

#include <string>
#include <vector>
#include <set>
#include <map>

namespace LS_Dyna {

class Keyword {
public:
};

#define FLOAT_FIELD   0
#define INT_FIELD     1

enum elem_type {_Solid_=0, _Shell_, _SPH_};

double readDoubleField(std::string &str, const int &pos, const int &length);
int    readIntField   (std::string &str, const int &pos, const int &length);
  
struct ls_node {
  void ls_nodeconst (const int &id_, const double &x, const double &y, const double &z){
    m_id = id_;
    m_x[0]=x;m_x[1]=y;m_x[2]=z;
  }
  int m_id;
  int id_sph_el;
  double m_x[3];
};

struct ls_element {
  int id;
  int pid;  //Part
  double mass;
  std::vector <int> node; /////// THIS IS THE ------POSITION--------
  elem_type m_type;
};

struct ls_set_node {
  int id;
  std::vector <int> node;
};

struct ls_property {
  
  
};

struct ls_section{
  
  
};

struct ls_material{
  
  
};

struct ls_spc_node{
  int  m_node_id;
  bool m_fix_dof[6];
};

/////////////////////////
//NON CLASS FUNCTIONS ///
/////////////////////////
// FOR FORTRAN OR OTHER LANGUAGE VERSION
extern "C" void removeComments(std::vector <std::string>);
extern "C" void LSDYNA_getLines(char* fname, char ***lines, double **nodes, int *node_count);
extern "C" void readNodes(char *fName, double **nodes, int *node_count);
void readSPCNodes(int *sections, int **node_ids, bool **dofs);

class lsdynaReader{
public:  
  lsdynaReader(){
    m_elem_count_type.push_back(10);
  }
  lsdynaReader(const char *);

  int m_line_count;
  int m_node_count;
  int m_elem_count;

  std::vector<int> m_elem_count_type;
  
  std::vector <std::string> m_line;
  void readNodes();
  void readCommands();
  void removeComments();
  void readElementSolid();
  void readElementSPH();
  void readSPCNodes();
  bool findSection(std::string str, int * ini_pos, int *end_pos, int start_pos = 0);
  bool readBPMNodes(); //*BOUNDARY_PRESCRIBED_MOTION_NODE
  bool readSetNodes();
  
  bool readContacts(); //ONLY NODE TO SURFACE

  int getNodePos(const int &n);
  ls_node & getElemNode(const int &e, const int &n);

  std::vector < ls_node    > m_node;
  std::vector < ls_element > m_elem;
  std::vector < ls_spc_node > m_spc_nod;
  std::vector < ls_set_node > m_set_nod;
  


  //WOULD BE FASTER A PAIR??
  std::set<int>     m_command_line;
  std::vector<std::string>  m_command;
  std::map<int, int> m_node_map;
};

};


#endif
