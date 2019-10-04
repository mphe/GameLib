# Entity Management

### Background

At first there was no support for hierachies, so a simple std::vector would theoretically suffice to manage entities.
To prevent allocate them using unique_ptr but store them as objects directly inside the vector, a Slotmap was used.
However, after introducing Transformable hierachies, this caused segfaults when the entity vector reallocated and Transformables (that are instantiated inside the Entity class) pointers changed. To circumvent this, the Slotmap uses std::deque internally.

Now as a hierachy should be supported, the old approach is no longer a good idea.

### Requirements

* Supports hierachy
    * Tree graph
    * Deleting a node also deletes all child nodes
    * Nodes have any number of children
* Easy to insert, move, and remove elements
* Memory safety
    * No segfaults after reallocation, etc.

### Possible implementations

#### Database like

Use Slotmap and indices (handles) to store and access entities.
For each entity, store a handle to the parent entity and a list of child entities.

##### Pros
* Cache locality

##### Cons
* Need to pass handles around and dereference them to actually get the entity
* Slotmap needs a deque as underlying data structure so that the Transformable hierachy does not segfault on reallocation
* Difficult to traverse in a tree-like manner


#### Tree Graph

Build a tree structure. Each entity can have a parent and any amount of children.

##### Pros
* Tree structure
* No reallocations
* No need for handles

##### Cons
* No cache locality


### Lifetime management

Entity objects must be allocated and freed somewhere.
Each entity should be stored with a unique_ptr.

#### Option 1
* Store list of unique_ptrs
* Build tree separately and reference other entities using raw pointers

**But**: Removal of one node requires manually descending to all children and removing them too

#### Option 2
* Manage lifetime inside entities
* Entities store unique_ptrs to child entities
* Deallocation of one node automatically removes all underlying children



### Tree implementation

#### Option 1: Linked lists

```c++
struct Node
{
    // data
    Node* parent;
    Node* next;
    Node* children;
};
```

`next` points to the next node on the same level as the current node. `children` points to the first child.
That means, to get a list of all children, for example, one must dereference `children` to get the first child and then follow the linked list using `next`.

##### Pros
* Easy insertion and deletion

##### Cons
* Non-intuitive traversal
* Linked list


#### Option 2

```c++
struct Node
{
    // data
    Node* parent;
    std::vector<Node*> children;
};
```

##### Pros
* Easy traversal
* Straight forward structure

##### Cons
* More complicated to remove elements
* Possibly slightly higher memory usage than linked list approach when the node has only a few children



### Solution
* Tree
* Nodes are managed using unique_ptr
* Nodes manage lifetime of child nodes
* Each node stores a pointer to parent
* Each node stores a vector of children
