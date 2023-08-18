# Prefab System

## Requirements

* Nested prefabs
* Delta saving
* Instantiation
    * Allows property changes to all (sub-)entities and components
    * Saves deltas aswell

## Would-be-cool Requirements
* Loading must not break if prefab changes (hierachy, components, ...)


## Not supported

* Changing hierachy in instantiation
* Adding/Removing components in instantiation


## Solution

### Serialization
The hierachy based serialization does not work for this problem, as it could break delta loading when the prefab changes.
E.g. Prefab changes hierachy, then applying deltas saved from old prefab version -> deltas get merged to wrong entities.

New save format:
```
{
    "nextid": <id>,
    "entities": {
        <id1>: {
            <entity>,
            "parent": <idX>
        },
        <id2>: {
            <prefab>,
            "parent": <idX>
        },
        <idN>: { ... },
    }
}
```

This "flat" structure allows for easily delta-ing and merging any configs, independent of hierachy and future changes in hierachy.
However, it requires to maintain an all-time unique ID for each entity per prefab. That means entities can have the same ID if they are from different prefabs, however, inside one prefab, all entities must have unique IDs. Even if an entity gets removed and new ones get added, none of those future entities must ever have the same ID as the removed one, otherwise, delta merging could break due to ambiguity. To guarantee no IDs are being reused, the next valid ID must be saved along the prefab template (`nextid`).

To define the hierachy in the config each entity node can have a `parent` member mapped to the ID of the parent entity.
The hierachy must not be constructed using an actual tree hierachy like before, because it would break delta-ing.

Nested prefabs can be handled recursively. A prefab node contains data structured in the same manner as here, so it can be processed by a simple recursive function call.


### Changes
* `prefabID` member in `Entity`
* Prefab resource/template
    * json template
    * ID counter
