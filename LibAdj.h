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

    std::map<unsigned int, HalfEdgeShort> GenerateHalfEdgeGraphShort(const std::vector<unsigned short> indices, std::vector<unsigned int>& keyList, unsigned int triCount = 0)
    {

        if (triCount == 0) triCount = indices.size() / 3;
        std::map<unsigned int, HalfEdgeShort> edgeMap; // Three edges per triangle
        keyList.clear();
        keyList.resize(triCount * 3);

        // Fill in the edge indices and their keys
        for (size_t face = 0; face < triCount; face++) {
            unsigned short v1 = indices[face * 3 + 0];
            unsigned short v2 = indices[face * 3 + 1];
            unsigned short v3 = indices[face * 3 + 2];

            unsigned int key1 = (unsigned int)v3 | (unsigned int)v1 << 16u;
            unsigned int key2 = (unsigned int)v1 | (unsigned int)v2 << 16u;
            unsigned int key3 = (unsigned int)v2 | (unsigned int)v3 << 16u;

            HalfEdgeShort* edge1 = &edgeMap[key1];
            HalfEdgeShort* edge2 = &edgeMap[key2];
            HalfEdgeShort* edge3 = &edgeMap[key3];

            edge1->index = v1;
            edge1->next = edge2;

            edge2->index = v2;
            edge2->next = edge3;

            edge3->index = v3;
            edge3->next = edge1;

            keyList[face * 3 + 0] = key1;
            keyList[face * 3 + 1] = key2;
            keyList[face * 3 + 2] = key3;
        }

        for (unsigned int key : keyList) {
            unsigned int twinKey = (key << 16u) | (key >> 16u);
            if (edgeMap.find(twinKey) == edgeMap.end()) {
                keyList.clear();
                return {};
            } else {
                edgeMap[key].twin = &edgeMap[twinKey];
                edgeMap[twinKey].twin = &edgeMap[key];
            }
        }

        return edgeMap;
    }

    std::vector<unsigned short> CalculateAdjacencyShort(std::map<unsigned int, HalfEdgeShort>& edgeList, std::vector<unsigned int>& keyList)
    {
        unsigned edgeCount = edgeList.size();
        std::vector<unsigned short> indexBuffer; // Two indices per edge
        indexBuffer.resize(edgeCount * 2);
        unsigned triCount = edgeCount / 3;

        for (unsigned int face = 0; face < triCount; face++) {
            auto& edge1 = edgeList[keyList[face * 3 + 0]];
            auto& edge2 = edgeList[keyList[face * 3 + 1]];
            auto& edge3 = edgeList[keyList[face * 3 + 2]];

            // We start from 3 to preserve index order
            indexBuffer[face * 6 + 0] = edge1.index;
            indexBuffer[face * 6 + 1] = edge2.twin->next->index;
            indexBuffer[face * 6 + 2] = edge2.index;
            indexBuffer[face * 6 + 3] = edge3.twin->next->index;
            indexBuffer[face * 6 + 4] = edge3.index;
            indexBuffer[face * 6 + 5] = edge1.twin->next->index;
        }

        return indexBuffer;
    }

    std::vector<unsigned short> CalculateAdjacencyShort(const std::vector<unsigned short> indexBuffer, unsigned triCount = 0)
    {
        std::vector<unsigned int> keyList;

        auto HalfEdgeGraph = GenerateHalfEdgeGraphShort(indexBuffer, keyList, triCount);
        return CalculateAdjacencyShort(HalfEdgeGraph, keyList);
    }

}