#pragma once
#include <string>
#include <memory>
#include <unordered_map>
#include <glm/glm.hpp>
#include "Texture2D.h"

struct SDL_Color;
namespace dae
{
	class Font;

	class BaseResourceManager
	{
	public:
		virtual ~BaseResourceManager() = default;
		virtual Texture2D* LoadTexture(const std::string& file) = 0;
		virtual std::unique_ptr<Texture2D> LoadTexture(Font* pFont, const glm::vec4& color, const std::string& text) = 0;
		virtual std::shared_ptr<Font> LoadFont(const std::string& file, unsigned int size) const = 0;
	};

	class NullResourceManager : public BaseResourceManager
	{
	public:
		virtual ~NullResourceManager() = default;
		virtual Texture2D* LoadTexture(const std::string&) override { return nullptr; }
		virtual std::unique_ptr<Texture2D> LoadTexture(Font*, const glm::vec4&, const std::string&) override { return nullptr; }
		virtual std::shared_ptr<Font> LoadFont(const std::string&, unsigned int) const override { return nullptr; }
	};

	class ResourceManager : public BaseResourceManager
	{
	public:
		ResourceManager();
		virtual ~ResourceManager() = default;
		virtual Texture2D* LoadTexture(const std::string& file) override;
		virtual std::unique_ptr<Texture2D> LoadTexture(Font* pFont, const glm::vec4& color, const std::string& text) override;
		virtual std::shared_ptr<Font> LoadFont(const std::string& file, unsigned int size) const override;
	private:
		std::string m_DataPath{ "../Data/" };
		std::unordered_map<std::string, std::unique_ptr<Texture2D>> m_mTextures;
	};

	class LoggingResourceManager : public BaseResourceManager
	{
	public:
		LoggingResourceManager(std::unique_ptr<BaseResourceManager>&& pResourceManager) : m_pResourceManager{ std::move(pResourceManager) } {}
		virtual ~LoggingResourceManager() = default;
		virtual Texture2D* LoadTexture(const std::string& file) override;
		virtual std::unique_ptr<Texture2D> LoadTexture(Font*, const glm::vec4&, const std::string&) override;
		virtual std::shared_ptr<Font> LoadFont(const std::string&, unsigned int) const override;
	private:
		std::unique_ptr<BaseResourceManager> m_pResourceManager;
	};
}
