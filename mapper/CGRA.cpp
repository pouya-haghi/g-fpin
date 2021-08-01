/*
 * ======================================================================
 * CGRA.cpp
 * ======================================================================
 * CGRA implementation.
 *
 * Author : Cheng Tan
 *   Date : July 16, 2019
 */

#include "CGRA.h"

CGRA::CGRA(int t_rows, int t_columns, bool t_heterogeneity) {
  m_rows = t_rows;
  m_columns = t_columns;
  m_FUCount = t_rows * t_columns;
  nodes = new CGRANode**[t_rows];
  // Initialize the unidirection connections.
  m_LinkCount = 2 * (t_rows * (t_columns-1) + (t_rows-1) * t_columns);
  links = new CGRALink*[m_LinkCount];

  // Initialize the CGRA nodes.
  int node_id = 0;
  for(int i=0; i<t_rows; ++i) {
    nodes[i] = new CGRANode*[t_columns];
    for (int j=0; j<t_columns; ++j) {
      nodes[i][j] = new CGRANode(node_id++, j, i);
    }
  }

  // Enable the specialized 'call' functionality.
  for (int r=0; r<t_rows; ++r) {
    if (r%2 == 0)
      nodes[r][t_columns-1]->enableCall();
  }

  // Enable the heterogeneity.
  if (t_heterogeneity) {
    for (int r=0; r<t_rows; ++r) {
      for (int c=0; c<t_columns; ++c) {
        if(r%2==1 and c%2 == 1)
          nodes[r][c]->enableComplex();
      }
    }
  }

  // Enable the load/store on specific CGRA nodes.
  for (int r=0; r<t_rows; ++r) {
    nodes[r][0]->enableLoad();
    nodes[r][0]->enableStore();
  }

//  for (int i=0; i<t_rows; ++i) {
//    for (int j=0; j<t_columns; ++j) {
//      nodes[i][j]->enableLoad();
//      nodes[i][j]->enableStore();
//  //  nodes[t_rows-1][j]->enableStore();
//    }
//  }

  // Connect the CGRA nodes with links.
  int link_id = 0;
  for (int i=0; i<t_rows; ++i) {
    for (int j=0; j<t_columns; ++j) {
      if (i < t_rows - 1) {
        links[link_id] = new CGRALink(link_id);
        nodes[i][j]->attachOutLink(links[link_id]);
        nodes[i+1][j]->attachInLink(links[link_id]);
        links[link_id]->connect(nodes[i][j], nodes[i+1][j]);
        ++link_id;
      }
      if (i > 0) {
        links[link_id] = new CGRALink(link_id);
        nodes[i][j]->attachOutLink(links[link_id]);
        nodes[i-1][j]->attachInLink(links[link_id]);
        links[link_id]->connect(nodes[i][j], nodes[i-1][j]);
        ++link_id;
      }
      if (j < t_columns - 1) {
        links[link_id] = new CGRALink(link_id);
        nodes[i][j]->attachOutLink(links[link_id]);
        nodes[i][j+1]->attachInLink(links[link_id]);
        links[link_id]->connect(nodes[i][j], nodes[i][j+1]);
        ++link_id;
      }
      if (j > 0) {
        links[link_id] = new CGRALink(link_id);
        nodes[i][j]->attachOutLink(links[link_id]);
        nodes[i][j-1]->attachInLink(links[link_id]);
        links[link_id]->connect(nodes[i][j], nodes[i][j-1]);
        ++link_id;
      }
    }
  }
  disable();
}

void CGRA::disable() {
//  nodes[0][0]->disable();
//  nodes[0][1]->disable();
//  nodes[0][2]->disable();
//  nodes[0][3]->disable();
//  nodes[0][4]->disable();
//  nodes[1][0]->disable();
//  nodes[1][1]->disable();
//  nodes[1][2]->disable();
//  nodes[1][4]->disable();
//  nodes[1][3]->disable();
//  nodes[2][0]->disable();
//  nodes[2][1]->disable();
//  nodes[2][2]->disable();
//  nodes[2][3]->disable();
//  nodes[2][4]->disable();
//  nodes[3][1]->disable();
//  nodes[3][2]->disable();
//  nodes[3][2]->disable();
//  nodes[3][3]->disable();
//  nodes[3][4]->disable();
//  nodes[4][1]->disable();
//  nodes[4][2]->disable();
//  nodes[4][3]->disable();
//  nodes[4][4]->disable();

}

void CGRA::setRegConstraint(int t_regConstraint) {
  for (int i=0; i<m_rows; ++i)
    for (int j=0; j<m_columns; ++j)
      nodes[i][j]->setRegConstraint(t_regConstraint);
}

void CGRA::setBypassConstraint(int t_bypassConstraint) {
  for (int i=0; i<m_LinkCount; ++i)
    links[i]->setBypassConstraint(t_bypassConstraint);
}

void CGRA::setCtrlMemConstraint(int t_ctrlMemConstraint) {
  for (int i=0; i<m_rows; ++i)
    for (int j=0; j<m_columns; ++j)
      nodes[i][j]->setCtrlMemConstraint(t_ctrlMemConstraint);

  for (int i=0; i<m_LinkCount; ++i)
    links[i]->setCtrlMemConstraint(t_ctrlMemConstraint);
}

int CGRA::getFUCount() {
  return m_FUCount;
}

void CGRA::constructMRRG(int t_II) {
  for (int i=0; i<m_rows; ++i)
    for (int j=0; j<m_columns; ++j)
      nodes[i][j]->constructMRRG(m_FUCount, t_II);
  for (int i=0; i<m_LinkCount; ++i)
    links[i]->constructMRRG(m_FUCount, t_II);
}

CGRALink* CGRA::getLink(CGRANode* t_n1, CGRANode* t_n2) {
   for (int i=0; i<m_LinkCount; ++i) {
     if (links[i]->getSrc()==t_n1 and links[i]->getDst() == t_n2) {
       return links[i];
     }
  }
  cout << "bad quiry for CGRA link\n";
  return NULL;
//  assert(0);
}

