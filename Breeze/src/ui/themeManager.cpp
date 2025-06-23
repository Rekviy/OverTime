#include "themeManager.h"
#include <map>
using namespace overtime;
namespace breeze {
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

	style::style(const style& other) noexcept
	{
		this->_color = other._color;
		this->_texture = other._texture;
		this->_textureSize = other._textureSize;
	}

	style::style(style&& other) noexcept
	{
		this->_color = std::move(other._color);
		this->_texture = std::move(other._texture);
		this->_textureSize = std::move(other._textureSize);
	}

	style& style::operator=(const style& other)
	{
		this->_color = other._color;
		this->_texture = other._texture;
		this->_textureSize = other._textureSize;
		return *this;
	}

	void themeManager::init()
	{
		storage = new std::map<std::string, ref<style>>();
	}

	void themeManager::shutdown()
	{
		delete storage;
	}

	void themeManager::add(const std::string& id, const style& newStyle)
	{
		auto res = storage->try_emplace(id, std::make_shared<style>(newStyle));
		OT_ASSERT(res.second, "Style already exists!");
	}
	void themeManager::add(const std::string& id, style&& newStyle)
	{
		auto res = storage->try_emplace(id, std::make_shared<style>(newStyle));
		OT_ASSERT(res.second, "Style already exists!");
	}
	const ref<style> themeManager::getStyle(const std::string& id)
	{
		auto it = storage->find(id);
		if (it == storage->end())
			throw std::runtime_error("Style not exists. Id: " + id);

		return (*it).second;
	}

	bool themeManager::isKeysValid(const std::vector<std::string>& idsToValidate)
	{
		for (auto& id : idsToValidate)
			if (storage->find(id) == storage->end())
				return false;
		return true;
	}
}