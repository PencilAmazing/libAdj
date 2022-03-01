// LibAdj.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#include <iostream>
#include <vector>
#include <map>

namespace libAdj {
    typedef struct HalfEdgeShort {
        unsigned short index;
        HalfEdgeShort* twin = nullptr;
        HalfEdgeShort* next = nullptr;
    } HalfEdgeShort;

    typedef struct HalfEdgeLarge {
        unsigned int index;
        HalfEdgeLarge* twin = nullptr;
        HalfEdgeLarge* next = nullptr;
    } HalfEdgeLarge;

    std::map<unsigned short, HalfEdgeShort*> GenerateHalfEdgeGraphShort(const std::vector<unsigned short> indices, unsigned int triCount = 0)
    {
        if (triCount == 0) triCount = indices.size() / 3;
        std::map<unsigned short, HalfEdgeShort*> edgeMap; // Three edges per triangle
        std::vector<HalfEdgeShort> edgeList;
        edgeList.reserve(triCount * 3);

        // Fill in the edge indices and their keys
        for (size_t face = 0; face < triCount; face++) {
            unsigned short v1 = indices[face * 3 + 0];
            unsigned short v2 = indices[face * 3 + 1];
            unsigned short v3 = indices[face * 3 + 2];

            HalfEdgeShort* edge1 = &edgeList[face * 3 + 0];
            HalfEdgeShort* edge2 = &edgeList[face * 3 + 1];
            HalfEdgeShort* edge3 = &edgeList[face * 3 + 2];

            edge1->index = v1;
            edge1->next = edge2;

            edge2->index = v2;
            edge2->next = edge3;

            edge3->index = v3;
            edge3->next = edge1;

            edgeMap[(unsigned int)v1 | (unsigned int)v2 << 16u] = edge1;
            edgeMap[(unsigned int)v2 | (unsigned int)v3 << 16u] = edge2;
            edgeMap[(unsigned int)v3 | (unsigned int)v1 << 16u] = edge3;
        }

        // Read the edges
        for (std::pair<unsigned int, HalfEdgeShort*> edge : edgeMap) {
            // If twin was already filled in then skip
            if (edge.second->twin)
                continue;

            unsigned int key = edge.first;
            unsigned int twinKey = (key << 16u) | (key >> 16u);

            // If twin does not exist, then mesh has holes and is invalid
            if (edgeMap.find(twinKey) == edgeMap.end()) {
                // Return empty set
                return {};
            } else {
                // Fill in twin references
                HalfEdgeShort* twinEdge = edgeMap[twinKey];
                twinEdge->twin = edge.second;
                edge.second->twin = twinEdge;
            }
        }
        return edgeMap;
    }

    std::vector<unsigned short> CalculateAdjacencyShort(const std::map<unsigned short, HalfEdgeShort*> edgeList)
    {
        unsigned edgeCount = edgeList.size();
        std::vector<unsigned short> indexBuffer(edgeCount * 2); // Two indices per edge

        unsigned triCount = edgeCount / 3;

        // Assuming the edges here are ordered, every consecutive three is a triangle
        // Push the triangle vert then the adjacent vert
        for (auto edge = edgeList.begin(); edge != edgeList.end(); edge++) {
            auto& edge1 = edge->second;
            auto& edge2 = (edge++)->second;
            auto& edge3 = (edge++)->second;

            // We start from 3 to preserve index order
            indexBuffer.push_back(edge3->index);
            indexBuffer.push_back(edge3->twin->next->index);

            indexBuffer.push_back(edge1->index);
            indexBuffer.push_back(edge1->twin->next->index);

            indexBuffer.push_back(edge2->index);
            indexBuffer.push_back(edge2->twin->next->index);
        }

        return indexBuffer;
    }

    std::vector<unsigned short> CalculateAdjacencyShort(const std::vector<unsigned short> indexBuffer, unsigned triCount = 0)
    {
        auto HalfEdgeGraph = GenerateHalfEdgeGraphShort(indexBuffer, triCount);
        return CalculateAdjacencyShort(HalfEdgeGraph);
    }

}