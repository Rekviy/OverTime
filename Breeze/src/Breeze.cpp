#include "Breeze.h"
class testLayer : public overtime::layer {
public:
	bool show_another_window = true;
	testLayer() : layer("testLayer") {
	}
	void onUpdate() override
	{
		//OT_INFO("testLayer - Update");
		if (show_another_window) {
			ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
			ImGui::Text("Hello from another window!");
			if (ImGui::Button("Close Me"))
				show_another_window = false;
			ImGui::End();
		}
	}
	void onEvent(overtime::event& event) override
	{
	}
};
class Breeze : public overtime::application {
public:
	Breeze()
	{
		pushImGuiLayer(new testLayer());
	}
	~Breeze()
	{}

private:

};

overtime::application* overtime::createApplication()
{
	return new Breeze();

}
