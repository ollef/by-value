`Example.h`:

```c++
struct Base
{
	virtual void PrintMe() = 0;
	virtual ~Base();
};

struct A : Base
{
	void PrintMe();
};

struct B : Base
{
	void PrintMe();
};
```

`Example.cpp`:
```c++
#include <iostream>
#include "by_value.h"

#include "Example.h"

Base::~Base() { }

void A::PrintMe()
{
	std::cout << "I'm an A" << std::endl;
}

void B::PrintMe()
{
	std::cout << "I'm a B" << std::endl;
}

using BaseByValue = poly::by_value<Base, poly::max<A, B>::size, poly::max<A, B>::alignment>;

int main()
{
	BaseByValue base{A()};
	base->PrintMe();
	base = B();
	base->PrintMe();
	return 0;
}
```

Prints:
```
I'm an A
I'm a B
```

