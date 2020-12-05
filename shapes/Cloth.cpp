#include "Cloth.h"
#include "gl/datatype/VAO.h"
#include "gl/datatype/VBO.h"
#include "gl/datatype/VBOAttribMarker.h"
#include "gl/shaders/ShaderAttribLocations.h"
#include "gl/GLDebug.h"
#include "glm/ext.hpp"
#include "glm/gtx/string_cast.hpp"

using namespace CS123::GL;

Cloth::Cloth(float resolution, int particleWidth, int particleHeight):
    m_resolution(resolution),
    L_structural(4.0/(resolution - 1)),
    L_shear(sqrt(2.0) * 4.0/(resolution - 1)),
    L_flexion(2.0 * (4.0/(resolution - 1)))
{
    particles.clear();

    numXPoints = resolution;
    numYPoints = resolution;

    float particleMass = 5.0;
    float incrementX = (1.0f/(float)numXPoints);
    float incrementY = (1.0f/(float)numYPoints);

    /*
        X ---> -0.5 to 0.5
      Y
      |
      |
      \/

      */

    for(int j = 0; j < numXPoints; j++) {
        for(int i = 0; i< numYPoints; i++) {
            float x = (1.f/numXPoints) * i - 0.5f;
            float y = 0.5f - ((1.f/numYPoints) * j);

            ClothParticle clothParticle = ClothParticle(
                                            glm::vec3(x,y,0),
                                            particleMass,
                                            glm::vec2(x, y)
                                          );
            particles.push_back(clothParticle);

            std::cout<<x<<std::endl;
            std::cout<<y<<std::endl;
        }
    }

      particles[0].isStatic= true;
      particles[numXPoints*(numYPoints-1)].isStatic= true;

      buildShape(0, 0, 0);
}

void Cloth::buildShape(int param1, int param2, int param3)
{
    m_vertices.clear();
    m_normals.clear();
    m_vertexData.clear();
    m_uv.clear();



    for ( int i = 0; i < numXPoints; ++i )
    {
        for ( int j = 0; j < numYPoints; ++j )
        {
            glm::vec3 pos = particles[i*numYPoints+j].m_pos;
            m_vertices.push_back(pos);
            m_normals.push_back(Normal(0,0,-1.0));
            m_uv.push_back(glm::vec2(0));
        }
    }



    buildFace( param1,  param2,  param3);
    buildVAO();


}

void Cloth::buildFace(int param1,int param2, int param3)
{
    for(int i = 0 ; i < numXPoints - 1; i++)
    {
        for(int j = 0 ; j < numYPoints - 1; j++)
        {
           int firstPoint = i + j * numXPoints;
           int secondPoint =(i + j * numXPoints )+ 1;
           int thirdPoint = (i + j * numXPoints )+ 1+ numXPoints;


           // here I am creating the triangles and sending them as vertex data
           Vertex v1 = m_vertices[firstPoint];
           Vertex v2 = m_vertices[secondPoint];
           Vertex v3 = m_vertices[thirdPoint];

           Vertex n1 = m_normals[firstPoint ];
           Vertex n2 = m_normals[secondPoint];
           Vertex n3 = m_normals[thirdPoint];

           //getTextCoordinates(); ??

           TextureCoord t1 = m_uv[firstPoint];
           TextureCoord t2 = m_uv[secondPoint];
           TextureCoord t3 = m_uv[thirdPoint];

           m_vertexData.push_back(v1.x);
           m_vertexData.push_back(v1.y);
           m_vertexData.push_back(v1.z);
          /* m_vertexData.push_back(n1.x);
           m_vertexData.push_back(n1.y);
           m_vertexData.push_back(n1.z);
           m_vertexData.push_back(t1.x);
           m_vertexData.push_back(t1.y)*/;

           m_vertexData.push_back(v2.x);
           m_vertexData.push_back(v2.y);
           m_vertexData.push_back(v2.z);
//           m_vertexData.push_back(n2.x);
//           m_vertexData.push_back(n2.y);
//           m_vertexData.push_back(n2.z);
//           m_vertexData.push_back(t2.x);
//           m_vertexData.push_back(t2.y);

           m_vertexData.push_back(v3.x);
           m_vertexData.push_back(v3.y);
           m_vertexData.push_back(v3.z);
//           m_vertexData.push_back(n3.x);
//           m_vertexData.push_back(n3.y);
//           m_vertexData.push_back(n3.z);
//           m_vertexData.push_back(t3.x);
//           m_vertexData.push_back(t3.y);

           int fourth =(i + j * numXPoints) + numXPoints;

           Vertex v4 = m_vertices[fourth];
           Vertex n4 = m_normals[fourth];
           TextureCoord t4 = m_uv[fourth];

           m_vertexData.push_back(v1.x);
           m_vertexData.push_back(v1.y);
           m_vertexData.push_back(v1.z);
//           m_vertexData.push_back(n1.x);
//           m_vertexData.push_back(n1.y);
//           m_vertexData.push_back(n1.z);
//           m_vertexData.push_back(t1.x);
//           m_vertexData.push_back(t1.y);

           m_vertexData.push_back(v3.x);
           m_vertexData.push_back(v3.y);
           m_vertexData.push_back(v3.z);
//           m_vertexData.push_back(n3.x);
//           m_vertexData.push_back(n3.y);
//           m_vertexData.push_back(n3.z);
//           m_vertexData.push_back(t3.x);
//           m_vertexData.push_back(t3.y);

           m_vertexData.push_back(v4.x);
           m_vertexData.push_back(v4.y);
           m_vertexData.push_back(v4.z);
//           m_vertexData.push_back(n4.x);
//           m_vertexData.push_back(n4.y);
//           m_vertexData.push_back(n4.z);
//           m_vertexData.push_back(t4.x);
//           m_vertexData.push_back(t4.y);

        }
    }
}

