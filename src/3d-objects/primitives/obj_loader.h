#pragma once

namespace RaytracingRenderer {

    struct Obj
    {
        int faceCount;
        int normalCount;
        int vertexCount;
        int indexCount;
        vector<Vertex> vertices;
        vector<float3> verts;
        vector<FaceDefinition> faces;
    };
    class Mesh : public Hittable
    {
    public:

        vector<Triangle> faces;

        Mesh(vector<Triangle> faces) {
            this->faces = faces;
            updateAABB(this->bounding_box);
        }
        Mesh() {}

        void updateAABB(AABB box) const override {
            for (Triangle tri : faces) {
                tri.updateAABB(box);
            }
            cout << box.p_min << " " << box.p_max;
        }

        bool intersect(const Ray& ray, float t_min, float t_max, hit_record& rec) const override {
            for (Triangle tri : faces) {
                if (tri.bounding_box.intersectBounds(ray, t_min, t_max)) {
                    return true;
                }
            }
            return false;
        }

        void computeBounds(const float3& planeNormal, float& dnear, float& dfar) const override {
            float d;
            vector<float3> p;
            for (Triangle tri : faces) {
                tri.computeBounds(planeNormal, dnear, dfar);
            }
        }
        vector<Triangle> fix_winding(vector<Triangle> tris) {
            int i = 0;
            for (Triangle ttris : tris) {
                float3 a0 = ttris.v0.position;
                float3 a1 = ttris.v1.position;
                float3 a2 = ttris.v2.position;
                for (int k = 0; k < tris.size(); k++) {
                    if (k != i) {
                        float3 b0 = tris[k].v0.position;
                        float3 b1 = tris[k].v1.position;
                        float3 b2 = tris[k].v2.position;
                        if (a0 == b0 && a1 == b1) {
                            tris[k].v0.position = b1;
                            tris[k].v1.position = b0;
                            cout << "Fixed winding " << k;
                        }
                        else if (a1 == b1 && a2 == b2) {
                            tris[k].v1.position = b2;
                            tris[k].v2.position = b1;
                            cout << "Fixed winding " << k;
                        }
                        else if (a0 == b0 && a2 == b2) {
                            tris[k].v0.position = b2;
                            tris[k].v2.position = b0;
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
            //fix_winding(vec);
            return vec;
        }

        Mesh load_model(string file) {
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
                    //cout << "TOKENS: " << tokens.size();
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

            //< ^^everything up here works^^ >
            //=========================================================
            // CONVERT THE DATA INTO SOMETHING USABLE
            //=========================================================

            //now is the boring part, creating the vertex list...
            //cout << normalCount << " " << vertexNormals.size() << vertexCount;
            indices.clear();
            vector<Triangle> triangles;
            for (int i = 0; i < polygonCount; i++)
            {
                Triangle tri = Triangle(float3(vertexPositions[modelData.faces[i].vertexIndexes[0]]), float3(vertexPositions[modelData.faces[i].vertexIndexes[2]]), float3(vertexPositions[modelData.faces[i].vertexIndexes[1]]));
                triangles.push_back(tri);
            }
            Mesh mesh = Mesh(triangles);
            return mesh;
        }
    };    
}
