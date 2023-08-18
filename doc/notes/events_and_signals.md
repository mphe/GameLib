# Events and Signals

### Why Handles?

TODO

## Implementation

### EventManager
* array of callbacks

TODO

### Signal

Signals are implemented as linked list. Calling `connect` on a Signal returns a handle. If the handle goes out of scope or `disconnect` is called, it will unregister automatically. It is therefore necessary to store a handle as long as the handler is needed.
These handles build a double linked list, where each handle points to the next and previous handle. Unregistering or moving a handle will automatically redirect the corresponding linked list pointers, as expected.
Note that handles are not instantiated using new, but the user has full control over where and how to store them.
When the signal is triggered, it will run through the LL chain and call each callback.
All calling relevant information is stored in handles. null callbacks will be skipped.
The Signal itself does therefore not store any handles except an empty (no callback) handle acting as start node and pointing to the first real handle.
Thus, Signals are very lightweight.

The advantage of this linked list approach over the array approach in EventManager is memory safety and simplicity.
First, it has all advantages of LLs, such as insertion/deletion without invalidating pointers/iterators.
This is especially relevant as during Signal callback execution, a handler should be able to unregister without breaking the event callback loop.
It is also much more memory safe in certain edge cases.
For example, a component A defines a signal S and another component B connects to S.
Eventually A gets removed and S does no longer exist. B is not notified about this, so trying to access S, e.g. for unregistering, will result in a segfault.
This does not happen here because unregistering a handle does not require to perform actions on the original Signal object, as the handler can be removed from the chain by simply redirecting pointers.
That means even if the original Signal does no longer exist, all operations on handles are still safe.

