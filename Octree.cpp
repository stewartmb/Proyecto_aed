#include "Octree.h"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

// Constructor de Octree
Octree::Octree(Point minBoundary, Point maxBoundary, int maxPointsPerNode, int maxTreeDepth)
        : root(new OctreeNode(minBoundary, maxBoundary, 0)), maxPoints(maxPointsPerNode), maxDepth(maxTreeDepth) {}

// Obtener el octante de un punto
int Octree::getOctant(OctreeNode* node, const Point& point) const {
    float midX = (node->minBoundary.x + node->maxBoundary.x) / 2.0;
    float midY = (node->minBoundary.y + node->maxBoundary.y) / 2.0;
    float midZ = (node->minBoundary.z + node->maxBoundary.z) / 2.0;

    int octant = 0;
    if (point.x >= midX) octant |= 1;
    if (point.y >= midY) octant |= 2;
    if (point.z >= midZ) octant |= 4;
    return octant;
}

// Subdividir un nodo
void Octree::subdivide(OctreeNode* node) {
    if (node->depth >= maxDepth) return;

    float midX = (node->minBoundary.x + node->maxBoundary.x) / 2.0;
    float midY = (node->minBoundary.y + node->maxBoundary.y) / 2.0;
    float midZ = (node->minBoundary.z + node->maxBoundary.z) / 2.0;

    node->children[0] = new OctreeNode(node->minBoundary, Point(midX, midY, midZ), node->depth + 1);
    node->children[1] = new OctreeNode(Point(midX, node->minBoundary.y, node->minBoundary.z),
                                       Point(node->maxBoundary.x, midY, midZ), node->depth + 1);
    node->children[2] = new OctreeNode(Point(node->minBoundary.x, midY, node->minBoundary.z),
                                       Point(midX, node->maxBoundary.y, midZ), node->depth + 1);
    node->children[3] = new OctreeNode(Point(midX, midY, node->minBoundary.z),
                                       Point(node->maxBoundary.x, node->maxBoundary.y, midZ), node->depth + 1);
    node->children[4] = new OctreeNode(Point(node->minBoundary.x, node->minBoundary.y, midZ),
                                       Point(midX, midY, node->maxBoundary.z), node->depth + 1);
    node->children[5] = new OctreeNode(Point(midX, node->minBoundary.y, midZ),
                                       Point(node->maxBoundary.x, midY, node->maxBoundary.z), node->depth + 1);
    node->children[6] = new OctreeNode(Point(node->minBoundary.x, midY, midZ),
                                       Point(midX, node->maxBoundary.y, node->maxBoundary.z), node->depth + 1);
    node->children[7] = new OctreeNode(Point(midX, midY, midZ), node->maxBoundary, node->depth + 1);

    for (const auto& point : node->points) {
        int octant = getOctant(node, point);
        node->children[octant]->points.push_back(point);
    }
    node->points.clear();
}

// Insertar un punto
void Octree::insert(OctreeNode* node, const Point& point) {
    if (!node->isLeaf()) {
        int octant = getOctant(node, point);
        insert(node->children[octant], point);
    } else {
        node->points.push_back(point);
        if (node->points.size() > maxPoints) {
            subdivide(node);
        }
    }
}

void Octree::insert(const Point& point) {
    insert(root, point);
}

// Renderizar el Octree
void Octree::render(OctreeNode* node, unsigned int VAO, unsigned int shaderProgram, const glm::mat4& projection, const glm::mat4& view) {
    if (!node) return;

    glm::vec3 center = glm::vec3(
            (node->minBoundary.x + node->maxBoundary.x) / 2.0f,
            (node->minBoundary.y + node->maxBoundary.y) / 2.0f,
            (node->minBoundary.z + node->maxBoundary.z) / 2.0f
    );

    glm::vec3 scale = glm::vec3(
            node->maxBoundary.x - node->minBoundary.x,
            node->maxBoundary.y - node->minBoundary.y,
            node->maxBoundary.z - node->minBoundary.z
    );

    glm::mat4 model = glm::scale(glm::translate(glm::mat4(1.0f), center), scale);

    glUseProgram(shaderProgram);
    int modelLoc = glGetUniformLocation(shaderProgram, "model");
    int viewLoc = glGetUniformLocation(shaderProgram, "view");
    int projLoc = glGetUniformLocation(shaderProgram, "projection");

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    glBindVertexArray(VAO);
    glPolygonMode(GL_FRONT_AND_BACK, node->points.empty() ? GL_LINE : GL_FILL);
    glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT, 0);

    for (int i = 0; i < 8; ++i) {
        render(node->children[i], VAO, shaderProgram, projection, view);
    }
}
