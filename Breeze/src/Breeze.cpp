#include "Breeze.h"

class MyClass : public overtime::Application {
public:
	MyClass() {
	}
	~MyClass() {
	}

private:

};

overtime::Application* overtime::createApplication() {

	return new MyClass();

}
