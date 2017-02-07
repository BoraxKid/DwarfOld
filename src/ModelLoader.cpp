#include "ModelLoader.h"

namespace Dwarf
{
    ModelLoader::ModelLoader(std::shared_ptr<MaterialManager> materialManager, std::vector<ModelData> &modelDatas)
        : _materialManager(materialManager), _modelDatas(modelDatas)
    {
    }

    ModelLoader::~ModelLoader()
    {
    }

    size_t ModelLoader::loadModel(const std::string &fileName)
    {
        const aiScene *scene = this->_importer.ReadFile(fileName, aiProcess_FlipWindingOrder | aiProcess_Triangulate | aiProcess_PreTransformVertices | aiProcess_CalcTangentSpace | aiProcess_GenSmoothNormals);
        if (scene)
        {
            this->loadMesh(scene, fileName);
            LOG(INFO) << "File \"" << fileName << "\" has " << scene->mNumMeshes << " meshes";
            return (this->_modelDatas.size());
        }
        else
            LOG(ERROR) << "Model \"" << fileName << "\": " << this->_importer.GetErrorString();
        return (0);
    }

    void ModelLoader::loadMesh(const aiScene *scene, const std::string &meshName)
    {
        uint32_t i = 0;
        uint32_t j;
        uint32_t verticesNum = 0;
        std::vector<MeshData> meshDatas(scene->mNumMeshes);
        const aiMesh *mesh;
        std::unordered_map<Vertex, size_t> uniqueVertices;
        std::vector<size_t> materialIDs(scene->mNumMaterials);

        while (i < scene->mNumMaterials)
        {
            materialIDs[i] = this->_materialManager->createMaterial(scene->mMaterials[i]);
            ++i;
        }
        i = 0;
        this->_modelDatas.push_back(ModelData());
        while (i < scene->mNumMeshes)
        {
            mesh = scene->mMeshes[i];
            meshDatas.at(i).materialID = materialIDs.at(mesh->mMaterialIndex);
            verticesNum += mesh->mNumVertices;
            j = 0;
            aiVector3D zero3D(0.0f, 0.0f, 0.0f);
            aiVector3D *pos;
            aiVector3D *normal;
            aiVector3D *textureCoord;
            Vertex vertex;
            uniqueVertices.clear();
            while (j < mesh->mNumVertices)
            {
                pos = &(mesh->mVertices[j]);
                normal = &(mesh->mNormals[j]);
                if (mesh->HasTextureCoords(0))
                {
                    textureCoord = &(mesh->mTextureCoords[0][j]);
                    vertex = Vertex(glm::vec3(pos->x, pos->y, pos->z), glm::vec3(normal->x, normal->y, normal->z), glm::vec2(textureCoord->x, textureCoord->y));
                }
                else
                    vertex = Vertex(glm::vec3(pos->x, pos->y, pos->z), glm::vec3(normal->x, normal->y, normal->z));
                if (uniqueVertices.count(vertex) == 0)
                {
                    uniqueVertices[vertex] = meshDatas.at(i).vertices.size();
                    meshDatas.at(i).vertices.push_back(vertex);
                }
                ++j;
            }
            j = 0;
            while (j < mesh->mNumFaces)
            {
                const aiFace &face = mesh->mFaces[j];
                if (face.mNumIndices != 3)
                {
                    ++j;
                    continue;
                }
                meshDatas.at(i).indices.push_back(face.mIndices[0]);
                meshDatas.at(i).indices.push_back(face.mIndices[1]);
                meshDatas.at(i).indices.push_back(face.mIndices[2]);
                ++j;
            }
            LOG(INFO) << "assimp: vertices number(" << meshDatas.at(i).vertices.size() << ") with indices number (" << meshDatas.at(i).indices.size() << ")";
            this->_modelDatas.back().meshDatas.clear();
            this->_modelDatas.back().meshDatas.insert(this->_modelDatas.back().meshDatas.begin(), meshDatas.begin(), meshDatas.end());
            ++i;
        }
    }
}
