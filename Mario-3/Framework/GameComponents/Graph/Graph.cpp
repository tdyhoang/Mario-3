#include "Graph.h"

std::vector<CNode*> CGraph::GetListNodes()
{
    return listNodes;
}

CNode* CGraph::GetNodeByID(int id)
{
    return listNodes.at(id);
}

void CGraph::AddNode(CNode* node)
{
    listNodes.push_back(node);
}