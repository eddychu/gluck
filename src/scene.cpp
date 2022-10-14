// #include "scene.h"

// int addNode(Scene &scene, int parent, int level)
// {
//     int node = (int)scene.hierarchy_.size();
//     {
//         // TODO: resize aux arrays (local/global etc.)
//         scene.localTransform_.push_back(glm::mat4(1.0f));
//         scene.globalTransform_.push_back(glm::mat4(1.0f));
//     }
//     scene.hierarchy_.push_back({.parent_ = parent, .lastSibling_ = -1});
//     if (parent > -1)
//     {
//         // find first item (sibling)
//         int s = scene.hierarchy_[parent].firstChild_;
//         if (s == -1)
//         {
//             scene.hierarchy_[parent].firstChild_ = node;
//             scene.hierarchy_[node].lastSibling_ = node;
//         }
//         else
//         {
//             int dest = scene.hierarchy_[s].lastSibling_;
//             if (dest <= -1)
//             {
//                 // no cached lastSibling, iterate nextSibling indices
//                 for (dest = s; scene.hierarchy_[dest].nextSibling_ != -1; dest = scene.hierarchy_[dest].nextSibling_)
//                     ;
//             }
//             scene.hierarchy_[dest].nextSibling_ = node;
//             scene.hierarchy_[s].lastSibling_ = node;
//         }
//     }
//     scene.hierarchy_[node].level_ = level;
//     scene.hierarchy_[node].nextSibling_ = -1;
//     scene.hierarchy_[node].firstChild_ = -1;
//     return node;
// }

// void markAsChanged(Scene &scene, int node)
// {
//     int level = scene.hierarchy_[node].level_;
//     scene.changedAtThisFrame_[level].push_back(node);

//     for (int s = scene.hierarchy_[node].firstChild_; s != -1; s = scene.hierarchy_[s].nextSibling_)
//     {
//         markAsChanged(scene, s);
//     }
// }
// int findNodeByName(const Scene &scene, const std::string &name)
// {
//     // Extremely simple linear search without any hierarchy reference
//     // To support DFS/BFS searches separate traversal routines are needed

//     for (size_t i = 0; i < scene.localTransform_.size(); i++)
//         if (scene.nameForNode_.contains(i))
//         {
//             int strID = scene.nameForNode_.at(i);
//             if (strID > -1)
//                 if (scene.names_[strID] == name)
//                     return (int)i;
//         }

//     return -1;
// }

// int getNodeLevel(const Scene &scene, int n)
// {
//     int level = -1;
//     for (int p = 0; p != -1; p = scene.hierarchy_[p].parent_, level++)
//         ;
//     return level;
// }

// void recalculateGlobalTransforms(Scene &scene)
// {
//     if (!scene.changedAtThisFrame_[0].empty())
//     {
//         int c = scene.changedAtThisFrame_[0][0];
//         scene.globalTransform_[c] = scene.localTransform_[c];
//         scene.changedAtThisFrame_[0].clear();
//     }

//     for (int i = 1; i < MAX_NODE_LEVEL && (!scene.changedAtThisFrame_[i].empty()); i++)
//     {
//         for (const int &c : scene.changedAtThisFrame_[i])
//         {
//             int p = scene.hierarchy_[c].parent_;
//             scene.globalTransform_[c] = scene.globalTransform_[p] * scene.localTransform_[c];
//         }
//         scene.changedAtThisFrame_[i].clear();
//     }
// }