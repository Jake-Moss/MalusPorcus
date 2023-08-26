#include "helper.h"
#include "stdio.h"


int pointInPhysicsBody(Vector2 testPoint, PhysicsBody body, int nvert) {
    float tx = testPoint.x;
    float ty = testPoint.y;

    int i, j, c = 0;
    for (i = 0, j = nvert-1; i < nvert; j = i++) {
        Vector2 vertexA = GetPhysicsShapeVertex(body, i);
        Vector2 vertexB = GetPhysicsShapeVertex(body, j);

        if ( ((vertexA.y > ty) != (vertexB.y > ty)) && (tx < (vertexB.x - vertexA.x) * (ty - vertexA.y) / (vertexB.y - vertexA.y) + vertexA.x) ) {
            c = !c;
        }
    }
    return c;
}