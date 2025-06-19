#include "themeManager.h"
#include <map>
using namespace overtime;

static std::map<std::string, ref<style>>* storage;

style::style(overtime::ref<overtime::texture2D> texture, float textureSize, glm::vec4 color)
	:_texture(texture), _textureSize(textureSize), _color(color)
{}

style::style(glm::vec4 color)
	:_color(color)
{
	_textureSize = 1.0f;
	_texture = texture2D::create(1, 1);
	uint32_t whiteData = 0xffffffff;
	_texture->setData(&whiteData, sizeof(uint32_t));
}

void themeManager::init()
{
	storage = new std::map<std::string, ref<style>>();
}

void themeManager::shutdown()
{
	delete storage;
}

void themeManager::add(const std::string& id, style& newStyle)
{
	auto res = storage->try_emplace(id, std::make_shared<style>(newStyle));
	OT_ASSERT(res.second, "Style already exists!");
}

const ref<style> themeManager::getStyle(const std::string& id)
{
	auto it = storage->find(id);
	OT_ASSERT(it != storage->end(), "Style not exists!");
	return (*it).second;
}