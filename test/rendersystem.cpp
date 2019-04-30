#include <iostream>
#include <cassert>
#include <vector>
#include <random>
#include <ctime>

#define private public
#include "gamelib/core/rendering/RenderSystem.hpp"

using namespace std;
using namespace gamelib;


constexpr int max_size = 20;

void checkQueue(RenderSystem& rendersystem)
{
    rendersystem._updateDirty();
    rendersystem._updateQueue();

    int lastdepth = __INT_MAX__;
    for (NodeHandle h : rendersystem._renderqueue)
    {
        const SceneNode* node = rendersystem.getNode(h);
        assert(node && "Invalid node in render queue");
        assert(node->depth <= lastdepth && "Wrong node order");
        lastdepth = node->depth;

        sf::Transform matrix = node->transform;
        assert(matrix == *rendersystem.getNodeGlobalMatrix(h) && "Wrong matrix");
    }
}

int main()
{
    auto seed = time(0);
    srand(seed);
    cout<<"seed: "<<seed<<endl;

    RenderSystem rendersystem;
    std::vector<NodeHandle> handles;
    Component* owner = reinterpret_cast<Component*>(0xDEADBEEF);

    for (size_t n = 0; n < 500; ++n)
    {
        NodeHandle handle = rendersystem.createNode(owner);
        handles.push_back(handle);
        const SceneNode& node = *rendersystem.getNode(handle);

        if (rand() % 2 == 0)
        {
            int depth = rand() - rand();
            rendersystem.setNodeDepth(handle, depth);
            assert(node.depth == depth && "Wrong depth");
        }

        assert(node.owner == owner && "Wrong owner");

        for (size_t k = 0; k < 5; ++k)
        {
            size_t size = 1 + rand() % max_size;
            rendersystem.createNodeMesh(handle, size, sf::Points);

            assert(node.mesh.size == size && "Wrong size");
            assert(node.mesh.handle.size == size && "Wrong size");

            size_t offset = size == 1 ? 0 : rand() % (size - 1);
            size_t usesize = rand() % (size - offset);
            size_t stop = offset + usesize;
            std::vector<sf::Vertex> vertices;

            for (size_t i = 0; i < usesize; ++i)
                vertices.push_back(sf::Vertex(sf::Vector2f(rand(), rand())));

            rendersystem.updateNodeMesh(handle, vertices.data(), usesize, offset);

            assert(node.mesh.size == stop && "Wrong size");
            assert(node.mesh.handle.size == size && "Wrong size");

            for (size_t i = offset; i < stop; ++i)
                assert(rendersystem.getNodeMesh(handle, i)->position == vertices[i - offset].position && "Wrong vertex");
        }

        checkQueue(rendersystem);
    }

    for (size_t n = 0; n < 100; ++n)
    {
        size_t index = rand() % handles.size();
        NodeHandle handle = handles[index];
        rendersystem.setNodeDepth(handle, rand() - rand());
        sf::Transform matrix(rand(), rand(), rand(), rand(), rand(), rand(), rand(), rand(), rand());
        rendersystem.setNodeTransform(handle, matrix);
    }

    checkQueue(rendersystem);

    for (size_t n = 0; n < 50; ++n)
    {
        for (size_t i = 0; i < 1 + rand() % 10; ++i)
        {
            size_t index = rand() % handles.size();
            rendersystem.removeNode(handles[index]);
            handles.erase(handles.begin() + index);
        }
        checkQueue(rendersystem);
    }

    return 0;
}
