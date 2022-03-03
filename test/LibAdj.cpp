// LibAdj.cpp : Defines the entry point for the application.
//

#include "LibAdj.h"

#include <array>

using namespace std;

// https://en.wikibooks.org/wiki/OpenGL_Programming/Modern_OpenGL_Tutorial_05
const std::vector<unsigned short> indexBuffer = {
    // front
    0, 1, 2,
    2, 3, 0,
    // right
    1, 5, 6,
    6, 2, 1,
    // back
    7, 6, 5,
    5, 4, 7,
    // left
    4, 0, 3,
    3, 7, 4,
    // bottom
    4, 5, 1,
    1, 0, 4,
    // top
    3, 2, 6,
    6, 7, 3
};

unsigned short GetAdjacentIndex(const std::vector<unsigned short>& indexBuffer, unsigned short from, unsigned short to, unsigned short other)
{
    for (unsigned face = 0; face < indexBuffer.size() / 3; face++) {
        unsigned short a = indexBuffer[face * 3 + 0];
        unsigned short b = indexBuffer[face * 3 + 1];
        unsigned short c = indexBuffer[face * 3 + 2];

        if (a == other || b == other || c == other) continue; // Skip target face

        if (!(a == from || b == from || c == from)) continue;
        if (!(a == to || b == to || c == to)) continue;

        return (a != from && a != to ? a : (b != from && b != to ? b : c));
    }

    return 0;
}

std::vector<unsigned short> GenerateTestCase(const std::vector<unsigned short>& indexBuffer)
{
    unsigned triCount = indexBuffer.size() / 3;

    std::vector<unsigned short> output;
    output.resize(indexBuffer.size() * 2);

    for (unsigned face = 0; face < triCount; face++) {
        unsigned short a = indexBuffer[face * 3 + 0];
        unsigned short b = indexBuffer[face * 3 + 1];
        unsigned short c = indexBuffer[face * 3 + 2];

        output[face * 6 + 0] = a;
        output[face * 6 + 1] = GetAdjacentIndex(indexBuffer, a, b, c);
        output[face * 6 + 2] = b;
        output[face * 6 + 3] = GetAdjacentIndex(indexBuffer, b, c, a);
        output[face * 6 + 4] = c;
        output[face * 6 + 5] = GetAdjacentIndex(indexBuffer, c, a, b);

    }

    return output;
}

int main()
{
    const std::vector<unsigned short> adjacentBuffer = libAdj::CalculateAdjacencyShort(indexBuffer);
    cout << "Test case:" << endl;
    for (auto index : adjacentBuffer)
        cout << index << ", ";
    cout << endl;

    std::vector<unsigned short> idealBuffer = GenerateTestCase(indexBuffer);
    cout << "Target result:" << endl;
    for (auto index : idealBuffer)
        cout << index << ", ";
    cout << endl;

    if (adjacentBuffer == idealBuffer) {
        cout << "Test case matches target" << endl;
        return 0;
    } else {
        cout << "Test case does not match target" << endl;
        return -1;
    }

}
