#include "../../include/Renderers/Mesh.h"
#include "../../include/Renderers/Texture.h"
#include "../../include/Renderers/Shader.h"
#include "../../include/Renderers/Renderer.h"
#include "../../include/Renderers/VertexArray.h"
#include "../../include/Renderers/Collision.h"

#include "document.h"

namespace
{
    union Vectex
    {
        float f;
        uint8_t b[4];
    };
}

Mesh::Mesh(/* args */)
{
    this->mBox = new AABB(Vector3::Infinity, Vector3::NegInfinity);
    this->mRadius = 0.0f;
    this->mVertexArray = nullptr;
    this->mSpecPower = 100.0f;
}

Mesh::~Mesh()
{
    if (this->mBox)
    {
        delete mBox;
        mBox = nullptr;
    }
}

VertexArray* Mesh::getVertexArray()
{
    return this->mVertexArray;
}

Texture* Mesh::getTexture(size_t index)
{
    Texture* tex = nullptr;
    // SDL_Log("%d", this->mTextures.size());
    size_t vecSize = (size_t)(this->mTextures.size());
    if (index < vecSize)
    {
        tex = this->mTextures[index];
    }
    
    return tex;
}

const std::string& Mesh::getShaderName() const
{
    return this->mShaderName;
}

float Mesh::getRadius() const
{
    return this->mRadius;
}

const std::string& Mesh::getFileName()
{
    return this->mFileName;
}

bool Mesh::load(const std::string& fileName, Renderer* renderer)
{
    this->mFileName = fileName;

    std::ifstream file(fileName);
    if (!file.is_open())
    {
        SDL_Log("File not found: Mesh: %s", fileName.c_str());
        return false;
    }

    std::stringstream fileStream;
    fileStream << file.rdbuf();
    std::string contents = fileStream.str();

    rapidjson::StringStream jsonStr(contents.c_str());
    rapidjson::Document doc;
    doc.ParseStream(jsonStr);

    if (!doc.IsObject())
    {
        SDL_Log("1.Mesh %s is not valid json", fileName.c_str());
        return false;
    }

    int version = doc["version"].GetInt();
    if (version != 1)
    {
        SDL_Log("2.Mesh %s not version 1", fileName.c_str());
        return false;
    }

    this->mShaderName = doc["shader"].GetString();


    VertexArray::Layout layout = VertexArray::PosNormTex;
    size_t vertSize = 8;

    // 获取顶点格式
    std::string vertexFormat = doc["vertexformat"].GetString();
    if (vertexFormat == "PosNormSkinTex")
    {
        layout = VertexArray::PosNormSkinTex;
        vertSize = 10;
    }

    SDL_Log("[Mesh] Vertex size: %d", vertSize);
    
    // 加载纹理
    const rapidjson::Value& textures = doc["textures"];
    if (!textures.IsArray() || textures.Size() < 1)
    {
        SDL_Log("3.Mesh %s has no textures...", fileName.c_str());
        return false;
    }

    this->mSpecPower = (float)(doc["specularPower"].GetDouble());
    
    for (rapidjson::SizeType i = 0; i < textures.Size(); i++)
    {
        std::string texName = textures[i].GetString();
        Texture* tex = renderer->getTexture(texName.c_str());
        if (tex == nullptr)
        {
            tex = renderer->getTexture(texName.c_str());
            if (tex == nullptr)
            {
                tex = renderer->getTexture("../../Assets/Images/Default.png");
            }
        }

        if (tex != nullptr)
        {
            this->mTextures.emplace_back(tex);
        }
        else
        {
            SDL_Log("4. Texture is null: %d", this->mTextures.size());
            return false;
        }
    }

    // 加载顶点
    const rapidjson::Value& vertsJson = doc["vertices"];
    if (!vertsJson.IsArray() || vertsJson.Size() < 1)
    {
        SDL_Log("Mesh %s has no vertices", fileName.c_str());
        return false;
    }

    // std::vector<float> vertices;
    std::vector<Vectex> vertices;
    vertices.reserve(vertsJson.Size() * vertSize);

    this->mRadius = 0.0f;
    for (rapidjson::SizeType i = 0; i < vertsJson.Size(); i++)
    {
        const rapidjson::Value& vert = vertsJson[i];
        if (!vert.IsArray() || (vert.Size() != 8 && vert.Size() != 16))
        {
            SDL_Log("Unexpected vertex format for %s", fileName.c_str());
            return false;
        }

        Vector3 pos(vert[0].GetDouble(), vert[1].GetDouble(), vert[2].GetDouble());
        this->mRadius = Math::Max(this->mRadius, pos.LengthSq());
        
        // 更新计算网格对象空间的包围框
        this->mBox->updateMinMax(pos);

        if (layout == VertexArray::PosNormTex)
        {
            Vectex vertex;
            // 将顶点添加到容器存储
            for (rapidjson::SizeType j = 0; j < vert.Size(); j++)
            {
                vertex.f = (float)(vert[j].GetDouble());
                vertices.emplace_back(vertex);
            }
        }
        else
        {
            Vectex vertex;
            // 顶点
            for (rapidjson::SizeType j = 0; j < 6; ++j)
            {
                vertex.f = (float)(vert[j].GetDouble());
                vertices.emplace_back(vertex);
            }

            // 影响顶点的骨骼以及权重
            for (rapidjson::SizeType j = 6; j < 14; j += 4)
            {
                vertex.b[0] = vert[j].GetUint();
                vertex.b[1] = vert[j+1].GetUint();
                vertex.b[2] = vert[j+2].GetUint();
                vertex.b[3] = vert[j+3].GetUint();
                vertices.emplace_back(vertex);
            }

            // 纹理坐标
            for (rapidjson::SizeType j = 14; j < vert.Size(); ++j)
            {
                vertex.f = float(vert[j].GetDouble());
                vertices.emplace_back(vertex);
            }
        }
    }
    this->mRadius = Math::Sqrt(this->mRadius);

    // 加载索引
    const rapidjson::Value& indJson = doc["indices"];
	if (!indJson.IsArray() || indJson.Size() < 1)
	{
		SDL_Log("Mesh %s has no indices", fileName.c_str());
		return false;
	}

	std::vector<unsigned int> indices;
	indices.reserve(indJson.Size() * 3);
	for (rapidjson::SizeType i = 0; i < indJson.Size(); i++)
	{
		const rapidjson::Value& ind = indJson[i];
		if (!ind.IsArray() || ind.Size() != 3)
		{
			SDL_Log("Invalid indices for %s", fileName.c_str());
			return false;
		}

        for (rapidjson::SizeType j = 0; j < ind.Size(); j++)
        {
		    indices.emplace_back(ind[j].GetUint());
        }
	}

    SDL_Log("Vertices size: %d verSize: %d", vertices.size(), vertSize);
    this->mVertexArray = new VertexArray(layout, vertices.data(), (uint32_t)(vertices.size() / vertSize),
        indices.data(), (uint32_t)(indices.size()));
    return true;    
}

void Mesh::unLoad()
{
    if (this->mVertexArray)
    {
        delete this->mVertexArray;
        this->mVertexArray = nullptr;
    }
}   

float Mesh::getSpecPower()
{
    return this->mSpecPower;
}

const AABB* Mesh::getBox()
{
    return this->mBox;
}