int Cloth::getParticleIndexFromCoordinates(int i, int j) {
    return i * numYPoints + j;
}

bool Cloth::isValidCoordinate(int i, int j) {
    return (i < m_resolution && i >= 0) &&(j < m_resolution && j >= 0);
}

glm::vec3 Cloth::getSpringForce(glm::vec3 p, glm::vec3 q, Cloth::SpringForceType forceType) {
    float stiffness; //K in the spring equation
    float restLength; //L in the spring equation
    switch(forceType) {
        case Cloth::SpringForceType::STRUCTURAL:
            stiffness = K_structural;
            restLength = L_structural;
        case Cloth::SpringForceType::SHEAR:
            stiffness = K_shear;
            restLength = L_shear;
        case Cloth::SpringForceType::FLEXION:
            stiffness = K_flexion;
            restLength = L_flexion;
    }
    return stiffness * (restLength - glm::length(p - q)) * glm::normalize(p - q);
}

glm::vec3 Cloth::getStructuralSpringForce(int i, int j) {
    glm::vec3 totalStructuralForce = glm::vec3(0, 0, 0);

    glm::vec3 p = particles[getParticleIndexFromCoordinates(i, j)].m_pos;
    std::cout<<"printing p"<<std::endl;
    std::cout<<glm::to_string(p)<<std::endl;
    std::cout<<"printing m_verticies"<<std::endl;
    std::cout<<glm::to_string(m_vertices[getParticleIndexFromCoordinates(i, j)])<<std::endl;

    std::cout<<"printing qs"<<std::endl;
    if(isValidCoordinate(i - 1, j)) {
        glm::vec3 q = particles[getParticleIndexFromCoordinates(i-1, j)].m_pos;
        std::cout<<glm::to_string(q)<<std::endl;
        totalStructuralForce = totalStructuralForce + getSpringForce(p, q, Cloth::SpringForceType::STRUCTURAL);
    }
    if(isValidCoordinate(i + 1, j)) {
        glm::vec3 q = particles[getParticleIndexFromCoordinates(i + 1, j)].m_pos;
        std::cout<<glm::to_string(q)<<std::endl;
        totalStructuralForce = totalStructuralForce + getSpringForce(p, q, Cloth::SpringForceType::STRUCTURAL);
    }
    if(isValidCoordinate(i, j + 1)) {
        glm::vec3 q = particles[getParticleIndexFromCoordinates(i, j + 1)].m_pos;
        std::cout<<glm::to_string(q)<<std::endl;
        totalStructuralForce = totalStructuralForce + getSpringForce(p, q, Cloth::SpringForceType::STRUCTURAL);
    }
    if(isValidCoordinate(i, j - 1)) {
        glm::vec3 q = particles[getParticleIndexFromCoordinates(i, j - 1)].m_pos;
        std::cout<<glm::to_string(q)<<std::endl;
        totalStructuralForce = totalStructuralForce + getSpringForce(p, q, Cloth::SpringForceType::STRUCTURAL);
    }

    return totalStructuralForce;
}

