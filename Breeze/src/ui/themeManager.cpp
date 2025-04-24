#include "themeManager.h"
#include <map>
using namespace overtime;

static std::map<std::string, ref<themeManager::style>>* storage;

themeManager::style::style(overtime::ref<overtime::texture2D> texture, float textureSize, glm::vec4 color)
	:_texture(texture), _textureSize(textureSize), _color(color)
{}

themeManager::style::style(glm::vec4 color)
	:_color(color)
{
	_textureSize = 1.0f;
	_texture = texture2D::create(1, 1);
	uint32_t whiteData = 0xffffffff;
	_texture->setData(&whiteData, sizeof(uint32_t));
}

void themeManager::init()
{
	storage = new std::map<std::string, ref<themeManager::style>>();
}

void themeManager::shutdown()
{
	delete storage;
}

void themeManager::add(const std::string& id, themeManager::style& style)
{
	auto res = storage->try_emplace(id, std::make_shared<themeManager::style>(style));
	OT_ASSERT(res.second, "Style already exists!");
}

const ref<themeManager::style>& themeManager::getStyle(const std::string& id)
{
	auto it = storage->find(id);
	OT_ASSERT(it != storage->end(), "Style already exists!");
	return (*it).second;
}

