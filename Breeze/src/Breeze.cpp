#include "Breeze.h"

class MyClass : public overtime::application {
public:
	MyClass()
	{}
	~MyClass()
	{}

private:

};

overtime::application* overtime::createApplication()
{

	return new MyClass();

}