void Cloth::update(float deltaTime)
{
    for ( int i = 0; i < numXPoints; ++i )
    {
        for ( int j = 0; j < numYPoints; ++j )
        {

            int index = getParticleIndexFromCoordinates(i, j);

            glm::vec3 f_gravity = glm::vec3(0, -9.8, 0) * particles[index].m_mass;
            glm::vec3 f_damping = particles[index].m_velocity * -Cd;
            glm::vec3 f_viscous = Cv * glm::dot(particles[index].m_normal, Ufluid - particles[index].m_velocity) * particles[index].m_normal;
            glm::vec3 f_spring = getStructuralSpringForce(i, j);// + getShearSpringForce(i, j) + getFlexionSpringForce(i, j);

            particles[index].m_force += f_gravity + f_damping + f_viscous + f_spring;
        }
    }

    m_vertices.clear();
//    m_normals.clear();
//    m_vertexData.clear();
//    m_uv.clear();


// //particles[numYPoints/2+numXPoints/2].force.z -= 1;


    for (size_t i = 0; i < particles.size(); i++)
    {
        particles[i].step(deltaTime);
    }

    m_vertices.clear();
    for ( int i = 0; i < numXPoints; ++i )
    {
        for ( int j = 0; j < numYPoints; ++j )
        {
            glm::vec3 pos = particles[i*numXPoints+j].m_pos;
           // std::cout << pos.x << " ,"<<  pos.y  << " ," << pos.z <<std::endl;
            m_vertices.push_back(pos);
            m_normals.push_back(Normal(0,0,-1.0));
            m_uv.push_back(glm::vec2(0));
        }
    }

    m_vertexData.clear();
    buildFace(0,0,0);

   // updateBuffer();
}

void Cloth::draw()
{
//     glBindVertexArray(m_vaohandle);
//     glDrawArrays(GL_TRIANGLES, 0, numVertices);
//     glBindVertexArray(0);
    updateBuffer();
}

void Cloth::buildVAO()
{
//    const int numFloatsPerVertex = 8;
//    const int numVertices = m_vertexData.size() / numFloatsPerVertex;

//    std::vector<VBOAttribMarker> markers;
//    markers.push_back(VBOAttribMarker(ShaderAttrib::POSITION, 3, 0));
//    markers.push_back(VBOAttribMarker(ShaderAttrib::NORMAL, 3, 3*sizeof(float)));
//    markers.push_back(VBOAttribMarker(ShaderAttrib::TEXCOORD0,2, 6*sizeof(float)));

//    VBO vbo = VBO(m_vertexData.data(), m_vertexData.size(), markers,
//                  VBO::GEOMETRY_LAYOUT::LAYOUT_TRIANGLES, GL_DYNAMIC_DRAW);
//    m_VAO = std::make_unique<VAO>(vbo, numVertices);

//    m_vertexData.clear();
//    m_vertexData ={  -0.5f, -0.5f, 0.0f, // left
//                     0.5f, -0.5f, 0.0f, // right
//                     0.0f,  0.5f, 0.0f  // top
//                  };

    numVertices = m_vertexData.size() / 3;

//    float vertices[] = {
//           -0.5f, -0.5f, 0.0f, // left
//            0.5f, -0.5f, 0.0f, // right
//            0.0f,  0.5f, 0.0f  // top
//       };

    glGenVertexArrays(1, &m_vaohandle);
    glBindVertexArray(m_vaohandle);
    glGenBuffers(1, &m_vbohandle);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbohandle);
    glBufferData(GL_ARRAY_BUFFER, m_vertexData.size()*sizeof (GL_FLOAT),
                 m_vertexData.data(), GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0,3, GL_FLOAT,GL_FALSE, 0 , 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray( 0);

     //vertexPositionBuffer = gl.createBuffer();
}

void Cloth::updateBuffer()
{

    //m_VAO->bind();
    checkError();
    //m_VAO->updateVBO(m_vertexData.data(),m_vertexData.size());
//    m_VAO->getVBO()->updateBuffer(m_vertexData.data(),m_vertexData.size());
    checkError();
    glBindVertexArray(m_vaohandle);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbohandle);
    glBufferData(GL_ARRAY_BUFFER, m_vertexData.size()*sizeof (GL_FLOAT),
                 m_vertexData.data(), GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glBindVertexArray(m_vaohandle);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glDrawArrays(GL_TRIANGLES, 0, numVertices);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glBindVertexArray(0);
     checkError();
    //glVertexAttribPointer(0,3, GL_FLOAT,GL_FALSE, 0 , 0);
    //m_VAO->getVBO()->bindAndEnable();
    //m_VAO->getVBO()->unbind();
    //m_VAO->unbind();
}
