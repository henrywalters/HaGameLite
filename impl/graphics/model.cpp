//
// Created by henry on 9/14/24.
//
#include "../../../include/hagame/graphics/model.h"
#include "../../include/hagame/graphics/color.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

using namespace hg::graphics;

Model::Model(std::string path) {
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path.c_str(), aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices | aiProcess_FixInfacingNormals);

    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << "\n";
        return;
    }

    auto processMesh = [&](aiMesh* mesh, const aiScene* scene) -> Mesh {
        Mesh out;
        for (int i = 0; i < mesh->mNumVertices; i++) {
            Vertex vert;
            vert.position = Vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
            vert.normal = Vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
            if (mesh->mTextureCoords[0]) {
                vert.texCoords = Vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
            }
            out.vertices.push_back(vert);
        }

        for (int i = 0; i < mesh->mNumFaces; i++) {
            aiFace face = mesh->mFaces[i];
            for (int j = 0; j < face.mNumIndices; j++) {
                out.indices.push_back(face.mIndices[j]);
            }
        }

        return out;
    };

    std::function<graphics::Color(std::string, aiMaterial*, graphics::Color)> getColor = [&](std::string key, aiMaterial* mat, graphics::Color defaultColor = graphics::Color::black()) {
        aiColor4D rawColor;
        if (mat->Get(key.c_str(), 0, 0, rawColor) == AI_SUCCESS) {
            return Color(rawColor.r, rawColor.g, rawColor.b, rawColor.a);
        } else {
            std::cout << "Missing material: " << key << "\n";
            return defaultColor;
        }
    };

    std::function<float(std::string, aiMaterial*, float)> getFloat = [&](std::string key, aiMaterial* mat, float defaultValue) {
        float value = defaultValue;
        if (mat->Get(key.c_str(), 0, 0, value) == AI_SUCCESS) {
        } else {
            std::cout << "Missing material: " << key << "\n";
        }
        return value;
    };

    std::function<void(ModelNode*, aiNode*, const aiScene*)> processNode = [&](ModelNode* modelNode, aiNode* node, const aiScene* scene) {
        for (int i = 0; i < node->mNumMeshes; i++) {
            auto mesh = scene->mMeshes[node->mMeshes[i]];
            modelNode->meshes.push_back(processMesh(mesh, scene));
            modelNode->instances.push_back(MeshInstance(&modelNode->meshes.back()));

            Material material;
            auto aiMat = scene->mMaterials[mesh->mMaterialIndex];
            material.diffuse = getColor("$clr.diffuse", aiMat, graphics::Color::black());
            material.albedo = getColor("$clr.ambient", aiMat, graphics::Color::black());
            material.emissive = getColor("$clr.emissive", aiMat, graphics::Color::black());
            material.specular = getColor("$clr.specular", aiMat, graphics::Color::black());
            material.shininess = getFloat("$mat.shininess", aiMat, 1.0f);

            modelNode->materials.push_back(material);
        }

        for (int i = 0; i < node->mNumChildren; i++) {
            m_nodes.push_back(std::make_unique<ModelNode>());
            modelNode->addChild(m_nodes.back().get());
            processNode(m_nodes.back().get(), node->mChildren[i], scene);
        }
    };

    m_nodes.push_back(std::make_unique<ModelNode>());

    processNode(m_nodes.back().get(), scene->mRootNode, scene);
}

void Model::render() {
    structures::Tree::DepthFirstTraverse(root(), [](const auto& node){
        auto modelNode = static_cast<ModelNode*>(node);
        for (int i = 0; i < modelNode->instances.size(); i++) {
            modelNode->instances[i].render();
            std::cout << modelNode->materials[i].shininess << "\n";
        }
        return true;
    });
}
