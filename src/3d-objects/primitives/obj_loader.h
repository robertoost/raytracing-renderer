#pragma once

namespace RaytracingRenderer {

    class Obj
    {
    public:
        int faceCount;
        int normalCount;
        int vertexCount;
        int indexCount;
        vector<Vertex> vertices; 
        vector<float3> verts;
        vector<FaceDefinition> faces;


        //Converts the list of vertexes into triangles, using the list of indices (840 indices, 439 verts, 840 faces. vertices[index_of_verts])

        vector<Triangle> loadOBJ(const char* path) {
            FILE* file = fopen(path, "r");
            vector<float3> vertices;
            vector<float3> normals;
            vector<float3> vertexIndices;
            vector<float3> normalIndices;
            int faceCount = 0;
            while (1) {
                char lineHeader[128];
                // read the first word of the line
                int res = fscanf(file, "%s", lineHeader);
                if (res == EOF)
                    break; // EOF = End Of File. Quit the loop.

            // else : parse lineHeader

                if (strcmp(lineHeader, "v") == 0) {
                    float3 vertex;
                    fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
                    vertices.push_back(vertex);
                }
                else if (strcmp(lineHeader, "vn") == 0) {
                    float3 normal;
                    fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
                    normals.push_back(normal);
                }
                else if (strcmp(lineHeader, "f") == 0) {
                    float3 vertexIndex;
                    float3 normalIndex;
                    float3 textureIndex;
                    string buffer;
                    fscanf(file, "%f/%f/%f %f/%f/%f %f/%f/%f\n", &vertexIndex.x, &textureIndex.x, &normalIndex.x, &vertexIndex.y, &textureIndex.y, &normalIndex.y, &vertexIndex.z, &textureIndex.z, &normalIndex.z);
                    faceCount++;
                    vertexIndex -= 1;
                    vertexIndices.push_back(vertexIndex);
                    normalIndex -= 1;
                    normalIndices.push_back(normalIndex);
                }
                else {
                    // Probably a comment, eat up the rest of the line
                    char stupidBuffer[1000];
                    fgets(stupidBuffer, 1000, file);
                }
            }
            vector<Triangle> tris;
            for (unsigned int i = 0; i < faceCount; i++)
            {
                //float3 p0 = float3(vertices[vertexIndices[i].x].x, vertices[vertexIndices[i].y].y, vertices[vertexIndices[i].z].z);
                //float3 p1 = float3(vertices[vertexIndices[i].y].x, vertices[vertexIndices[i].y].y, vertices[vertexIndices[i].y].z);
                //float3 p2 = float3(vertices[vertexIndices[i].z].x, vertices[vertexIndices[i].z].y, vertices[vertexIndices[i].z].z);
                float3 avNormal = (normals[normalIndices[i].x] + normals[normalIndices[i].y] + normals[normalIndices[i].z]) / length(normals[normalIndices[i].x] + normals[normalIndices[i].y] + normals[normalIndices[i].z]);
                Triangle tri = Triangle(vertices[vertexIndices[i].x], vertices[vertexIndices[i].y], vertices[vertexIndices[i].z]);
                tris.push_back(tri);
            }
            return tris;
        }

        vector<Triangle> fix_winding(vector<Triangle> tris) {
            int i = 0;
            for (Triangle ttris : tris) {
                float3 a0 = ttris.v0;
                float3 a1 = ttris.v1;
                float3 a2 = ttris.v2;
                for (int k = 0; k < tris.size(); k++) {
                    if (k != i) {
                        float3 b0 = tris[k].v0;
                        float3 b1 = tris[k].v1;
                        float3 b2 = tris[k].v2;
                        if (a0 == b0 && a1 == b1) {
                            tris[k].v0 = b1;
                            tris[k].v1 = b0;
                            cout << "Fixed winding " << k;
                        }
                        else if (a1 == b1 && a2 == b2) {
                            tris[k].v1 = b2;
                            tris[k].v2 = b1;
                            cout << "Fixed winding " << k;
                        }
                        else if (a0 == b0 && a2 == b2) {
                            tris[k].v0 = b2;
                            tris[k].v2 = b0;
                            cout << "Fixed winding " << k;
                        }
                    }
                }
                i++;
            }
            return tris;
        }


        vector<Triangle> mesh_to_triangle() {
            vector<Triangle> vec;
            for (int i = 0; i < faces.size(); i++) {
                vec.push_back(Triangle(faces[i]));
            }
            fix_winding(vec);
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
                        cout << "Bad vertex";
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
                        cout << "Bad normal";
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

                        modelData.faces.push_back(tempFace);
                    }
                    else
                        cout << "Bad face ";
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
            cout << normalCount << " " << vertexNormals.size() << vertexCount;
            indices.clear();
            for (int i = 0; i < polygonCount; i++)
            {
                modelData.faces[i].verts[0] = float3(vertexPositions[modelData.faces[i].vertexIndexes[0]]);
                modelData.faces[i].verts[1] = float3(vertexPositions[modelData.faces[i].vertexIndexes[1]]);
                modelData.faces[i].verts[2] = float3(vertexPositions[modelData.faces[i].vertexIndexes[2]]);

                modelData.faces[i].normals[0] = float3(vertexNormals[modelData.faces[i].normalIndexes[0]]);
                modelData.faces[i].normals[1] = float3(vertexNormals[modelData.faces[i].normalIndexes[1]]);
                modelData.faces[i].normals[2] = float3(vertexNormals[modelData.faces[i].normalIndexes[2]]);
            }

            return modelData;
        }
    };    
}
