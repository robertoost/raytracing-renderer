#pragma once

namespace RaytracingRenderer {

    struct FaceDefinition {
        int vertexIndexes[3];
        int textureIndexes[3];
        int normalIndexes[3];
    };
    class Obj
    {
    public:
        int faceCount;
        int normalCount;
        int vertexCount;
        int indexCount;
        vector<Vertex> vertices; 
        vector<int> vert_indices;
        vector<int> norm_indices;

        vector<Triangle> mesh_to_triangle() {
            vector<Triangle> vec;
            for (int i = 0; i < indexCount; i+=3) {
                vec.push_back(Triangle(vertices[vert_indices[i]], vertices[vert_indices[i + 1]], vertices[vert_indices[i + 2]]));
            }
            return vec;
        }

        Obj load_model(string file) {
            ifstream loaderStream(file);

            Obj modelData = Obj();

            vector<float3> vertexPositions;
            vector<float3> vertexNormals;
            vector<float3> vertexTextureCoords;
            vector<int> setVertices;
            vector<FaceDefinition> faceDefinitions;
            vector<string> tokens;
            vector<string> faceToken;
            int vertexCount = 0, normalCount = 0, polygonCount = 0;
            vector<int> indices;
            vector<int> normals;
            int selectedVertex;

            string input;
            string currentToken;
            string currentFaceToken;
            istringstream streamReader;
            istringstream faceReader;
            float3 tempVector;
            FaceDefinition tempFace;

            while (std::getline(loaderStream, input))
            {
                streamReader.clear();
                streamReader.str(input);
                tokens.clear();
                while (getline(streamReader, currentToken, ' '))
                {
                    tokens.push_back(currentToken);
                }
                if (tokens.size() < 1)
                    continue;

                if (tokens[0] == "v")
                {
                    vertexCount++;
                    if (tokens.size() >= 4)
                    {
                        tempVector = float3(0, 0, 0);
                        tempVector.x = (float)atof(tokens[1].c_str());
                        tempVector.y = (float)atof(tokens[2].c_str());
                        tempVector.z = (float)atof(tokens[3].c_str());
                        vertexPositions.push_back(tempVector);
                        continue;
                    }
                    else
                    {
                        vertexPositions.push_back(float3(0, 0, 0)); //give it a bad vertex, because we received a bad vertex
                    }
                }
                if (tokens[0] == "vn")
                {
                    normalCount++;
                    if (tokens.size() >= 4)
                    {
                        tempVector = float3(0, 0, 0);
                        tempVector.x = (float)atof(tokens[1].c_str());
                        tempVector.y = (float)atof(tokens[2].c_str());
                        tempVector.z = (float)atof(tokens[3].c_str());
                        vertexNormals.push_back(tempVector);
                        continue;
                    }
                    else
                    {
                        vertexNormals.push_back(float3(0, 0, 0)); //give it a bad normal, because we received a bad normal
                    }
                }
                if (tokens[0] == "f")
                {
                    polygonCount++;
                    if (tokens.size() == 4) //sorry, but non triangles aren't worth the time
                    {
                        tempFace = FaceDefinition();

                        //vertex 1
                        faceReader.clear();
                        faceReader.str(tokens[1]);
                        faceToken.clear();
                        while (getline(faceReader, currentFaceToken, '/'))
                        {
                            faceToken.push_back(currentFaceToken);
                        }
                        if (faceToken.size() == 3) //should have all the info, or, same as above, its not worth the time
                        {
                            tempFace.vertexIndexes[0] = atoi(faceToken[0].c_str()) - 1;
                            tempFace.textureIndexes[0] = atoi(faceToken[1].c_str()) - 1;
                            tempFace.normalIndexes[0] = atoi(faceToken[2].c_str()) - 1;
                        }

                        //vertex 2
                        faceReader.clear();
                        faceReader.str(tokens[2]);
                        faceToken.clear();
                        while (std::getline(faceReader, currentFaceToken, '/'))
                        {
                            faceToken.push_back(currentFaceToken);
                        }
                        if (faceToken.size() == 3) //should have all the info, or, same as above, its not worth the time
                        {
                            tempFace.vertexIndexes[1] = atoi(faceToken[0].c_str()) - 1;
                            tempFace.textureIndexes[1] = atoi(faceToken[1].c_str()) - 1;
                            tempFace.normalIndexes[1] = atoi(faceToken[2].c_str()) - 1;
                        }

                        //vertex 3
                        faceReader.clear();
                        faceReader.str(tokens[3]);
                        faceToken.clear();
                        while (std::getline(faceReader, currentFaceToken, '/'))
                        {
                            faceToken.push_back(currentFaceToken);
                        }
                        if (faceToken.size() == 3) //should have all the info, or, same as above, its not worth the time
                        {
                            tempFace.vertexIndexes[2] = atoi(faceToken[0].c_str()) - 1;
                            tempFace.textureIndexes[2] = atoi(faceToken[1].c_str()) - 1;
                            tempFace.normalIndexes[2] = atoi(faceToken[2].c_str()) - 1;
                        }

                        faceDefinitions.push_back(tempFace);
                    }
                    //if we receive a bad face, we just wont read it
                }
            }

            indices = vector<int>(vertexCount);

            modelData.faceCount = polygonCount;
            modelData.normalCount = normalCount;
            modelData.vertexCount = vertexCount;
            modelData.vertices = vector<Vertex>(vertexCount);

            //< ^^everything up here works^^ >
            //=========================================================
            // CONVERT THE DATA INTO SOMETHING USABLE
            //=========================================================

            //now is the boring part, creating the vertex list...

            indices.clear();
            for (int i = 0; i < polygonCount; i++)
            {
                for (int i2 = 0; i2 < 3; i2++)
                {
                    indices.push_back(faceDefinitions[i].vertexIndexes[i2]);
                    normals.push_back(faceDefinitions[i].normalIndexes[i2]);
                }

                modelData.vert_indices = vector<int>(indices.size());
                modelData.norm_indices = vector<int>(normals.size());
                modelData.indexCount = indices.size();

                for (int i = 0; i < vertexCount; i++)
                {
                    modelData.vertices[i].position = vertexPositions[i];
                    modelData.vertices[i].normal = vertexNormals[i];
                }

                for (int i = 0; i < indices.size(); i++)
                {
                    modelData.vert_indices[i] = indices[i];
                }

                for (int i = 0; i < normals.size(); i++) {
                    modelData.norm_indices[i] = normals[i];
                }
            }

            return modelData;
        }
    };    
}
