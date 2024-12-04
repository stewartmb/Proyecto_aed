#ifndef OCTREE_H
#define OCTREE_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>

// Estructura de punto 3D
struct Point {
    float x, y, z;
    Point(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
};

// Clase OctreeNode (nodo del Octree)
class OctreeNode {
public:
    Point minBoundary, maxBoundary;
    std::vector<Point> points;
    OctreeNode* children[8] = {nullptr};
    int depth;

    OctreeNode(Point minB, Point maxB, int d) : minBoundary(minB), maxBoundary(maxB), depth(d) {}

    bool isLeaf() const {
        for (int i = 0; i < 8; i++) {
            if (children[i] != nullptr) return false;
        }
        return true;
    }
};

// Clase Octree
class Octree {
private:
    OctreeNode* root;
    int maxPoints;  // Número máximo de puntos antes de subdividir
    int maxDepth;   // Profundidad máxima del Octree

    int getOctant(OctreeNode* node, const Point& point) const;
    void subdivide(OctreeNode* node);
    void insert(OctreeNode* node, const Point& point);

public:
    Octree(Point minBoundary, Point maxBoundary, int maxPointsPerNode, int maxTreeDepth);
    void insert(const Point& point);
    OctreeNode* getRoot() const { return root; }

    // Función para renderizar el Octree
    void render(OctreeNode* node, unsigned int VAO, unsigned int shaderProgram, const glm::mat4& projection, const glm::mat4& view);
};

#endif // OCTREE_H
