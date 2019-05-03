# Properties

#### Idea
* Consider moving read/write functions to IPropType
* Makes separation between IPropType and PropertyHandle obsolete
* Make whole property system OOP based
* Probably cleaner
* Performance negligible

This effectively removes the data oriented approach and uses OOP instead.

#### Pros
* Seems cleaner
* More straightforward usage
  * No field reuse (e.g. hints being used differently depending on property type)
* Easy read/write overloading
* Easier to extend
  * More attributes
  * Overloading

#### Cons
* More development overhead to derive the class and overwrite getter/setter than to simply pass a lambda
* Inheritance problems


#### Conclusions
OOP approach is probably cleaner as it removes the separation between PropertyHandle and IPropType, and is more straightforward and self-explanatory, as fields, such as `hints` aren't reused and interpreted differently depending on the property type.
It also allows properties to have more members than PropertyHandle defines.

There are, however, no actual benefits in terms of features.
Instead, the OOP approach would probably introduce more development overhead because classes have to be derived for any customizations, such as setters, which is not desirable.
To workaround this, there would be a class that takes a setter lambda and calls that in `set()`. This basically comes down to the current data oriented approach but with additional OOP overhead.
Additionally, the OOP approach might introduce inheritance problems (e.g. dimaond inheritance).

It is therefore probably not a good idea to rewrite the property system to the OOP approach.
There should be, however, a clear documentation how properties work and how different property types behave, aswell as a respective register function for each property type, that takes exactly the parameters it needs with proper description.

For example
```
void registerBitflags(const std::string& name, unsigned int& flags, const char* const* flagnames, size_t size)
void registerInt(const std::string& name, int& var, int min, int max)
```



