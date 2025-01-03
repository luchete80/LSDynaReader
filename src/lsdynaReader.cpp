#include "lsdynaReader.h"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <stdio.h>

using namespace std;

namespace LS_Dyna {
  
bool isComment(const string &line){
  
  bool ret = false;
  if (line.find('$') == 0)
    ret = true;
  return ret;
}
// str.erase(
  // std::remove_if(str.begin(), str.end(), [](char chr){ return chr == '&' || chr == ' ';}),
  // str.end());

int findNextCommandLine(const int &curr_line, std::vector <std::string> line) {
  string first;
  int i = curr_line;
  bool end = false;
  while (!end) {
    i++;
    if (i==line.size()) end = true; 
      first = line[i].substr(0,1);
      //cout << "first "<<first <<", "<<line[i]<<endl;
      //Can a line start with spaces??
      //if (first.find("$")!=string::npos)
      if (first=="*"){
        //cout << "FOUND * at line "<<i<<endl;
        end = true;
      }
  }// while end
  
        //cout << "* NOT FOUND "<<i<<endl;
  return i;
}

void removeComments(std::vector <string> m_line){
  std::vector<std::string>::iterator it;
  int i=0;
  for (it = m_line.begin();it!=m_line.end();){
    string first = it->substr(0,1);
    if (first=="$"){
      //cout << "first"<<first <<endl;
      //cout << "it "<<*it<<endl;
      m_line.erase(it);
      //m_line_count--;
    } else {
      it ++;
      //cout << "NOT $ FOUND "<<first<<", LIN: "<<*it<<endl;
    }
    i++;
  }
}

// Función para eliminar los elementos del vector que contengan cierto caracter en la primera posición
void lsdynaReader::removeComments(/*std::vector<std::string>& vec, char caracter*/) {
    char caracter = '$';
    // Usamos la función erase-remove idiom para eliminar los elementos que cumplan con la condición
    m_line.erase(std::remove_if(m_line.begin(), m_line.end(), [caracter](const std::string& str) {
        return !str.empty() && str[0] == '$';
    }), m_line.end());
    
    m_line_count = m_line.size();
}

string removeSpaces(string str) { 
    str.erase(remove(str.begin(), str.end(), ' '), str.end()); 
    return str; 
}

double readDoubleField(string &str, const int &pos, const int &length) {
  double ret = 0.0;
    std::string f_str = str.substr(pos,length);
    f_str = removeSpaces(f_str);

  if (str.size()<pos+length) {
    cout << "ERROR TRYING TO READ: "<<str<<", subs: " <<f_str<<endl;
    return ret;
  }
  // return ret;
  ret = strtod(f_str.c_str(),NULL);
  // cout << "readed: "<<ret;
  return ret;
}

int readIntField(string &str, const int &pos, const int &length) {
  int ret = 0.0;
    std::string f_str = str.substr(pos,length);
    f_str = removeSpaces(f_str);
  //cout << "reading "<<f_str.c_str()<<endl;
  if (str.size()<pos+length) {
    cout << "ERROR TRYING TO READ: "<<str<<", subs: " <<f_str<<endl;
    return ret;
  }
  // return ret;
  ret = stoi(f_str.c_str());
  //cout << "readed string "<<f_str<<endl;
  // cout << "readed: "<<ret;
  return ret;
}

bool findSection(std::vector<string> m_line, string str, int * ini_pos, int *end_pos, int start_pos = 0){
  
  bool end = false;
  bool found = false;
  int endpos;

  int i = start_pos;
  cout << "Reading "<<str<<endl;

  cout << "Size "<<m_line.size()<<endl;
  while (!end){
    cout << "i "<<i<<endl;
      if (m_line[i].find(str) != std::string::npos){
        found = true;
        cout << "Found section" << str << " at line "<<i<<endl;
        *ini_pos = i+1;
        //*endpos = findNextCommandLine(i,m_line) - i;
        *end_pos = findNextCommandLine(i,m_line)-1;
        cout << "Section length: "<<endpos<<endl;
        end = true;
      }
    if (i==m_line.size()-1) {
      end = true;
      cout << str<<" not defined "<<endl;
    }
    i++;
  } 
  return found;
}

bool lsdynaReader::findSection(string str, int * ini_pos, int *end_pos, int start_pos){
  
  bool end = false;
  bool found = false;
  int i = start_pos;
  cout << "Reading "<< str << endl;

  while (!end){

      if (m_line[i].find(str) != std::string::npos){
        found = true;
        cout << "Found section" << str << " at line "<<i<<endl;
        *ini_pos = i+1;
        *end_pos = findNextCommandLine(i,m_line) - 1;
        m_elem_count = *end_pos -*ini_pos +1 ;
        cout << "Section length: "<<m_elem_count<<endl;
        end = true;
      }
    if (i==m_line_count-1) {
      end = true;
      //cout << str <<" not found "<<endl;
    }
    i++;
  }
  cout << str << " section lines: "<<m_elem_count<<endl;
  return found;
}

void lsdynaReader::readNodes() {
  bool end = false;
  int ini_pos, end_pos;
  int i = 0;
  findSection ("*NODE", &ini_pos, &end_pos);
  cout << "Nodes at pos: "<<ini_pos <<", "<<end_pos<<endl;
  int npos=0;
  
  for (i=ini_pos;i<end_pos+1;i++){
    int id;
    id = readIntField(m_line[i], 0, 8);
    ls_node nod;
    nod.m_id = id;
    for (int d=0;d<3;d++)
      nod.m_x[d] = readDoubleField(m_line[i], 8+16*d, 16);
    
    if (i>end_pos-2){
      //cout << "Node id"<<id <<", npos: "<<npos<<", XYZ: "<<nod.m_x[0]<<", "<<nod.m_x[1]<<", "<<nod.m_x[2]<<endl;
      //cout << "pos "<<i<<", "<<"end_pos"<<end_pos<<endl;
    }      
    m_node.push_back(nod);
    m_node_map.insert(pair<int, int>(id, npos));
    
    npos++;
  }
  cout << "Node reading done."<<endl;  
  // map<int, int>::iterator it=m_node_map.begin();
  // for (int n=0;n<npos;n++){
    // cout << "Node "<< it->first<<", id pos w/map: "<<it->second<<", id from class var"<< m_node[n].m_id<<endl;
    // it++;
  // }

}  //line

int lsdynaReader::getNodePos(const int &n){

    if (n<m_node.size())
      return m_node_map[m_node[n].m_id];
  
}

void lsdynaReader::readElementSolid() {
  bool end = false;
  int ini_pos, end_pos;
  int i = 0;
  if (findSection ("*ELEMENT_SOLID", &ini_pos, &end_pos)){
  //cout<<"ELEMENT SOLID at "<< ini_pos<<" to "<<end_pos<<endl;
  for (i=ini_pos;i<end_pos+1;i++){
    int id, pid;
    ls_element ls_el;
    int nodecount;
    if (m_line[i].size()>16) nodecount = (int)((m_line[i].size()-16)/8);
    //cout << "Elkeemnt node count "<<nodecount<<endl;
    ls_el.id  = readIntField(m_line[i], 0, 8);
    ls_el.pid = readIntField(m_line[i], 1, 8);
    
    for (int d=0;d<nodecount;d++){
      int node_id = readIntField(m_line[i], 16+8*d, 8);
      ls_el.node.push_back(m_node_map[node_id]);
    
    }
      // cout << "Node "<<id <<"XYZ: "<<nod.m_x[0]<<", "<<nod.m_x[1]<<", "<<nod.m_x[2]<<endl; 
    
    m_elem.push_back(ls_el);

  }
  }
}  //line

void lsdynaReader::readElementSPH() {
  bool end = false;
  int ini_pos, end_pos;
  int i = 0;
  findSection ("*ELEMENT_SPH", &ini_pos, &end_pos);
  cout << "Searching SPH Elements..."<<endl;
  int el = 0;
  for (i=ini_pos;i<end_pos+1;i++){
    int id, pid;
    ls_element ls_el;
    int nodecount;
    //if (m_line[i].size()>16) nodecount = (int)((m_line[i].size()-16)/8);
    //cout << "Elem node pos "<<m_node_map[readIntField(m_line[i], 0, 8)] <<endl;
	int nodepos = m_node_map[readIntField(m_line[i], 0, 8)];
    ls_el.node.push_back(nodepos);
	m_node[nodepos].id_sph_el = el;
    ls_el.pid  = readIntField(m_line[i], 8, 8);
    ls_el.mass = readDoubleField(m_line[i], 16, 16);
    ls_el.m_type = _SPH_;
    m_elem_count_type[_SPH_]++;

    // cout << "Node "<<id <<"XYZ: "<<nod.m_x[0]<<", "<<nod.m_x[1]<<", "<<nod.m_x[2]<<endl; 
    m_elem.push_back(ls_el);
    el++;
  }

}  //line

/********************************************/
/* 1 NODE ID AND N DOF AND */
void lsdynaReader::readSPCNodes(){
  
    bool end = false;
  int ini_pos, end_pos;
  int i = 0;
  cout << "Searching Boundary SPC"<<endl;
  for (int c=0;c<m_command_line.size()-1;c++){ //*END IS NOT COUNTED

    if (findSection ("*BOUNDARY_SPC_NODE", &ini_pos, &end_pos)) {
    
      int k = 0;
      for (i=ini_pos;i<end_pos+1;i++){
        int id, cid;
        ls_spc_node ls_nspc;
        // cout << "Elem node count "<<nodecount <<endl;
        ls_nspc.m_node_id  = readIntField(m_line[i], 0, 10);
        //cout << "m_node "<<ls_nspc.m_node_id<<endl;
        // ls_el.cid = readIntField(m_line[i], 1, 8);
        
        for (int d=0;d<6;d++)
          ls_nspc.m_fix_dof[d] = readIntField(m_line[i], 20+10*d, 10);
          // cout << "Node "<<id <<"XYZ: "<<nod.m_x[0]<<", "<<nod.m_x[1]<<", "<<nod.m_x[2]<<endl; 
        //cout << "spc: "<<k<<endl; k++;
        m_spc_nod.push_back(ls_nspc);    
      }
    }
  }
}

ls_node & lsdynaReader::getElemNode(const int &e, const int &n){
  return m_node[m_elem[e].node[n]];
}

// $#     nid       dof       vad      lcid        sf       vid     death     birth
bool readBPMNodes() {
  bool ret = true;
  
  
  
  return ret;  
}

// *SET_NODE_LIST_TITLE
// NODESET(SPC) 1
// $#     sid       da1       da2       da3       da4    solver       its         -
         // 1       0.0       0.0       0.0       0.0          1                  
// $#    nid1      nid2      nid3      nid4      nid5      nid6      nid7      nid8
bool lsdynaReader::readSetNodes(){
  bool ret = true;
  auto it = m_command_line.begin();
  
  for (int c=0;c<m_command_line.size()-1;c++){ //*END IS NOT COUNTED
    if (m_command[c].find("*SET_NODE_LIST_TITLE")!=string::npos){
      cout << "Node Set Found, command "<<c<<", line: "<<*it<<endl;
      cout << "End line "<<*(std::next(it))<<endl;
      // READ SET ID
      //FIRST LINE IS TITLE
      cout << "SET ID: "<<readIntField(m_line[*it+2], 0, 10)<<endl;
      
      ls_set_node set;
      set.id = readIntField(m_line[*it+2],0,10);

      int nodecount, ncl; //Total and per line node count
      nodecount = 0;
      for(int i=*it+3;i< *(std::next(it));i++) {
        if (m_line[i].size()>0) {
          int nmax = (int)(m_line[i].size()/10);
          ncl = 0;
          for (int n=0;n<nmax;n++){
            //cout <<"ncl: "<<ncl<<endl;
            //cout << "str "<<m_line[i]<<endl;
            int nodeid = readIntField(m_line[i], 10*n, 10);
            if (nodeid!=0){
              set.node.push_back(m_node_map[nodeid]);
              ncl++;
            }
            //cout << "id" << nodeid<<", pos "<< m_node_map[nodeid]<<endl;
          }
        }//If linesize != 0
        nodecount+=ncl;
      } //Loop Trough lines
      m_set_nod.push_back(set);
      cout << "Node set element count "<<nodecount<<endl;

    }
    it ++;
  }
  
  
  return ret;
  
}

bool lsdynaReader::readContacts(){
  bool ret = true;
  
   for (int c=0;c<m_command.size();c++){
    if (m_command[c].find("ELEMENT_SOLID")!=string::npos){
      readElementSolid();
    }
  }
  
  return ret;
  
}

void lsdynaReader::readCommands(){
  
  for (int i=0;i<m_line.size();i++){
      if (m_line[i].substr(0,1)=="*"){
        m_command_line.insert(i);
        m_command.push_back(m_line[i]);
      }
    }
  cout<< "Command count: "<<m_command.size()<<endl;
}


///// TODO: REMOVE SEARCH EACH COMMAND
lsdynaReader::lsdynaReader(const char *fname){
  m_elem_count_type.resize(10);
  string line;
  m_line_count = 0;
  int start, end;
  char dl = ' ';
  ifstream file(fname);
  if (file.is_open()) {
    while (getline(file, line)) {
      m_line.push_back(line);
      m_line_count++;
    }
  }
        file.close();
  cout << "Line count: "<< m_line_count << endl;
  
  //removeComments();
  removeComments();
  cout << "Reading commands "<<endl;
  readCommands();
  // for (int c=0;c<m_command.size();c++){
    // if ()
  // }
  cout << "Reading nodes "<<endl;
  readNodes();
  cout << "AAAAA"<<endl;
  //Search for parts 
  bool are_pts = true;
  int ini_pos, end_pos;
  int start_pos=0;
  //TODO COMMAND SWIPE AS BELOW WITH PART
  for (int c=0;c<m_command.size();c++){
    if (m_command[c].find("ELEMENT_SOLID")!=string::npos){
      readElementSolid();
    }
  }

  for (int c=0;c<m_command.size();c++){
    if (m_command[c].find("ELEMENT_SPH")!=string::npos){
      readElementSPH();
    }
  }

      // //cout << "part found at"<< ini_pos <<" and "<< end_pos<<endl;
      // findSection ("*", &ini_pos, &end_pos, start_pos); //Or wirh find next command
      // //cout << "next section is"<< ini_pos <<" and "<< end_pos<<endl;
      // //cout << "section name "<< m_line[ini_pos-1]<<endl;
      // if (m_line[ini_pos-1].find("ELEMENT_SOLID") != std::string::npos){
        // cout << "Element Solid found"<<endl;
        // readElementSolid();
      // } else if (m_line[ini_pos-1].find("ELEMENT_SPH") != std::string::npos){
      // // readElementSPH();
      // // readSPCNodes();
      // }
    // }
  // }
  
  readSPCNodes();

  cout << "SPH node id 1 pos w/map: " <<m_node_map[m_elem[0].node[0]]<<endl;
  readSetNodes();
  
  //CONTACT

  //CHECK FOR
// *BOUNDARY_SPC_SET
// $#    nsid       cid      dofx      dofy      dofz     dofrx     dofry     dofrz
         // 1         0         1         1         1         1         1         1
// *SET_NODE_LIST_TITLE
// *BOUNDARY_PRESCRIBED_MOTION_NODE

}


//// NON CLASS /////
void readNodes(char *fName, double **nodes, int *node_count){
  string line;
  int m_line_count = 0;
  std::vector <string> m_line;
  int start, end;
  cout << "Opening:  "<<fName<< " ."<<endl;
  char dl = ' ';
  ifstream file(fName);
  if (file.is_open()) {
    while (getline(file, line)) {
      m_line.push_back(line);
      m_line_count++;
    }
  }
  removeComments(m_line);
  cout << "New length "<<m_line.size()<<endl;
  //findSection(std::vector<string> m_line, string str, int * ini_pos, int *end_pos)
  int length = findSection(m_line, "*NODE", &start, &end);
  cout << "Length: "<< length <<endl;
  cout << "lines: "<<m_line_count<<endl;
  cout << "start: "<<start <<"end"<<end<<endl;
  *node_count = length;
  
  // *nodes = (double *) malloc(3*reader.m_node_count*sizeof(double));
  *nodes = (double *) malloc(3*(*node_count)*sizeof(double));
  // for (int n=0;n<*node_count;n++)
    // for (int d=0;d<3;d++) {
    // //(*nodes)[3*n+d] = reader.m_node[n].m_x[d];
    // //(*nodes)[3*n+d] = 1.;
  // }
  
    for (int i=start;i<end;i++){ //REMOVE COMMENTS DOES NOT WORK IF CALLED BY FORTRAN ....
      int id;
      cout << "reading "<<m_line[i]<<endl;
      if (m_line[i].substr(0,1)!= "$"){
      id = readDoubleField(m_line[i], 0, 8);
      ls_node nod;
      nod.m_id = id;
      for (int d=0;d<3;d++)
        nod.m_x[d] = readDoubleField(m_line[i], 8+16*d, 16);
      }
    }
}

void LSDYNA_getLines(char* fname, char ***lines, double **nodes) {
  string line;
  std::vector <string> m_line;
  int m_line_count = 0;
  int start, end;
  char dl = ' ';
  ifstream file(fname);
  if (file.is_open()) {
    while (getline(file, line)) {
      m_line.push_back(line);
      m_line_count++;
    }
  }
  removeComments(m_line);
  
  cout << "Line size: "<< m_line_count<<endl;
  
  *lines = (char **) malloc(m_line_count * sizeof(char *));
  for (int l=0;l<m_line_count;l++){
    (*lines)[l] = (char *) malloc (100 * sizeof(char));
    //(*lines)[l] = (char *) m_line[l].c_str(); 
    for (int i=0;i<m_line[l].size();i++)
      (*lines)[l][i] = m_line[l].c_str()[i];
  }
  
}

};
