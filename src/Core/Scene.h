#pragma once
#include <string>
using namespace std;
#include <Core/Model.h>
#include <Common/uuid.h>
#include <Core/Light.h>
#include <glm/glm.hpp>
using namespace glm;

// #include <assimp/scene.h>
// #include <assimp/postprocess.h>
// #include <assimp/cimport.h>
// #include <assimp/version.h>

//  struct Vertex
//  {
//      glm::vec3 pos;
//      glm::vec3 normal;
//      glm::vec2 uv0;
//      //glm::vec2 uv1;
//      //glm::vec4 joint0;
//      //glm::vec4 weight0;
//      //glm::vec4 color;
//  };

//   struct Mesh
//   {
//       vector<Vertex> vertices;
//       vector<uint32_t> indices;
//   };

// struct SceneNode
//  {

//     SceneNode() : id(uuid::generate_uuid_v4()) {

//     }
//     int index;
//     int parent = -1;
//     vector<int> children;
//     vector<int> meshes;
//     mat4 transform;
//     string id;
// };

//  class Scene
//  {
//  public:
//      vector<SceneNode> nodes;
//      vector<Mesh> meshes;

//      void load(const string &filename)
//      {
//          const aiScene *scene = aiImportFile(filename.c_str(), aiProcess_Triangulate);

//          if (!scene || !scene->HasMeshes())
//          {
//              printf("Unable to load %s\n", filename);
//              exit(255);
//          }

//          process_node(scene->mRootNode, scene);

//         for (int i = 0; i < scene->mNumMaterials; i++) {
//             process_material(scene->mMaterials[i]);
//         }

//          aiReleaseImport(scene);
//      }

//      void process_material(aiMaterial* material) {
//          auto count = material->mNumProperties;
//          aiString material_name;
//          material->Get(AI_MATKEY_NAME, material_name);
//          printf("material name %s\n", material_name.C_Str());
//          int numtextures = material->GetTextureCount(aiTextureType_DIFFUSE);
//          printf("diffuse count %d\n", numtextures);
//          numtextures = material->GetTextureCount(aiTextureType_EMISSIVE);
//          printf("emissive count %d\n", numtextures);
//          numtextures = material->GetTextureCount(aiTextureType_NORMALS);
//          printf("normal count %d\n", numtextures);
//          numtextures = material->GetTextureCount(aiTextureType_METALNESS);
//          printf("metalness count %d\n", numtextures);
//          numtextures = material->GetTextureCount(aiTextureType_DIFFUSE_ROUGHNESS);
//          printf("roughness count %d\n", numtextures);
//          printf("count %d\n", count);
//      }

//      static mat4 convert_matrix(const aiMatrix4x4 &aiMat)
//      {
//          return {
//              aiMat.a1, aiMat.b1, aiMat.c1, aiMat.d1,
//              aiMat.a2, aiMat.b2, aiMat.c2, aiMat.d2,
//              aiMat.a3, aiMat.b3, aiMat.c3, aiMat.d3,
//              aiMat.a4, aiMat.b4, aiMat.c4, aiMat.d4};
//      }

//      void process_node(aiNode *node, const aiScene *scene, int parent = -1)
//      {
//          nodes.push_back(SceneNode());
//          int current_node_index = nodes.size() - 1;
//          auto &current_node = nodes[current_node_index];
//          current_node.index = current_node_index;
//          if (parent >= 0)
//          {
//              current_node.parent = parent;
//          }
//          current_node.transform = Scene::convert_matrix(node->mTransformation);

//          int starting_mesh_index = meshes.size();
//          // process each mesh located at the current node
//          for (unsigned int i = 0; i < node->mNumMeshes; i++)
//          {
//              // the node object only contains indices to index the actual objects in the scene.
//              // the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
//              aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
//              process_mesh(mesh, scene);
//              current_node.meshes.push_back(starting_mesh_index + i);
//          }

//          int starting_children_index = nodes.size();
//          // after we've processed all of the meshes (if any) we then recursively process each of the children nodes
//          for (unsigned int i = 0; i < node->mNumChildren; i++)
//          {
//              process_node(node->mChildren[i], scene, current_node_index);
//              current_node.children.push_back(starting_children_index + i);
//          }
//      }

//      void process_mesh(aiMesh *mesh, const aiScene *scene)
//      {
//          std::vector<Vertex> vertices;
//          std::vector<uint32_t> indices;
//          {

//              for (unsigned i = 0; i != mesh->mNumVertices; i++)
//              {
//                  const aiVector3D v = mesh->mVertices[i];
//                  const aiVector3D n = mesh->mNormals[i];
//                  const aiVector3D t = mesh->mTextureCoords[0][i];
//                  Vertex vertex = {.pos = vec3(v.x, v.y, v.z), .normal = vec3(n.x, n.y, n.z), .uv0 = vec2(t.x, 1.0f - t.y)};
//                  vertices.push_back(vertex);
//              }
//              for (unsigned i = 0; i != mesh->mNumFaces; i++)
//              {
//                  for (unsigned j = 0; j != 3; j++)
//                      indices.push_back(mesh->mFaces[i].mIndices[j]);
//              }
//          }
//          // Mesh mesh = {.vertices = vertices, .indices = indices};
//          Mesh m = {.vertices = vertices,
//                    .indices = indices};
//          meshes.push_back(m);
//      }
//  };

class Scene
{
public:
    Scene() : id(uuid::generate_uuid_v4())
    {
    }

    Model model;
    Light light;
    std::string environment;
    std::string environmentIrradiance;
    std::string brdfLut;
    std::string id;
};