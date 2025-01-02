#include "lsdynaReader.h"
#include <iostream>

using namespace std;
using namespace LS_Dyna;

int main(int argc, char **argv){
  if (argc > 1){
  cout << "Opening "<<argv[1]<<endl;
  lsdynaReader reader(argv[1]);
  cout << "---------------" << endl;
  cout << "Node    count: " << reader.m_node.size() <<endl;
  cout << "Element count: " << reader.m_elem.size() <<endl;
  //cout << "BOUNDARY_SPC_NODE count: " << reader.m_spc_nod.size()<<endl;
  /*
  for(int e=0;e<reader.m_elem.size();e++){
    cout << "Element node count "<<reader.m_elem[e].node.size()<<endl;
    for (int en=0;en<reader.m_elem[e].node.size();en++  ){
      ls_node n = reader.getElemNode(e, en);
      cout << "Node id "<<n.m_id<<", xyz:"<<n.m_x[0]<<", "<<n.m_x[1]<<", "<<n.m_x[2]<<endl;
    }
  }*/
  }
  //TODO: 
  //READ ALL BOUNDARY_SPC_NODE instances
  // *BOUNDARY_SPC_SET
  // $#    nsid       cid      dofx      dofy      dofz     dofrx     dofry     dofrz
           // 1         0         1         1         1         1         1         1
  // *SET_NODE_LIST_TITLE
  // *BOUNDARY_PRESCRIBED_MOTION_NODE
  //BOUNDARY_PRESCRIBED_MOTION_SET
  //*SET_NODE_LIST
}
